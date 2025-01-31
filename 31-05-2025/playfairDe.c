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
void decrypt(char ciphertext[], char decryptedText[]) {
    int len = strlen(ciphertext);
    int row1, col1, row2, col2;

    for (int i = 0; i < len; i += 2) {
        findPosition(ciphertext[i], &row1, &col1);
        findPosition(ciphertext[i + 1], &row2, &col2);

        if (row1 == row2) {
            decryptedText[i] = keySquare[row1][(col1 + SIZE - 1) % SIZE];
            decryptedText[i + 1] = keySquare[row2][(col2 + SIZE - 1) % SIZE];
        }
        else if (col1 == col2) {
            decryptedText[i] = keySquare[(row1 + SIZE - 1) % SIZE][col1];
            decryptedText[i + 1] = keySquare[(row2 + SIZE - 1) % SIZE][col2];
        }
        else {
            decryptedText[i] = keySquare[row1][col2];
            decryptedText[i + 1] = keySquare[row2][col1];
        }
    }
    decryptedText[len] = '\0';
}


int main() {
    char key[50], cipherText[100], decryptedText[100];
    printf("Enter the keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    printf("Enter the cipher text: ");
    fgets(cipherText, sizeof(cipherText), stdin);
    key[strcspn(cipherText, "\n")] = '\0';
    

    generateKey(key);
    printMatrix();

    decrypt(cipherText, decryptedText);
    printf("cipherText: %s\n", decryptedText);

}