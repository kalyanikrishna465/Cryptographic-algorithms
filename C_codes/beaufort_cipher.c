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

void beaufort_cipher(char *text, const char *keyword) {
    int text_len = strlen(text);
    int key_len = strlen(keyword);
    int key_index = 0;

    for (int i = 0; i < text_len; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            char key_char = tolower(keyword[key_index % key_len]);
            int key_pos = key_char - 'a';
            int text_pos = text[i] - base;
            
            text[i] = base + ((key_pos - text_pos + 26) % 26);
            
            key_index++; 
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

    if (choice == 'e' || choice == 'E' || choice == 'd' || choice == 'D') {
        beaufort_cipher(text, keyword);
        printf("%s text: %s\n", 
               (choice == 'e' || choice == 'E') ? "Encrypted" : "Decrypted", 
               text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}