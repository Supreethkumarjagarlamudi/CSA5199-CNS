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

void decrypt(char ciphertext[], char cipherAlphabet[], char plaintext[]) {
    char plainAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = toupper(ciphertext[i]);
        if (isalpha(ch)) {
            char *pos = strchr(cipherAlphabet, ch);
            plaintext[i] = plainAlphabet[pos - cipherAlphabet];
        } else {
            plaintext[i] = ch;
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}
int main() {
    char keyword[] = "CIPHER";
    char cipherAlphabet[ALPHABET_LEN + 1];
    char plaintext[100], ciphertext[100], decryptedText[100];
    generateCipherAlphabet(keyword, cipherAlphabet);
    printf("Cipher Alphabet: %s\n", cipherAlphabet);
    printf("Enter cipherText: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';
    decrypt(ciphertext, cipherAlphabet, decryptedText);
    printf("Decrypted Text: %s\n", decryptedText);

    return 0;
}