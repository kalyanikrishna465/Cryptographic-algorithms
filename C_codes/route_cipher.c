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

void route_cipher(char *text, const char *keyword, int encrypt) {
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

    if (encrypt) {
        int index = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = prepared[index++];
            }
        }

        char result[200] = {0};
        index = 0;
        int top = 0, bottom = rows - 1, left = 0, right = cols - 1;

        while (top <= bottom && left <= right) {
            for (int j = left; j <= right; j++) {
                result[index++] = grid[top][j];
            }
            top++;

            for (int i = top; i <= bottom; i++) {
                result[index++] = grid[i][right];
            }
            right--;

            if (top <= bottom) {
                for (int j = right; j >= left; j--) {
                    result[index++] = grid[bottom][j];
                }
                bottom--;
            }

            if (left <= right) {
                for (int i = bottom; i >= top; i--) {
                    result[index++] = grid[i][left];
                }
                left++;
            }
        }
        result[index] = '\0';

        memset(text, 0, 100);
        strncpy(text, result, prepared_len);
        text[prepared_len] = '\0';
    } else {
        int pos[rows][cols];
        int index = 0;
        int top = 0, bottom = rows - 1, left = 0, right = cols - 1;

        while (top <= bottom && left <= right) {
            for (int j = left; j <= right; j++) {
                pos[top][j] = index++;
            }
            top++;
            for (int i = top; i <= bottom; i++) {
                pos[i][right] = index++;
            }
            right--;
            if (top <= bottom) {
                for (int j = right; j >= left; j--) {
                    pos[bottom][j] = index++;
                }
                bottom--;
            }
            if (left <= right) {
                for (int i = bottom; i >= top; i--) {
                    pos[i][left] = index++;
                }
                left++;
            }
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = ' ';
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (pos[i][j] < prepared_len) {
                    grid[i][j] = prepared[pos[i][j]];
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
        route_cipher(text, keyword, 1);
        if (strncmp(text, "Error", 5) != 0) {
            printf("Encrypted text: %s\n", text);
        } else {
            printf("%s\n", text);
        }
    } else if (choice == 'd' || choice == 'D') {
        route_cipher(text, keyword, 0);
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
