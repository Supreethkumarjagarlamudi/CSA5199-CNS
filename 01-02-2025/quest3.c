#include <stdio.h>
#include <stdint.h>

// Initial Permutation (IP) and Final Permutation (FP) Tables
int IP[64] = { 58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
               62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
               57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
               61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7 };

// Final Permutation (IP⁻¹)
int FP[64] = { 40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
               38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
               36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
               34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25 };

// Expansion Table (E)
int E[48] = { 32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,
              12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,
              24,25,26,27,28,29,28,29,30,31,32,1 };

// Permutation Table (P)
int P[32] = { 16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
              2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25 };

// S-Boxes (6-bit input, 4-bit output)
int S[8][4][16] = {
    { {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
      {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
      {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
      {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} },

    { {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
      {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
      {0,14,7,11,10,4,13,1,5,8,12,9,3,15,2,6},
      {1,15,13,8,10,7,4,14,12,9,5,11,3,2,6,0} },

    { {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
      {1,14,7,4,13,10,8,15,9,3,5,11,12,0,2,6},
      {8,15,3,10,1,13,4,7,9,11,14,2,12,5,6,0},
      {15,13,10,0,3,8,9,14,5,1,11,7,12,2,4,6} },

    { {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
      {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
      {0,14,7,11,10,4,13,1,5,8,12,9,3,15,2,6},
      {1,15,13,8,10,7,4,14,12,9,5,11,3,2,6,0} },

    // Add all remaining S-Boxes (sufficient for DES)
};

// Function to generate subkeys (in reverse order for decryption)
void generateKeys(uint64_t key, uint64_t subkeys[16]) {
    // Simplified key schedule for illustration
    for (int i = 0; i < 16; i++) {
        subkeys[15 - i] = key ^ (i + 1); // Dummy key generation
    }
}

// Feistel function
uint32_t feistel(uint32_t R, uint64_t subkey) {
    uint64_t expanded = 0;
    for (int i = 0; i < 48; i++) {
        expanded |= ((uint64_t)((R >> (32 - E[i])) & 1)) << (47 - i);
    }

    expanded ^= subkey;

    // Use S-Boxes (dummy, replace with full S-Box logic)
    uint32_t permuted = 0;
    for (int i = 0; i < 32; i++) {
        permuted |= ((expanded >> (32 - P[i])) & 1) << (31 - i);
    }

    return permuted;
}

// Permutation function
uint64_t permute(uint64_t block, int *table, int size) {
    uint64_t permuted = 0;
    for (int i = 0; i < size; i++) {
        permuted |= ((block >> (64 - table[i])) & 1) << (size - 1 - i);
    }
    return permuted;
}

// DES Decryption
uint64_t desDecrypt(uint64_t ciphertext, uint64_t key) {
    uint64_t subkeys[16];
    generateKeys(key, subkeys);

    // Initial Permutation
    uint64_t permuted = permute(ciphertext, IP, 64);

    // Split into L and R
    uint32_t L = permuted >> 32;
    uint32_t R = permuted & 0xFFFFFFFF;

    // 16 Feistel rounds in reverse order
    for (int i = 0; i < 16; i++) {
        uint32_t temp = L;
        L = R;
        R = temp ^ feistel(R, subkeys[i]);
    }

    // Final Permutation
    uint64_t preOutput = ((uint64_t)R << 32) | L;
    uint64_t plaintext = permute(preOutput, FP, 64);

    return plaintext;
}

int main() {
    uint64_t ciphertext = 0x123456789ABCDEF0;
    uint64_t key = 0x133457799BBCDFF1;

    uint64_t plaintext = desDecrypt(ciphertext, key);
    printf("Decrypted Plaintext: %016llX\n", plaintext);
    return 0;
}