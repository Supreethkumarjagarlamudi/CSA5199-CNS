#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26
int key[2][2] = {{9, 4}, {5, 7}};

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; 
}

void computeInverseKey(int inverseKey[2][2]) {
    int det = (key[0][0] * key[1][1] - key[0][1] * key[1][0]) % MOD;
    if (det < 0) det += MOD;

    int detInverse = modInverse(det, MOD);
    if (detInverse == -1) {
        printf("Inverse key does not exist!\n");
        return;
    }

    inverseKey[0][0] = (key[1][1] * detInverse) % MOD;
    inverseKey[0][1] = (-key[0][1] * detInverse) % MOD;
    inverseKey[1][0] = (-key[1][0] * detInverse) % MOD;
    inverseKey[1][1] = (key[0][0] * detInverse) % MOD;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (inverseKey[i][j] < 0)
                inverseKey[i][j] += MOD;
        }
    }
}
void encrypt(char plaintext[], char ciphertext[]) {
    int len = strlen(plaintext);
    int numText[100], encText[100];

    for (int i = 0; i < len; i++) {
        numText[i] = plaintext[i] - 'A';
    }

    for (int i = 0; i < len; i += 2) {
        encText[i] = (key[0][0] * numText[i] + key[0][1] * numText[i + 1]) % MOD;
        encText[i + 1] = (key[1][0] * numText[i] + key[1][1] * numText[i + 1]) % MOD;
    }

    for (int i = 0; i < len; i++) {
        ciphertext[i] = encText[i] + 'A';
    }
    ciphertext[len] = '\0';
}

void decrypt(char ciphertext[], char decryptedText[]) {
    int len = strlen(ciphertext);
    int numText[100], decText[100], inverseKey[2][2];

    computeInverseKey(inverseKey);

    for (int i = 0; i < len; i++) {
        numText[i] = ciphertext[i] - 'A';
    }

    for (int i = 0; i < len; i += 2) {
        decText[i] = (inverseKey[0][0] * numText[i] + inverseKey[0][1] * numText[i + 1]) % MOD;
        decText[i + 1] = (inverseKey[1][0] * numText[i] + inverseKey[1][1] * numText[i + 1]) % MOD;
    }

    for (int i = 0; i < len; i++) {
        decryptedText[i] = decText[i] + 'A';
    }
    decryptedText[len] = '\0';
}

void formatText(char text[], char formattedText[]) {
    int len = strlen(text), j = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            formattedText[j++] = toupper(text[i]);
        }
    }

    if (j % 2 != 0) {
        formattedText[j++] = 'X';
    }
    formattedText[j] = '\0';
}

int main() {
    char plaintext[] = "meet me at the usual place at ten rather than eight oclock";
    char formattedText[100], ciphertext[100], decryptedText[100];

    formatText(plaintext, formattedText);
    printf("\nFormatted Plaintext: %s\n", formattedText);

    encrypt(formattedText, ciphertext);
    printf("\nEncrypted Text: %s\n", ciphertext);

    decrypt(ciphertext, decryptedText);
    printf("\nDecrypted Text: %s\n", decryptedText);

    return 0;
}