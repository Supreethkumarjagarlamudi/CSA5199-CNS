#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 8

uint64_t feistel(uint64_t data, uint64_t key) {
    return (data ^ key);
}
uint64_t encrypt_block(uint64_t block, uint64_t key) {
    return feistel(block, key);
}

void ecb_encrypt(uint64_t *plaintext, uint64_t *ciphertext, int blocks, uint64_t key) {
    for (int i = 0; i < blocks; i++) {
        ciphertext[i] = encrypt_block(plaintext[i], key);
    }
}
void cbc_encrypt(uint64_t *plaintext, uint64_t *ciphertext, int blocks, uint64_t key, uint64_t iv) {
    uint64_t prev_cipher = iv;
    for (int i = 0; i < blocks; i++) {
        plaintext[i] ^= prev_cipher; 
        ciphertext[i] = encrypt_block(plaintext[i], key);
        prev_cipher = ciphertext[i]; 
    }
}
void cfb_encrypt(uint64_t *plaintext, uint64_t *ciphertext, int blocks, uint64_t key, uint64_t iv) {
    uint64_t feedback = iv;
    for (int i = 0; i < blocks; i++) {
        feedback = encrypt_block(feedback, key); 
        ciphertext[i] = plaintext[i] ^ feedback; 
        feedback = ciphertext[i];  
    }
}
void apply_padding(uint64_t *data, int *blocks) {
    int last_block = *blocks - 1;
    uint8_t *byte_ptr = (uint8_t *)&data[last_block];

    for (int i = BLOCK_SIZE - 1; i >= 0; i--) {
        if (byte_ptr[i] != 0) {
            byte_ptr[i + 1] = 0x80;
            for (int j = i + 2; j < BLOCK_SIZE; j++) {
                byte_ptr[j] = 0x00; 
            }
            return;
        }
    }
    byte_ptr[0] = 0x80;
    for (int i = 1; i < BLOCK_SIZE; i++) {
        byte_ptr[i] = 0x00;
    }
}

void print_blocks(uint64_t *data, int blocks, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < blocks; i++) {
        printf("%016llx ", data[i]);
    }
    printf("\n");
}

int main() {
    uint64_t plaintext[] = {0x123456789abcdef0, 0xfedcba9876543210};
    int blocks = sizeof(plaintext) / sizeof(plaintext[0]);

    uint64_t key = 0xAABB09182736CCDD;
    uint64_t iv = 0x0123456789ABCDEF;

    apply_padding(plaintext, &blocks);

    uint64_t ciphertext_ecb[blocks];
    uint64_t ciphertext_cbc[blocks];
    uint64_t ciphertext_cfb[blocks];

    print_blocks(plaintext, blocks, "Plaintext");

    ecb_encrypt(plaintext, ciphertext_ecb, blocks, key);
    print_blocks(ciphertext_ecb, blocks, "ECB Ciphertext");

    cbc_encrypt(plaintext, ciphertext_cbc, blocks, key, iv);
    print_blocks(ciphertext_cbc, blocks, "CBC Ciphertext");

    cfb_encrypt(plaintext, ciphertext_cfb, blocks, key, iv);
    print_blocks(ciphertext_cfb, blocks, "CFB Ciphertext");

    return 0;
}