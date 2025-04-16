#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_keyword(const char *keyword) {
    for (int i = 0; keyword[i] != '\0'; i++) {
        if (!isalpha(keyword[i])) {
            return 0;
        }
    }
    return 1;
}

void autokey_cipher(char *text, const char *keyword, int encrypt) {
    int text_len = strlen(text);
    int key_len = strlen(keyword);
    char keystream[100] = {0}; 
    int alpha_count = 0; 

    for (int i = 0; i < key_len && alpha_count < text_len; i++) {
        if (alpha_count < text_len && isalpha(text[alpha_count])) {
            keystream[alpha_count] = tolower(keyword[i]);
            alpha_count++;
        }
    }

    if (encrypt) {
        for (int i = 0; alpha_count < text_len && i < text_len; i++) {
            if (isalpha(text[i])) {
                keystream[alpha_count] = tolower(text[i]);
                alpha_count++;
            }
        }
    }

    alpha_count = 0; 
    for (int i = 0; i < text_len; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int shift = keystream[alpha_count] - 'a';

            if (encrypt) {
                text[i] = base + ((text[i] - base + shift) % 26);
            } else {
                int new_pos = (text[i] - base - shift + 26) % 26;
                text[i] = base + new_pos;
                if (alpha_count < text_len - 1) {
                    keystream[alpha_count + 1] = tolower(text[i]);
                }
            }

            alpha_count++;
        }
    }
}

int main() {
    char text[100];
    char keyword[100];
    char choice;

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; 

    printf("Enter keyword (letters only): ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0'; 

    if (!is_valid_keyword(keyword)) {
        printf("Error: Keyword must contain only letters\n");
        return 1;
    }
    if (strlen(keyword) == 0) {
        printf("Error: Keyword cannot be empty\n");
        return 1;
    }

    printf("Encrypt (e) or Decrypt (d)? ");
    scanf(" %c", &choice);

    if (choice == 'e' || choice == 'E') {
        autokey_cipher(text, keyword, 1);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 'd' || choice == 'D') {
        autokey_cipher(text, keyword, 0);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}