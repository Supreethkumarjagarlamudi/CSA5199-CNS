#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE 8  // S-DES uses 8-bit blocks

// Initial 8-bit IV (10101010 in binary)
uint8_t IV = 0b10101010;

// Permutation tables
const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[]  = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[]  = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV[] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[]  = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[]  = {2, 4, 3, 1};

// S-Boxes
const int SBOX1[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};
const int SBOX2[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Permutation function
uint16_t permute(uint16_t key, const int *table, int n) {
    uint16_t permuted = 0;
    for (int i = 0; i < n; i++) {
        permuted |= ((key >> (10 - table[i])) & 1) << (n - 1 - i);
    }
    return permuted;
}

// Circular Left Shift
uint16_t left_shift(uint16_t key, int shifts) {
    return ((key << shifts) | (key >> (5 - shifts))) & 0b11111;
}

// Generate subkeys
void generate_keys(uint16_t key, uint8_t *K1, uint8_t *K2) {
    key = permute(key, P10, 10);
    uint16_t left = (key >> 5) & 0b11111;
    uint16_t right = key & 0b11111;

    left = left_shift(left, 1);
    right = left_shift(right, 1);
    *K1 = permute((left << 5) | right, P8, 8);

    left = left_shift(left, 2);
    right = left_shift(right, 2);
    *K2 = permute((left << 5) | right, P8, 8);
}

// S-Box lookup
uint8_t sbox_lookup(uint8_t input, const int SBOX[4][4]) {
    int row = ((input & 0b1000) >> 2) | (input & 0b0001);
    int col = (input & 0b0110) >> 1;
    return SBOX[row][col];
}

// Function F for Feistel
uint8_t function_F(uint8_t right, uint8_t key) {
    uint8_t expanded = 0;
    for (int i = 0; i < 8; i++) {
        expanded |= ((right >> (4 - EP[i])) & 1) << (7 - i);
    }
    expanded ^= key;
    uint8_t left_sbox = sbox_lookup((expanded >> 4) & 0b1111, SBOX1);
    uint8_t right_sbox = sbox_lookup(expanded & 0b1111, SBOX2);
    uint8_t combined = (left_sbox << 2) | right_sbox;

    uint8_t output = 0;
    for (int i = 0; i < 4; i++) {
        output |= ((combined >> (4 - P4[i])) & 1) << (3 - i);
    }
    return output;
}

// S-DES Encryption
uint8_t sdes_encrypt(uint8_t plaintext, uint8_t K1, uint8_t K2) {
    plaintext = permute(plaintext, IP, 8);
    uint8_t left = (plaintext >> 4) & 0b1111;
    uint8_t right = plaintext & 0b1111;

    uint8_t temp = function_F(right, K1);
    left ^= temp;

    uint8_t combined = (right << 4) | left;
    left = (combined >> 4) & 0b1111;
    right = combined & 0b1111;

    temp = function_F(right, K2);
    left ^= temp;

    uint8_t ciphertext = permute((right << 4) | left, IP_INV, 8);
    return ciphertext;
}

// S-DES Decryption
uint8_t sdes_decrypt(uint8_t ciphertext, uint8_t K1, uint8_t K2) {
    return sdes_encrypt(ciphertext, K2, K1);
}

// CBC Mode Encryption
void cbc_encrypt(uint8_t *plaintext, uint8_t *ciphertext, int blocks, uint8_t K1, uint8_t K2) {
    uint8_t prev_cipher = IV;
    for (int i = 0; i < blocks; i++) {
        plaintext[i] ^= prev_cipher;
        ciphertext[i] = sdes_encrypt(plaintext[i], K1, K2);
        prev_cipher = ciphertext[i];

        // Debug print
        printf("CBC Encryption Step %d: ", i);
        printf("Ciphertext: ");
        for (int j = 7; j >= 0; j--) printf("%d", (ciphertext[i] >> j) & 1);
        printf("\n");
    }
}

// CBC Mode Decryption
void cbc_decrypt(uint8_t *ciphertext, uint8_t *plaintext, int blocks, uint8_t K1, uint8_t K2) {
    uint8_t prev_cipher = IV;
    for (int i = 0; i < blocks; i++) {
        uint8_t temp = sdes_decrypt(ciphertext[i], K1, K2);
        plaintext[i] = temp ^ prev_cipher;
        prev_cipher = ciphertext[i];

        // Debug print
        printf("CBC Decryption Step %d: ", i);
        printf("Plaintext: ");
        for (int j = 7; j >= 0; j--) printf("%d", (plaintext[i] >> j) & 1);
        printf("\n");
    }
}

// Main function
int main() {
    uint16_t key = 0b0111111101;  // 10-bit key
    uint8_t K1, K2;
    generate_keys(key, &K1, &K2);

    printf("Generated Keys:\nK1: ");
    for (int j = 7; j >= 0; j--) printf("%d", (K1 >> j) & 1);
    printf("\nK2: ");
    for (int j = 7; j >= 0; j--) printf("%d", (K2 >> j) & 1);
    printf("\n");

    uint8_t plaintext[] = {0b00000001, 0b00100011};
    uint8_t ciphertext[2], decrypted[2];

    cbc_encrypt(plaintext, ciphertext, 2, K1, K2);
    cbc_decrypt(ciphertext, decrypted, 2, K1, K2);

    return 0;
}