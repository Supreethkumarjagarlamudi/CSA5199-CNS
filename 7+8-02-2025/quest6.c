#include <stdio.h>
#include <stdlib.h>

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    long long q = 23;
    long long a = 5;  

    long long x_A = 6;
    long long A = mod_exp(a, x_A, q); 

    long long x_B = 15;  
    long long B = mod_exp(a, x_B, q); 

    long long K_Alice = mod_exp(B, x_A, q);
    long long K_Bob = mod_exp(A, x_B, q);

    printf("Publicly shared prime q = %lld\n", q);
    printf("Publicly shared base a = %lld\n", a);
    
    printf("Alice's Private Key (x_A) = %lld\n", x_A);
    printf("Alice Sends (A = a^x_A mod q) = %lld\n", A);
    
    printf("Bob's Private Key (x_B) = %lld\n", x_B);
    printf("Bob Sends (B = a^x_B mod q) = %lld\n", B);

    printf("Shared Key Computed by Alice = %lld\n", K_Alice);
    printf("Shared Key Computed by Bob = %lld\n", K_Bob);

    if (K_Alice == K_Bob) {
        printf("Key Exchange Successful! Shared Secret Key = %lld\n", K_Alice);
    } else {
        printf("Key Exchange Failed!\n");
    }

    return 0;
}