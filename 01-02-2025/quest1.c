#include <stdio.h>
#include <stdlib.h>

#define MOD 26 

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

int determinant(int key[2][2]) {
    return (key[0][0] * key[1][1] - key[0][1] * key[1][0]) % MOD;
}

void inverseMatrix(int key[2][2], int invKey[2][2]) {
    int det = determinant(key);
    if (det < 0)
        det += MOD;
    
    int detInv = modInverse(det, MOD);
    if (detInv == -1) {
        printf("Inverse does not exist (Key is not invertible).\n");
        exit(1);
    }

    invKey[0][0] = ( key[1][1] * detInv) % MOD;
    invKey[0][1] = (-key[0][1] * detInv) % MOD;
    invKey[1][0] = (-key[1][0] * detInv) % MOD;
    invKey[1][1] = ( key[0][0] * detInv) % MOD;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (invKey[i][j] < 0)
                invKey[i][j] += MOD;
}

void encrypt(int plaintext[2], int key[2][2], int ciphertext[2]) {
    for (int i = 0; i < 2; i++) {
        ciphertext[i] = (key[i][0] * plaintext[0] + key[i][1] * plaintext[1]) % MOD;
    }
}

void decrypt(int ciphertext[2], int key[2][2], int plaintext[2]) {
    int invKey[2][2];
    inverseMatrix(key, invKey);
    
    for (int i = 0; i < 2; i++) {
        plaintext[i] = (invKey[i][0] * ciphertext[0] + invKey[i][1] * ciphertext[1]) % MOD;
        if (plaintext[i] < 0)
            plaintext[i] += MOD;
    }
}

void recoverKey(int plaintext[2][2], int ciphertext[2][2], int recoveredKey[2][2]) {
    int invPlaintext[2][2];
    inverseMatrix(plaintext, invPlaintext);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            recoveredKey[i][j] = (ciphertext[i][0] * invPlaintext[0][j] + ciphertext[i][1] * invPlaintext[1][j]) % MOD;
            if (recoveredKey[i][j] < 0)
                recoveredKey[i][j] += MOD;
        }
}

int main() {
    int key[2][2] = { {9, 4}, {5, 7} };

    int plaintext[2][2] = { {7, 8}, {11, 4} }; 

    int ciphertext[2][2];
    encrypt(plaintext[0], key, ciphertext[0]);
    encrypt(plaintext[1], key, ciphertext[1]);

    printf("Original Key:\n%d %d\n%d %d\n\n", key[0][0], key[0][1], key[1][0], key[1][1]);

    printf("Known Plaintext - Ciphertext Pairs:\n");
    for (int i = 0; i < 2; i++)
        printf("(%d, %d) -> (%d, %d)\n", plaintext[i][0], plaintext[i][1], ciphertext[i][0], ciphertext[i][1]);

    int recoveredKey[2][2];
    recoverKey(plaintext, ciphertext, recoveredKey);

    printf("\nRecovered Key:\n%d %d\n%d %d\n", recoveredKey[0][0], recoveredKey[0][1], recoveredKey[1][0], recoveredKey[1][1]);

    return 0;
}