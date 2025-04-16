#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_key(const char *key) {
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isdigit(key[i])) {
            return 0;
        }
    }
    return 1;
}

void gronsfeld_cipher(char *text, const char *key, int encrypt) {
    int text_len = strlen(text);
    int key_len = strlen(key);
    int key_index = 0;

    for (int i = 0; i < text_len; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int shift = key[key_index % key_len] - '0';
            
            if (encrypt) {
                text[i] = base + ((text[i] - base + shift) % 26);
            } else {
                text[i] = base + ((text[i] - base - shift + 26) % 26);
            }
            
            key_index++; 
        }
    }
}

int main() {
    char text[100];
    char key[100];
    char choice;

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; 

    printf("Enter key (digits only): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; 

    if (!is_valid_key(key)) {
        printf("Error: Key must contain only digits\n");
        return 1;
    }
    if (strlen(key) == 0) {
        printf("Error: Key cannot be empty\n");
        return 1;
    }

    printf("Encrypt (e) or Decrypt (d)? ");
    scanf(" %c", &choice);

    if (choice == 'e' || choice == 'E') {
        gronsfeld_cipher(text, key, 1);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 'd' || choice == 'D') {
        gronsfeld_cipher(text, key, 0);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}