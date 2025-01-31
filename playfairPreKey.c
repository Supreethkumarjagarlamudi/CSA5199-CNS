#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5
char keySquare[SIZE][SIZE] = {
    {'M', 'F', 'H', 'I', 'K'},
    {'U', 'N', 'O', 'P', 'Q'},
    {'Z', 'V', 'W', 'X', 'Y'},
    {'E', 'L', 'A', 'R', 'G'},
    {'D', 'S', 'T', 'B', 'C'}
};
void formatPlaintext(char plainText[], char formattedText[]) {
    int len = strlen(plainText);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha(plainText[i])) {
            formattedText[j++] = toupper(plainText[i] == 'J' ? 'I' : plainText[i]);
        }
    }
    formattedText[j] = '\0';
    char finalText[100];
    int finalLen = 0;
    for (int i = 0; i < j; i++) {
        finalText[finalLen++] = formattedText[i];
        if (i < j - 1 && formattedText[i] == formattedText[i + 1]) {
            finalText[finalLen++] = 'X';
        }
    }
    if (finalLen % 2 != 0) finalText[finalLen++] = 'X';
    finalText[finalLen] = '\0';

    strcpy(formattedText, finalText);
}

void findPosition(char letter, int *row, int *col) {
    if (letter == 'J') letter = 'I'; 
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keySquare[i][j] == letter) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}
void encrypt(char formattedText[], char ciphertext[]) {
    int len = strlen(formattedText);
    int row1, col1, row2, col2;

    for (int i = 0; i < len; i += 2) {
        findPosition(formattedText[i], &row1, &col1);
        findPosition(formattedText[i + 1], &row2, &col2);
        if (row1 == row2) {
            ciphertext[i] = keySquare[row1][(col1 + 1) % SIZE];
            ciphertext[i + 1] = keySquare[row2][(col2 + 1) % SIZE];
        }
        else if (col1 == col2) {
            ciphertext[i] = keySquare[(row1 + 1) % SIZE][col1];
            ciphertext[i + 1] = keySquare[(row2 + 1) % SIZE][col2];
        }
        else {
            ciphertext[i] = keySquare[row1][col2];
            ciphertext[i + 1] = keySquare[row2][col1];
        }
    }
    ciphertext[len] = '\0';
}

void displayKeySquare() {
    printf("Predefined Playfair Key Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keySquare[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char plaintext[] = "Must see you over Cadogan West. Coming at once.";
    char formattedText[100], ciphertext[100];

    displayKeySquare();

    formatPlaintext(plaintext, formattedText);
    printf("\nFormatted Plaintext: %s\n", formattedText);

    encrypt(formattedText, ciphertext);
    printf("\nEncrypted Text: %s\n", ciphertext);

    return 0;
}