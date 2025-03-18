#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Function to compute (base^exp) % mod using modular exponentiation
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) {  // If exp is odd, multiply base with result
            result = (result * base) % mod;
        }
        exp = exp >> 1;  // Divide exp by 2
        base = (base * base) % mod;  // Square the base
    }
    return result;
}

int main() {
    srand(time(NULL));

    // Public values: base (a) and prime modulus (q)
    uint64_t a = 5;     // Public base
    uint64_t q = 23;    // Prime modulus (small prime for demonstration)

    // Private keys for Alice and Bob
    uint64_t x_Alice = rand() % (q - 1) + 1;  // Secret key of Alice
    uint64_t x_Bob = rand() % (q - 1) + 1;    // Secret key of Bob

    // Public values sent by Alice and Bob
    uint64_t A = mod_exp(a, x_Alice, q);  // Alice computes a^x_Alice mod q
    uint64_t B = mod_exp(a, x_Bob, q);    // Bob computes a^x_Bob mod q

    // Compute shared secret key
    uint64_t key_Alice = mod_exp(B, x_Alice, q);  // Alice computes B^x_Alice mod q
    uint64_t key_Bob = mod_exp(A, x_Bob, q);      // Bob computes A^x_Bob mod q

    printf("Publicly known values: Base (a) = %llu, Prime modulus (q) = %llu\n", a, q);
    printf("Alice's secret key (x_Alice) = %llu\n", x_Alice);
    printf("Bob's secret key (x_Bob) = %llu\n", x_Bob);
    printf("Alice sends: %llu\n", A);
    printf("Bob sends: %llu\n", B);
    printf("Shared secret key (computed by Alice) = %llu\n", key_Alice);
    printf("Shared secret key (computed by Bob) = %llu\n", key_Bob);

    return 0;
}