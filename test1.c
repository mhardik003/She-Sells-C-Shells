#include <stdio.h>
#include <string.h>

void splitString(char *str) {
    char *token;
    char delimiters[] = ";&";
    char *lastDelimiter = NULL;

    // Use strtok() to split string based on delimiters
    token = strtok(str, delimiters);
    while (token != NULL) {
        if (lastDelimiter) {
            printf("Delimiter: '%c'\n", *lastDelimiter);
        }
        printf("Token: %s\n", token);
        
        // This moves the pointer to just after the last found token.
        // It allows us to check the next character which is the delimiter used.
        lastDelimiter = token + strlen(token);

        // Get next token
        token = strtok(NULL, delimiters);
    }
}

int main() {
    char inputStr[256];
    printf("Enter the string: ");
    fgets(inputStr, sizeof(inputStr), stdin);

    // Remove newline character
    size_t len = strlen(inputStr);
    if (len > 0 && inputStr[len - 1] == '\n') {
        inputStr[len - 1] = '\0';
    }

    splitString(inputStr);

    return 0;
}
