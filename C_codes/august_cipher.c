#include <stdio.h>
#include <string.h>
#include <ctype.h>

void august_cipher(char *text, int encrypt) {
    int shift = encrypt ? 1 : -1; 
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = base + (text[i] - base + shift + 26) % 26;
        }
    }
}

int main() {
    char text[100];
    char choice;

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; 

    printf("Encrypt (e) or Decrypt (d)? ");
    scanf(" %c", &choice);

    if (choice == 'e' || choice == 'E') {
        august_cipher(text, 1);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 'd' || choice == 'D') {
        august_cipher(text, 0);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}