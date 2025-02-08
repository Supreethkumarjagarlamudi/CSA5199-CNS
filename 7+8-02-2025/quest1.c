#include <stdio.h>
#include <stdint.h>
int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
int IP_inv[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};
uint64_t permute(uint64_t input, const int *table, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; i++) {
        output |= ((input >> (64 - table[i])) & 1) << (size - 1 - i);
    }
    return output;
}

uint32_t feistel(uint32_t right, uint64_t subkey) {
    return right ^ (uint32_t)(subkey & 0xFFFFFFFF); 
}
uint64_t des_decrypt(uint64_t ciphertext, uint64_t key) {
    uint64_t permuted_text = permute(ciphertext, IP, 64);
    
    uint32_t left = (permuted_text >> 32) & 0xFFFFFFFF;
    uint32_t right = permuted_text & 0xFFFFFFFF;

    uint64_t subkeys[16] = {key, key, key, key, key, key, key, key, 
                            key, key, key, key, key, key, key, key};

    for (int i = 15; i >= 0; i--) { 
        uint32_t temp = right;
        right = left ^ feistel(right, subkeys[i]);
        left = temp;
    }
    uint64_t combined = ((uint64_t)right << 32) | left;

    uint64_t decrypted_text = permute(combined, IP_inv, 64);

    return decrypted_text;
}

int main() {
    uint64_t ciphertext = 0x85E813540F0AB405;
    uint64_t key = 0x133457799BBCDFF1;

    uint64_t plaintext = des_decrypt(ciphertext, key);
    printf("Decrypted plaintext: %016llX\n", plaintext);
    
    return 0;
}