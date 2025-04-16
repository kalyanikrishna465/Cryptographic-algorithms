#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_valid_keyword(const char *keyword) {
    for (int i = 0; keyword[i] != '\0'; i++) {
        if (!isalpha(keyword[i])) {
            return 0;
        }
    }
    return 1;
}

void prepare_text(char *text, char *prepared, int *prepared_len, int cols) {
    int len = strlen(text);
    int index = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            prepared[index++] = toupper(text[i]);
        }
    }

    int rows = (index + cols - 1) / cols;
    while (index < rows * cols) {
        prepared[index++] = 'X';
    }
    prepared[index] = '\0';
    *prepared_len = index;
}

void assign_ranks(const char *keyword, int *ranks, int cols) {
    char sorted[100];
    strcpy(sorted, keyword);
    for (int i = 0; i < cols; i++) {
        sorted[i] = toupper(sorted[i]);
    }

    int rank = 1;
    char prev = '\0';
    for (char c = 'A'; c <= 'Z'; c++) {
        for (int i = 0; i < cols; i++) {
            if (sorted[i] == c) {
                if (c == prev) {
                    ranks[i] = rank - 1;
                } else {
                    ranks[i] = rank;
                    rank++;
                }
                prev = c;
            }
        }
    }
}

void myszkowski_cipher(char *text, const char *keyword, int encrypt) {
    int cols = strlen(keyword);
    if (cols < 2) {
        strcpy(text, "Error: Keyword too short");
        return;
    }

    char prepared[200] = {0};
    int prepared_len;
    prepare_text(text, prepared, &prepared_len, cols);

    int rows = prepared_len / cols;
    char grid[20][20];
    int ranks[100];

    assign_ranks(keyword, ranks, cols);

    if (encrypt) {
        int index = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = prepared[index++];
            }
        }

        char result[200] = {0};
        index = 0;
        for (int r = 1; r <= cols; r++) {
            for (int j = 0; j < cols; j++) {
                if (ranks[j] == r) {
                    for (int i = 0; i < rows; i++) {
                        result[index++] = grid[i][j];
                    }
                }
            }
        }
        result[index] = '\0';

        memset(text, 0, 100);
        strncpy(text, result, prepared_len);
        text[prepared_len] = '\0';
    } else {
        int col_len[cols];
        for (int j = 0; j < cols; j++) {
            col_len[j] = rows;
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = ' ';
            }
        }

        int index = 0;
        for (int r = 1; r <= cols; r++) {
            for (int j = 0; j < cols; j++) {
                if (ranks[j] == r) {
                    for (int i = 0; i < rows; i++) {
                        if (index < prepared_len) {
                            grid[i][j] = prepared[index++];
                        }
                    }
                }
            }
        }

        char result[200] = {0};
        index = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] != ' ') {
                    result[index++] = grid[i][j];
                }
            }
        }
        result[index] = '\0';

        memset(text, 0, 100);
        strncpy(text, result, prepared_len);
        text[prepared_len] = '\0';
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
    if (strlen(keyword) < 2) {
        printf("Error: Keyword too short\n");
        return 1;
    }

    printf("Encrypt (e) or Decrypt (d)? ");
    scanf(" %c", &choice);

    if (choice == 'e' || choice == 'E') {
        myszkowski_cipher(text, keyword, 1);
        if (strncmp(text, "Error", 5) != 0) {
            printf("Encrypted text: %s\n", text);
        } else {
            printf("%s\n", text);
        }
    } else if (choice == 'd' || choice == 'D') {
        myszkowski_cipher(text, keyword, 0);
        if (strncmp(text, "Error", 5) != 0) {
            printf("Decrypted text: %s\n", text);
        } else {
            printf("%s\n", text);
        }
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
