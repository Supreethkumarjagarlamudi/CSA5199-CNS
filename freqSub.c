#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT 1000
void countFrequency(const char *text, int freq[26]) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            freq[toupper(text[i]) - 'A']++;
        }
    }
}

void sortFrequencies(int freq[26], char letters[26]) {
    for (int i = 0; i < 25; i++) {
        for (int j = i + 1; j < 26; j++) {
            if (freq[i] < freq[j]) { 
                int tempFreq = freq[i];
                freq[i] = freq[j];
                freq[j] = tempFreq;

                char tempLetter = letters[i];
                letters[i] = letters[j];
                letters[j] = tempLetter;
            }
        }
    }
}

void substituteLetters(char *text, char cipherLetter, char plainLetter) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (toupper(text[i]) == cipherLetter) {
            text[i] = isupper(text[i]) ? plainLetter : tolower(plainLetter);
        }
    }
}

int main() {
    char ciphertext[MAX_TEXT];
    int freq[26] = {0};
    char letters[26];

    for (int i = 0; i < 26; i++) {
        letters[i] = 'A' + i;
    }

    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_TEXT, stdin);

    countFrequency(ciphertext, freq);

    sortFrequencies(freq, letters);

    printf("\nLetter Frequency Analysis:\n");
    for (int i = 0; i < 10; i++) {
        if (freq[i] > 0) {
            printf("%c: %d\n", letters[i], freq[i]);
        }
    }

    printf("\nPossible substitutions:\n");
    printf("Most frequent letter ('%c') might be 'E'.\n", letters[0]);
    printf("Look for words resembling 'THE' to find 'T' and 'H'.\n");

    char cipherLetter, plainLetter;
    while (1) {
        printf("\nEnter cipher letter to replace (or '0' to stop): ");
        scanf(" %c", &cipherLetter);
        if (cipherLetter == '0') break;
        printf("Enter corresponding plain letter: ");
        scanf(" %c", &plainLetter);

        substituteLetters(ciphertext, toupper(cipherLetter), toupper(plainLetter));

        printf("\nUpdated text: %s\n", ciphertext);
    }

    printf("\nFinal Deciphered Message:\n%s\n", ciphertext);
    return 0;
}