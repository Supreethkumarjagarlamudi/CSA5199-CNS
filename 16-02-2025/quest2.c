#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) { 
            result = (result * base) % mod;
        }
        exp = exp >> 1; 
        base = (base * base) % mod; 
    }
    return result;
}

int main() {
    srand(time(NULL));

    uint64_t a = 5;     
    uint64_t q = 23;    

    uint64_t x_Alice = rand() % (q - 1) + 1; 
    uint64_t x_Bob = rand() % (q - 1) + 1;

    uint64_t A = mod_exp(a, x_Alice, q);  
    uint64_t B = mod_exp(a, x_Bob, q);    

    uint64_t key_Alice = mod_exp(B, x_Alice, q); 
    uint64_t key_Bob = mod_exp(A, x_Bob, q);      

    printf("Publicly known values: Base (a) = %llu, Prime modulus (q) = %llu\n", a, q);
    printf("Alice's secret key (x_Alice) = %llu\n", x_Alice);
    printf("Bob's secret key (x_Bob) = %llu\n", x_Bob);
    printf("Alice sends: %llu\n", A);
    printf("Bob sends: %llu\n", B);
    printf("Shared secret key (computed by Alice) = %llu\n", key_Alice);
    printf("Shared secret key (computed by Bob) = %llu\n", key_Bob);

    return 0;
}