#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

char key[ALPHABET_SIZE + 1] = "MUALVOZKRNJXQDFSHPEBCTIWYG";

void encrypt(char plainText[], char cipherText[], char key[]){
    for (int i = 0; plainText[i] != '\0'; i++) {
        if (isalpha(plainText[i])) {
            int isLower = islower(plainText[i]);
            int index = tolower(plainText[i]) - 'a';
            cipherText[i] = isLower ? tolower(key[index]) : key[index];
        } else {
            cipherText[i] = plainText[i]; 
        }
    }
    cipherText[strlen(plainText)] = '\0';
};
int main(){
    char plainText[100], cipherText[100];

    printf("Enter Plain Text: ");
    fgets(plainText, sizeof(plainText), stdin);

    encrypt(plainText, cipherText, key);
    printf("cipherText: %s\n", cipherText);
}