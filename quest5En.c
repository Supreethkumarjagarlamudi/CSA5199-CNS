#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26

void generateCipherAlphabet(char keyword[], char cipherAlphabet[]) {
    int used[26] = {0};
    int index = 0;
    for (int i = 0; keyword[i] != '\0'; i++) {
        char ch = toupper(keyword[i]);
        if (!used[ch - 'A']) {
            cipherAlphabet[index++] = ch;
            used[ch - 'A'] = 1;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!used[ch - 'A']) {
            cipherAlphabet[index++] = ch;
        }
    }
    cipherAlphabet[index] = '\0';
}

void encrypt(char plaintext[], char cipherAlphabet[], char ciphertext[]) {
    char plainAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = toupper(plaintext[i]);
        if (isalpha(ch)) {
            int pos = ch - 'A';
            ciphertext[i] = cipherAlphabet[pos];
        } else {
            ciphertext[i] = ch;
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

int main() {
    char keyword[] = "CIPHER";
    char cipherAlphabet[ALPHABET_LEN + 1];
    char plaintext[100], ciphertext[100], decryptedText[100];
    generateCipherAlphabet(keyword, cipherAlphabet);
    printf("Cipher Alphabet: %s\n", cipherAlphabet);
    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';
    encrypt(plaintext, cipherAlphabet, ciphertext);
    printf("Encrypted Text: %s\n", ciphertext);

    return 0;
}