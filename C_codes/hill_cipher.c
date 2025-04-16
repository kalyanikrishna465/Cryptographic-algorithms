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

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int mod_inverse(int a) {
    a = a % 26;
    if (a < 0) a += 26;
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1) {
            return x;
        }
    }
    return -1; 
}

int generate_key_matrix(const char *keyword, int matrix[2][2]) {
    char key[4] = {0};
    int used[26] = {0};
    int key_index = 0;

    for (int i = 0; keyword[i] != '\0' && key_index < 4; i++) {
        char c = toupper(keyword[i]);
        if (isalpha(c) && !used[c - 'A']) {
            key[key_index++] = c;
            used[c - 'A'] = 1;
        }
    }

    for (char c = 'A'; c <= 'Z' && key_index < 4; c++) {
        if (!used[c - 'A']) {
            key[key_index++] = c;
            used[c - 'A'] = 1;
        }
    }

    int det = -1;
    int attempts = 0;
    char temp_key[4];
    memcpy(temp_key, key, 4);

    while (det == -1 && attempts < 26 * 26) {
        matrix[0][0] = temp_key[0] - 'A';
        matrix[0][1] = temp_key[1] - 'A';
        matrix[1][0] = temp_key[2] - 'A';
        matrix[1][1] = temp_key[3] - 'A';

        det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % 26;
        if (det < 0) det += 26;

        if (gcd(det, 26) == 1 && mod_inverse(det) != -1) {
            return 1; 
        }

        temp_key[3]++;
        if (temp_key[3] > 'Z') {
            temp_key[3] = 'A';
            temp_key[2]++;
        }
        if (temp_key[2] > 'Z') {
            temp_key[2] = 'A';
            temp_key[1]++;
        }
        if (temp_key[1] > 'Z') {
            break; 
        }
        attempts++;
    }

    matrix[0][0] = 3;
    matrix[0][1] = 2;
    matrix[1][0] = 5;
    matrix[1][1] = 7; 
    return 1;
}

void inverse_matrix(int matrix[2][2], int inverse[2][2]) {
    int det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % 26;
    if (det < 0) det += 26;
    int det_inv = mod_inverse(det);

    inverse[0][0] = (matrix[1][1] * det_inv) % 26;
    inverse[0][1] = ((-matrix[0][1]) * det_inv) % 26;
    inverse[1][0] = ((-matrix[1][0]) * det_inv) % 26;
    inverse[1][1] = (matrix[0][0] * det_inv) % 26;

    if (inverse[0][1] < 0) inverse[0][1] += 26;
    if (inverse[1][0] < 0) inverse[1][0] += 26;
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

    if (strlen(prepared) % 2 == 1) {
        prepared[index++] = 'X';
        prepared[index] = '\0';
    }

    *prepared_len = strlen(prepared);
}

void hill_cipher(char *text, const char *keyword, int encrypt) {
    int key_matrix[2][2];
    int prepared_len;
    char prepared[200] = {0};

    if (!generate_key_matrix(keyword, key_matrix)) {
        strcpy(text, "Error: Non-invertible key matrix");
        return;
    }

    prepare_text(text, prepared, &prepared_len);

    int inverse[2][2];
    if (!encrypt) {
        inverse_matrix(key_matrix, inverse);
    }

    for (int i = 0; i < prepared_len; i += 2) {
        int x = prepared[i] - 'A';
        int y = prepared[i + 1] - 'A';
        int result[2];

        if (encrypt) {
            result[0] = (key_matrix[0][0] * x + key_matrix[0][1] * y) % 26;
            result[1] = (key_matrix[1][0] * x + key_matrix[1][1] * y) % 26;
        } else {
            result[0] = (inverse[0][0] * x + inverse[0][1] * y) % 26;
            result[1] = (inverse[1][0] * x + inverse[1][1] * y) % 26;
        }

        prepared[i] = 'A' + result[0];
        prepared[i + 1] = 'A' + result[1];
    }

    memset(text, 0, 100);
    strncpy(text, prepared, prepared_len);
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
        hill_cipher(text, keyword, 1);
        if (strncmp(text, "Error", 5) != 0) {
            printf("Encrypted text: %s\n", text);
        } else {
            printf("%s\n", text);
        }
    } else if (choice == 'd' || choice == 'D') {
        hill_cipher(text, keyword, 0);
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