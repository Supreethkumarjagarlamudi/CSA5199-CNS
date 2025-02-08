#include <stdio.h>
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int e, int phi) {
    int t = 0, new_t = 1;
    int r = phi, new_r = e;
    
    while (new_r != 0) {
        int quotient = r / new_r;
        
        int temp = new_t;
        new_t = t - quotient * new_t;
        t = temp;
        
        temp = new_r;
        new_r = r - quotient * new_r;
        r = temp;
    }
    
    if (t < 0) {
        t += phi;
    }
    return t;
}

int main() {
    int n = 3599;
    int e = 31;

    int p = 59, q = 61; 

    int phi_n = (p - 1) * (q - 1);

    int d = mod_inverse(e, phi_n);

    printf("RSA Public Key: (e, n) = (%d, %d)\n", e, n);
    printf("RSA Private Key: d = %d\n", d);
    printf("Verification: (d * e) mod φ(n) = (%d * %d) mod %d = %d\n", d, e, phi_n, (d * e) % phi_n);

    return 0;
}