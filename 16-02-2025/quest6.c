#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 16

void simple_encrypt(uint8_t *block, uint8_t *key, uint8_t *output) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = block[i] ^ key[i]; 
    }
}

void compute_cbc_mac(uint8_t *message, uint8_t *key, uint8_t *mac) {
    uint8_t iv[BLOCK_SIZE] = {0}; 
    uint8_t temp[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        temp[i] = message[i] ^ iv[i];
    }

    simple_encrypt(temp, key, mac);
}

void print_block(uint8_t *block, char *label) {
    printf("%s: ", label);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[BLOCK_SIZE] = {0x1F, 0x2E, 0x3D, 0x4C, 0x5B, 0x6A, 0x79, 0x88,
                               0x97, 0xA6, 0xB5, 0xC4, 0xD3, 0xE2, 0xF1, 0x00}; 
    uint8_t message[BLOCK_SIZE] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                                    0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50}; 

    uint8_t mac[BLOCK_SIZE];
    uint8_t forged_block[BLOCK_SIZE]; 

    compute_cbc_mac(message, key, mac);
    print_block(mac, "Computed MAC (T)");

    for (int i = 0; i < BLOCK_SIZE; i++) {
        forged_block[i] = message[i] ^ mac[i];
    }
    print_block(forged_block, "Forged Block (X ⊕ T)");

    uint8_t final_mac[BLOCK_SIZE];
    compute_cbc_mac(forged_block, key, final_mac);
    print_block(final_mac, "Forged MAC (T')");

    return 0;
}