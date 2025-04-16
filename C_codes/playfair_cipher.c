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

void generate_key_square(const char *keyword, char square[5][5]) {
    char key[26] = {0};
    int used[26] = {0};
    int key_index = 0;

    for (int i = 0; keyword[i] != '\0'; i++) {
        char c = toupper(keyword[i]);
        if (c == 'J') c = 'I';
        if (isalpha(c) && !used[c - 'A']) {
            key[key_index++] = c;
            used[c - 'A'] = 1;
        }
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        if (!used[c - 'A']) {
            key[key_index++] = c;
            used[c - 'A'] = 1;
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            square[i][j] = key[i * 5 + j];
        }
    }
}

void find_position(char square[5][5], char c, int *row, int *col) {
    if (c == 'J') c = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (square[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

int prepare_text(char *text, char *prepared, int *prepared_len) {
    int len = strlen(text);
    int index = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            prepared[index++] = toupper(text[i]) == 'J' ? 'I' : toupper(text[i]);
        }
    }
    prepared[index] = '\0';
    char temp[200] = {0};
    index = 0;
    int is_encryption = 0;
    if (strchr(text, 'X') == NULL) { 
        is_encryption = 1;
    }

    if (is_encryption) {
        for (int i = 0; i < strlen(prepared); i++) {
            temp[index++] = prepared[i];
            if (i < strlen(prepared) - 1 && prepared[i] == prepared[i + 1]) {
                temp[index++] = 'X';
            } else if (i == strlen(prepared) - 1 && (strlen(prepared) % 2) == 1) {
                temp[index++] = 'X';
            }
        }
        temp[index] = '\0';
        strcpy(prepared, temp);
    } else {
        if (strlen(prepared) % 2 != 0) {
            return 0; 
        }
    }

    *prepared_len = strlen(prepared);
    return 1; 
}

int playfair_cipher(char *text, const char *keyword, int encrypt) {
    char square[5][5];
    char prepared[200] = {0};
    int prepared_len;

    generate_key_square(keyword, square);

    if (!prepare_text(text, prepared, &prepared_len)) {
        return 0; 
    }

    for (int i = 0; i < prepared_len; i += 2) {
        int row1, col1, row2, col2;
        find_position(square, prepared[i], &row1, &col1);
        find_position(square, prepared[i + 1], &row2, &col2);

        if (row1 == row2) {
            prepared[i] = square[row1][encrypt ? (col1 + 1) % 5 : (col1 - 1 + 5) % 5];
            prepared[i + 1] = square[row2][encrypt ? (col2 + 1) % 5 : (col2 - 1 + 5) % 5];
        } else if (col1 == col2) {
            prepared[i] = square[encrypt ? (row1 + 1) % 5 : (row1 - 1 + 5) % 5][col1];
            prepared[i + 1] = square[encrypt ? (row2 + 1) % 5 : (row2 - 1 + 5) % 5][col2];
        } else {
            prepared[i] = square[row1][col2];
            prepared[i + 1] = square[row2][col1];
        }
    }

    memset(text, 0, 100);
    strncpy(text, prepared, prepared_len);
    text[prepared_len] = '\0'; 
    return 1;
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
        if (playfair_cipher(text, keyword, 1)) {
            printf("Encrypted text: %s\n", text);
        } else {
            printf("Error: Invalid input for encryption\n");
        }
    } else if (choice == 'd' || choice == 'D') {
        if (playfair_cipher(text, keyword, 0)) {
            printf("Decrypted text: %s\n", text);
        } else {
            printf("Error: Input must have even number of letters for decryption\n");
        }
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}