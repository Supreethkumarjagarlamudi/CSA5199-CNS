#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

char key[ALPHABET_SIZE + 1] = "MUALVOZKRNJXQDFSHPEBCTIWYG";

void decrypt(char cipherText[], char decryptedText[], char key[]){
    char reverseKey[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        reverseKey[tolower(key[i]) - 'a'] = 'a' + i;
    }
    for (int i = 0; cipherText[i] != '\0'; i++) {
        if (isalpha(cipherText[i])) {
            int isLower = islower(cipherText[i]);
            int index = tolower(cipherText[i]) - 'a';
            decryptedText[i] = isLower ? tolower(reverseKey[index]) : reverseKey[index];
        } else {
            decryptedText[i] = cipherText[i];
        }
    }
    decryptedText[strlen(cipherText)] = '\0';
};

int main(){
    char cipherText[100], decryptedText[100];

    printf("Enter cipher text: ");
    fgets(cipherText, sizeof(cipherText), stdin);

    decrypt(cipherText, decryptedText, key);
    printf("plain text: %s\n", decryptedText);
}
