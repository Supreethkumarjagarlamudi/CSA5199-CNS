#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 16  // Block size in bytes
#define MESSAGE_SIZE 64  // Maximum message size

uint8_t key[BLOCK_SIZE] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                           0xAB, 0xF7, 0x14, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

uint8_t iv[BLOCK_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                          0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

// Simple XOR-based encryption (for testing, not secure)
void simple_encrypt_block(uint8_t *input, uint8_t *output, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];  // XOR encryption
    }
}

// Padding function (1 bit followed by zeros)
void pad_data(uint8_t *data, int *len) {
    int pad_len = BLOCK_SIZE - (*len % BLOCK_SIZE);
    data[*len] = 0x80;  // First padding byte = 1
    for (int i = *len + 1; i < *len + pad_len; i++) data[i] = 0x00;
    *len += pad_len;
}

// ECB Mode
void encrypt_ecb(uint8_t *input, uint8_t *output, int len) {
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        simple_encrypt_block(input + i, output + i, key);
    }
}

// CBC Mode
void encrypt_cbc(uint8_t *input, uint8_t *output, int len) {
    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, iv, BLOCK_SIZE); 

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        for (int j = 0; j < BLOCK_SIZE; j++) input[i + j] ^= temp[j]; 
        simple_encrypt_block(input + i, output + i, key);
        memcpy(temp, output + i, BLOCK_SIZE);  
    }
}

void encrypt_cfb(uint8_t *input, uint8_t *output, int len) {
    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, iv, BLOCK_SIZE);  

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        simple_encrypt_block(temp, temp, key);  
        for (int j = 0; j < BLOCK_SIZE; j++) output[i + j] = input[i + j] ^ temp[j]; 
        memcpy(temp, output + i, BLOCK_SIZE);  
    }
}

int main() {
    uint8_t plaintext[MESSAGE_SIZE] = "This is a test message.";
    int len = strlen((char *)plaintext);

    pad_data(plaintext, &len);

    uint8_t encrypted_ecb[MESSAGE_SIZE] = {0}, encrypted_cbc[MESSAGE_SIZE] = {0}, encrypted_cfb[MESSAGE_SIZE] = {0};


    encrypt_ecb(plaintext, encrypted_ecb, len);
    encrypt_cbc(plaintext, encrypted_cbc, len);
    encrypt_cfb(plaintext, encrypted_cfb, len);


    printf("ECB Encrypted: ");
    for (int i = 0; i < len; i++) printf("%02X ", encrypted_ecb[i]);
    printf("\n");

    printf("CBC Encrypted: ");
    for (int i = 0; i < len; i++) printf("%02X ", encrypted_cbc[i]);
    printf("\n");

    printf("CFB Encrypted: ");
    for (int i = 0; i < len; i++) printf("%02X ", encrypted_cfb[i]);
    printf("\n");

    return 0;
}