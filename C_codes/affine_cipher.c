#include <stdio.h>
#include <string.h>
#include <ctype.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        if (m == 0) return -1; 
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (a > 1) return -1; 
    if (x1 < 0) x1 += m0;

    return x1;
}

void affine_cipher(char *text, int a, int b, int encrypt) {
    int a_inv = mod_inverse(a, 26); 

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int x = text[i] - base; 
            if (encrypt) {
                text[i] = base + ((a * x + b) % 26);
            } else {
                text[i] = base + (a_inv * (x - b + 26) % 26);
            }
        }
    }
}

int main() {
    char text[100];
    int a, b;
    char choice;

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; 

    printf("Enter a (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter b: ");
    scanf("%d", &b);
    b = b % 26; 

    if (gcd(a, 26) != 1) {
        printf("Error: a must be coprime with 26 (e.g., 1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25)\n");
        return 1;
    }

    printf("Encrypt (e) or Decrypt (d)? ");
    scanf(" %c", &choice);

    if (choice == 'e' || choice == 'E') {
        affine_cipher(text, a, b, 1);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 'd' || choice == 'D') {
        if (mod_inverse(a, 26) == -1) {
            printf("Error: No modular inverse exists for a=%d\n", a);
            return 1;
        }
        affine_cipher(text, a, b, 0);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}