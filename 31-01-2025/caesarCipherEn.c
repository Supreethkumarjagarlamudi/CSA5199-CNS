#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encrypt(char text[], int shift){
    for(int i = 0; text[i] != '\0'; ++i){
        char ch = text[i];
        if(isupper(ch)){
            text[i] = (ch - 'A' + shift) % 26 + 'A';
        }
        else if(islower(ch)){
            text[i] = (ch - 'a' + shift) % 26 + 'a';
        }
    }

    printf("Encrypted message: %s\n", text);
}

int main(){
    char text[100];
    int shift;

    printf("Enter a message for encryption: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter shift number: ");
    scanf("%d", &shift);

    encrypt(text, shift);
    
}