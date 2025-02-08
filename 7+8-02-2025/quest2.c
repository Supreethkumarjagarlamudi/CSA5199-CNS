#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 8 

const uint8_t S_BOX[64] = {
    0x3, 0xf, 0xe, 0x1, 0x0, 0x5, 0xa, 0x7,
    0x6, 0x9, 0xc, 0x8, 0x4, 0xd, 0x2, 0xb,
    0xf, 0x0, 0x8, 0xe, 0x7, 0x2, 0x1, 0xa,
    0x4, 0xd, 0x9, 0x3, 0xc, 0x6, 0xb, 0x5,
    0xa, 0x3, 0x6, 0x8, 0xe, 0xc, 0x5, 0xb,
    0x0, 0x9, 0xd, 0x2, 0x7, 0x1, 0xf, 0x4,
    0x1, 0xe, 0x2, 0xc, 0xa, 0x6, 0x8, 0xf,
    0x9, 0x4, 0x7, 0x3, 0xd, 0x5, 0x0, 0xb
};
uint32_t feistel(uint32_t half_block, uint8_t key) {
    return ((half_block << 1) | (half_block >> 31)) ^ S_BOX[key % 64];
}

uint64_t simple_des_encrypt(uint64_t block, uint8_t key) {
    uint32_t left = block >> 32;
    uint32_t right = block & 0xFFFFFFFF;

    for (int i = 0; i < 16; i++) {
        uint32_t temp = right;
        right = left ^ feistel(right, key);
        left = temp;
    }

    return ((uint64_t)left << 32) | right;
}
void triple_des_encrypt(uint64_t *plaintext, uint64_t *ciphertext, int blocks, uint64_t key1, uint64_t key2, uint64_t key3, uint64_t iv) {
    uint64_t prev_cipher = iv;

    for (int i = 0; i < blocks; i++) {
        plaintext[i] ^= prev_cipher;
        
        uint64_t temp = simple_des_encrypt(plaintext[i], key1);
        temp = simple_des_encrypt(temp, key2); 
        ciphertext[i] = simple_des_encrypt(temp, key3);

        prev_cipher = ciphertext[i]; 
    }
}
void print_hex(uint64_t *data, int blocks) {
    for (int i = 0; i < blocks; i++) {
        printf("%016llx ", data[i]);
    }
    printf("\n");
}

int main() {
    uint64_t plaintext[] = {0x123456789abcdef0, 0xfedcba9876543210};
    int blocks = sizeof(plaintext) / sizeof(plaintext[0]);

    uint64_t key1 = 0xAABB09182736CCDD;
    uint64_t key2 = 0xCCDDAABB09182736;
    uint64_t key3 = 0x09182736CCDDDDEE;

    uint64_t iv = 0x0123456789ABCDEF;
    uint64_t ciphertext[blocks];

    printf("Plaintext:  ");
    print_hex(plaintext, blocks);

    triple_des_encrypt(plaintext, ciphertext, blocks, key1, key2, key3, iv);

    printf("Ciphertext: ");
    print_hex(ciphertext, blocks);

    return 0;
}