#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT_LEN 1000

double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

void decryptCaesar(char *ciphertext, int key, char *plaintext) {
    int i;
    for (i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = ((ciphertext[i] - base - key + ALPHABET_SIZE) % ALPHABET_SIZE) + base;
        } else {
            plaintext[i] = ciphertext[i]; 
        }
    }
    plaintext[i] = '\0';
}

void calculateFrequency(char *text, double freq[ALPHABET_SIZE]) {
    int count[ALPHABET_SIZE] = {0};
    int total_letters = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            count[toupper(text[i]) - 'A']++;
            total_letters++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (total_letters == 0) ? 0 : (count[i] * 100.0) / total_letters;
    }
}

double computeScore(double freq[ALPHABET_SIZE]) {
    double score = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += (freq[i] - english_freq[i]) * (freq[i] - english_freq[i]);
    }
    return score;
}

typedef struct {
    int shift;
    double score;
    char plaintext[MAX_TEXT_LEN];
} DecryptionResult;

int compare(const void *a, const void *b) {
    return ((DecryptionResult*)a)->score > ((DecryptionResult*)b)->score ? 1 : -1;
}

void frequencyAttack(char *ciphertext, int topN) {
    DecryptionResult results[ALPHABET_SIZE];
    double freq[ALPHABET_SIZE];

    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        decryptCaesar(ciphertext, shift, results[shift].plaintext);
        calculateFrequency(results[shift].plaintext, freq);
        results[shift].score = computeScore(freq);
        results[shift].shift = shift;
    }

    qsort(results, ALPHABET_SIZE, sizeof(DecryptionResult), compare);

    printf("\nTop %d most likely plaintexts:\n", topN);
    for (int i = 0; i < topN && i < ALPHABET_SIZE; i++) {
        printf("Shift: %2d | Score: %.2f | Plaintext: %s\n", results[i].shift, results[i].score, results[i].plaintext);
    }
}

int main() {
    char ciphertext[MAX_TEXT_LEN];
    int topN;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_TEXT_LEN, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("Enter number of top probable plaintexts to display: ");
    scanf("%d", &topN);

    frequencyAttack(ciphertext, topN);

    return 0;
}