#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to calculate modular inverse of 'a' under mod 26
int mod_inverse(int a, int m) {
    for (int i = 1; i < m; i++) {
        if ((a * i) % m == 1) return i;
    }
    return -1; // No modular inverse exists
}

// Encrypt a character using Affine Cipher
char encrypt_char(char p, int a, int b) {
    if (p >= 'A' && p <= 'Z') { // Uppercase letters
        return (char)((((a * (p - 'A')) + b) % 26) + 'A');
    }
    if (p >= 'a' && p <= 'z') { // Lowercase letters
        return (char)((((a * (p - 'a')) + b) % 26) + 'a');
    }
    return p; // Keep other characters unchanged
}

// Decrypt a character using Affine Cipher
char decrypt_char(char c, int a, int b) {
    int a_inv = mod_inverse(a, 26);
    if (a_inv == -1) {
        printf("Error: 'a' has no modular inverse under mod 26.\n");
        exit(1);
    }

    if (c >= 'A' && c <= 'Z') { // Uppercase
        return (char)((a_inv * ((c - 'A' - b + 26)) % 26) + 'A');
    }
    if (c >= 'a' && c <= 'z') { // Lowercase
        return (char)((a_inv * ((c - 'a' - b + 26)) % 26) + 'a');
    }
    return c; // Keep other characters unchanged
}

// Encrypt a string using Affine Cipher
void encrypt(char *plaintext, char *ciphertext, int a, int b) {
    for (int i = 0; plaintext[i] != '\0'; i++) {
        ciphertext[i] = encrypt_char(plaintext[i], a, b);
    }
    ciphertext[strlen(plaintext)] = '\0';
}

// Decrypt a string using Affine Cipher
void decrypt(char *ciphertext, char *plaintext, int a, int b) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        plaintext[i] = decrypt_char(ciphertext[i], a, b);
    }
    plaintext[strlen(ciphertext)] = '\0';
}

// Check if 'a' is coprime with 26
int is_coprime(int a) {
    int gcd = 26;
    int temp;
    while (a != 0) {
        temp = gcd % a;
        gcd = a;
        a = temp;
    }
    return (gcd == 1);
}

int main() {
    char plaintext[100], ciphertext[100], decrypted[100];
    int a, b;

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0; // Remove newline

    // Get valid 'a' (must be coprime to 26)
    do {
        printf("Enter 'a' (must be coprime with 26): ");
        scanf("%d", &a);
        if (!is_coprime(a)) {
            printf("Invalid 'a', must be coprime with 26. Try again.\n");
        }
    } while (!is_coprime(a));

    printf("Enter 'b': ");
    scanf("%d", &b);

    encrypt(plaintext, ciphertext, a, b);
    printf("\nEncrypted Text: %s\n", ciphertext);

    decrypt(ciphertext, decrypted, a, b);
    printf("Decrypted Text: %s\n", decrypted);

    return 0;
}