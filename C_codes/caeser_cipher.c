#include <stdio.h>
#include <string.h>
#include <ctype.h>

void caesar_cipher(char *text, int shift, int encrypt) {
    shift = encrypt ? shift : -shift;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = base + (text[i] - base + shift + 26) % 26;
        }
    }
}

int main() {
    char text[100];
    int shift;
    char choice;

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; 

    printf("Enter shift value: ");
    scanf("%d", &shift);
    shift = shift % 26; 

    printf("Encrypt (e) or Decrypt (d)? ");
    scanf(" %c", &choice);

    if (choice == 'e' || choice == 'E') {
        caesar_cipher(text, shift, 1);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 'd' || choice == 'D') {
        caesar_cipher(text, shift, 0);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
