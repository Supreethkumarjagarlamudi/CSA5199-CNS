#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char keySquare[SIZE][SIZE];

void generateKey(char key[]){
    int alphabet[26] = {0};
    int row = 0, col = 0;
    char ch;

    for (int i = 0; key[i] != '\0'; i++) {
        ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';
        if (!alphabet[ch - 'A']) {
            keySquare[row][col++] = ch;
            alphabet[ch - 'A'] = 1;
            if (col == SIZE) { row++; col = 0; }
        }
    }
    for (ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!alphabet[ch - 'A']) {
            keySquare[row][col++] = ch;
            alphabet[ch - 'A'] = 1;
            if (col == SIZE) { row++; col = 0; }
        }
    }
}
void printMatrix() {
    printf("\nPlayfair 5x5 Key Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keySquare[i][j]);
        }
        printf("\n");
    }
}
void formatPlainText(char plainText[], char formattedText[]){
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
void findPosition(char ch, int *row, int *col){
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keySquare[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }    
}


void encrypt(char plainText[], char cipherText[]){
    char formattedText[100];
    formatPlainText(plainText, formattedText);
    int len = strlen(formattedText);
    int row1, col1, row2, col2;
    for (int i = 0; i < len; i += 2) {
        findPosition(formattedText[i], &row1, &col1);
        findPosition(formattedText[i + 1], &row2, &col2);

        if (row1 == row2) {
            cipherText[i] = keySquare[row1][(col1 + 1) % SIZE];
            cipherText[i + 1] = keySquare[row2][(col2 + 1) % SIZE];
        }
        else if (col1 == col2) {
            cipherText[i] = keySquare[(row1 + 1) % SIZE][col1];
            cipherText[i + 1] = keySquare[(row2 + 1) % SIZE][col2];
        }
        else {
            cipherText[i] = keySquare[row1][col2];
            cipherText[i + 1] = keySquare[row2][col1];
        }
    }
    cipherText[len] = '\0';
}

int main() {
    char key[50], plainText[100], cipherText[100];
    printf("Enter the keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    printf("Enter the plain text: ");
    fgets(plainText, sizeof(plainText), stdin);
    key[strcspn(plainText, "\n")] = '\0';
    

    generateKey(key);
    printMatrix();

    encrypt(plainText, cipherText);
    printf("cipherText: %s\n", cipherText);

}