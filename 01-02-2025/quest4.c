#include <stdio.h>
#include <stdint.h>

// Initial Key Permutation (PC1)
int PC1[56] = { 57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,
                59,51,43,35,27,19,11,3,60,52,44,36,28,20,12,4,
                62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,
                63,55,47,39,31,23,15,7 };

// PC2 permutation for key selection
int PC2[48] = { 14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,
                26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,
                51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32 };

// Left shift schedule for the 16 rounds
int shifts[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

// Perform PC1 permutation
uint64_t permutePC1(uint64_t key) {
    uint64_t permutedKey = 0;
    for (int i = 0; i < 56; i++) {
        permutedKey |= ((key >> (64 - PC1[i])) & 1) << (55 - i);
    }
    return permutedKey;
}

// Perform PC2 permutation to generate a subkey
uint64_t permutePC2(uint64_t C, uint64_t D) {
    uint64_t subkey = 0;
    uint64_t combined = (C << 28) | D;  // Concatenate C and D
    for (int i = 0; i < 48; i++) {
        subkey |= ((combined >> (56 - PC2[i])) & 1) << (47 - i);
    }
    return subkey;
}

// Generate subkeys
void generateSubkeys(uint64_t key, uint64_t subkeys[16]) {
    uint64_t permutedKey = permutePC1(key);

    // Split the key into two 28-bit halves
    uint64_t C = permutedKey >> 28;   // Left 28 bits
    uint64_t D = permutedKey & 0xFFFFFFF;  // Right 28 bits

    // Generate the subkeys for the 16 rounds
    for (int round = 0; round < 16; round++) {
        // Perform the left shift on C and D
        C = (C << shifts[round]) | (C >> (28 - shifts[round]));
        D = (D << shifts[round]) | (D >> (28 - shifts[round]));
        
        // Mask to get the last 28 bits in case of overflow
        C &= 0xFFFFFFF;
        D &= 0xFFFFFFF;
        
        // Generate the subkey for the current round
        subkeys[round] = permutePC2(C, D);
    }
}

// Function to print the subkeys in hexadecimal format
void printSubkeys(uint64_t subkeys[16]) {
    for (int i = 0; i < 16; i++) {
        printf("Subkey %2d: %012llX\n", i + 1, subkeys[i]);
    }
}

int main() {
    uint64_t key = 0x133457799BBCDFF1;  // Example key (56-bit, represented as 64-bit)
    uint64_t subkeys[16];

    generateSubkeys(key, subkeys);
    printSubkeys(subkeys);
    
    return 0;
}