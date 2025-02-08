#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Simple DES implementation (this is a mock implementation for educational purposes)
void des_encrypt(uint64_t *data, uint64_t key) {
    // This is a simplified mock implementation of DES encryption
    // In a real implementation, DES would perform several permutations and XOR operations
    *data ^= key;  // For simplicity, we're just XORing data with the key
}

// 3DES encryption function (applies DES three times)
void triple_des_encrypt(uint64_t *data, uint64_t key1, uint64_t key2, uint64_t key3) {
    // Encrypt with key1
    des_encrypt(data, key1);
    // Encrypt with key2
    des_encrypt(data, key2);
    // Encrypt with key3
    des_encrypt(data, key3);
}

// XOR two blocks of data (to implement CBC mode)
void xor_blocks(uint64_t *block1, uint64_t *block2) {
    *block1 ^= *block2;
}

// CBC Mode encryption for 3DES
void cbc_encrypt(uint64_t *plaintext, size_t len, uint64_t *key1, uint64_t *key2, uint64_t *key3, uint64_t *iv, uint64_t *ciphertext) {
    uint64_t previous_block = *iv;  // Initialize previous block as the IV

    for (size_t i = 0; i < len; ++i) {
        // XOR the plaintext block with the previous ciphertext block (or IV for the first block)
        xor_blocks(&plaintext[i], &previous_block);

        // Encrypt the block with 3DES
        triple_des_encrypt(&plaintext[i], *key1, *key2, *key3);

        // Store the ciphertext
        ciphertext[i] = plaintext[i];

        // Set the current ciphertext as the previous block for the next iteration
        previous_block = ciphertext[i];
    }
}

// Helper function to print data in hexadecimal format
void hex_print(uint64_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%016llx ", data[i]);
    }
    printf("\n");
}

int main() {
    // Sample plaintext (16 bytes = 2 blocks of 64-bits)
    uint64_t plaintext[] = {
        0x0123456789ABCDEF,  // First block
        0x1234567890ABCDEF   // Second block
    };
    size_t len = sizeof(plaintext) / sizeof(plaintext[0]);

    // 3DES keys (each key is 64 bits = 8 bytes)
    uint64_t key1 = 0x0F0F0F0F0F0F0F0F;
    uint64_t key2 = 0x1F1F1F1F1F1F1F1F;
    uint64_t key3 = 0x2F2F2F2F2F2F2F2F;

    // Initialization Vector (IV) for CBC mode
    uint64_t iv = 0x0000000000000001;

    // Ciphertext array
    uint64_t ciphertext[len];

    // Perform 3DES CBC encryption
    cbc_encrypt(plaintext, len, &key1, &key2, &key3, &iv, ciphertext);

    // Print the ciphertext
    printf("Encrypted ciphertext in CBC mode (hex):\n");
    hex_print(ciphertext, len);

    return 0;
}