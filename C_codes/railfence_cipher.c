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

int get_num_rails(const char *keyword) {
    int sum = 0;
    for (int i = 0; keyword[i] != '\0'; i++) {
        sum += toupper(keyword[i]) - 'A' + 1;
    }
    int rails = sum % 10;
    return rails < 2 ? 2 : rails; 
}

void prepare_text(char *text, char *prepared, int *prepared_len) {
    int len = strlen(text);
    int index = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            prepared[index++] = toupper(text[i]);
        }
    }
    prepared[index] = '\0';
    *prepared_len = index;
}

void rail_fence_cipher(char *text, const char *keyword, int encrypt) {
    char prepared[200] = {0};
    int prepared_len;
    prepare_text(text, prepared, &prepared_len);

    int num_rails = get_num_rails(keyword);
    if (num_rails > prepared_len) {
        strcpy(text, "Error: Too many rails for text length");
        return;
    }

    char result[200] = {0};

    if (encrypt) {
        int rail_len[num_rails];
        memset(rail_len, 0, sizeof(rail_len));
        char rails[num_rails][200];

        for (int i = 0; i < num_rails; i++) {
            memset(rails[i], 0, sizeof(rails[i]));
        }

        int row = 0;
        int dir = 1; // 1 for down, -1 for up
        for (int i = 0; i < prepared_len; i++) {
            rails[row][rail_len[row]++] = prepared[i];
            row += dir;
            if (row == num_rails - 1) dir = -1;
            if (row == 0) dir = 1;
        }

        int index = 0;
        for (int r = 0; r < num_rails; r++) {
            for (int j = 0; j < rail_len[r]; j++) {
                result[index++] = rails[r][j];
            }
        }
        result[index] = '\0';
    } else {
        int rail_len[num_rails];
        memset(rail_len, 0, sizeof(rail_len));

        int row = 0;
        int dir = 1;
        for (int i = 0; i < prepared_len; i++) {
            rail_len[row]++;
            row += dir;
            if (row == num_rails - 1) dir = -1;
            if (row == 0) dir = 1;
        }

        char rails[num_rails][200];
        for (int i = 0; i < num_rails; i++) {
            memset(rails[i], 0, sizeof(rails[i]));
        }

        int index = 0;
        for (int r = 0; r < num_rails; r++) {
            for (int j = 0; j < rail_len[r]; j++) {
                rails[r][j] = prepared[index++];
            }
        }

        row = 0;
        dir = 1;
        index = 0;
        int pos[num_rails];
        memset(pos, 0, sizeof(pos));
        for (int i = 0; i < prepared_len; i++) {
            result[index++] = rails[row][pos[row]++];
            row += dir;
            if (row == num_rails - 1) dir = -1;
            if (row == 0) dir = 1;
        }
        result[index] = '\0';
    }

    memset(text, 0, 100);
    strncpy(text, result, prepared_len);
    text[prepared_len] = '\0';
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
        rail_fence_cipher(text, keyword, 1);
        if (strncmp(text, "Error", 5) != 0) {
            printf("Encrypted text: %s\n", text);
        } else {
            printf("%s\n", text);
        }
    } else if (choice == 'd' || choice == 'D') {
        rail_fence_cipher(text, keyword, 0);
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