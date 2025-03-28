#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 16  
#define CONST_128 0x87 


void dummy_aes_encrypt(const uint8_t *input, uint8_t *output) {
    uint8_t fixed_key[BLOCK_SIZE] = {
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
        0xAB, 0xF7, 0x14, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    };
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ fixed_key[i];  
    }
}

void generate_subkeys(uint8_t *L, uint8_t *K1, uint8_t *K2, uint8_t poly) {
    int i;
    uint8_t carry = (L[0] & 0x80) ? 1 : 0; 

    for (i = 0; i < BLOCK_SIZE - 1; i++) {
        K1[i] = (L[i] << 1) | (L[i + 1] >> 7);
    }
    K1[BLOCK_SIZE - 1] = (L[BLOCK_SIZE - 1] << 1);
    if (carry) K1[BLOCK_SIZE - 1] ^= poly;

    carry = (K1[0] & 0x80) ? 1 : 0;
    for (i = 0; i < BLOCK_SIZE - 1; i++) {
        K2[i] = (K1[i] << 1) | (K1[i + 1] >> 7);
    }
    K2[BLOCK_SIZE - 1] = (K1[BLOCK_SIZE - 1] << 1);
    if (carry) K2[BLOCK_SIZE - 1] ^= poly;
}

int main() {
    uint8_t L[BLOCK_SIZE] = {0};   
    uint8_t K1[BLOCK_SIZE] = {0};   
    uint8_t K2[BLOCK_SIZE] = {0};   
    uint8_t zero_block[BLOCK_SIZE] = {0};

    dummy_aes_encrypt(zero_block, L);

    generate_subkeys(L, K1, K2, CONST_128);

    printf("AES-encrypted L: ");
    for (int i = 0; i < BLOCK_SIZE; i++) printf("%02X ", L[i]);
    printf("\n");

    printf("Subkey K1: ");
    for (int i = 0; i < BLOCK_SIZE; i++) printf("%02X ", K1[i]);
    printf("\n");

    printf("Subkey K2: ");
    for (int i = 0; i < BLOCK_SIZE; i++) printf("%02X ", K2[i]);
    printf("\n");

    return 0;
}