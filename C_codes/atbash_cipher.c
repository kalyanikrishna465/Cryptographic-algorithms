#include <stdio.h>
#include <string.h>
#include <ctype.h>

void atbash_cipher(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = base + (25 - (text[i] - base));
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

    if (choice == 'e' || choice == 'E' || choice == 'd' || choice == 'D') {
        atbash_cipher(text);
        printf("%s text: %s\n", 
               (choice == 'e' || choice == 'E') ? "Encrypted" : "Decrypted", 
               text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}