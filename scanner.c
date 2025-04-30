#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 40

typedef enum {
    IF, ELSE, INT, RETURN, VOID, WHILE,
    ID, NUM,
    PLUS, MINUS, TIMES, OVER,
    LT, LTE, GT, GTE, EQ, NEQ, ASSIGN,
    SEMI, COMMA, LPAREN, RPAREN, LBRACK, RBRACK, LBRACE, RBRACE,
    ENDFILE, ERROR
} TokenType;

const char* tokenNames[] = {
    "IF", "ELSE", "INT", "RETURN", "VOID", "WHILE",
    "ID", "NUM",
    "PLUS", "MINUS", "TIMES", "OVER",
    "LT", "LTE", "GT", "GTE", "EQ", "NEQ", "ASSIGN",
    "SEMI", "COMMA", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE",
    "ENDFILE", "ERROR"
};

char tokenString[MAX_TOKEN_LENGTH + 1];
int lineno = 1;

TokenType reservedLookup(char* s) {
    if (strcmp(s, "if") == 0) return IF;
    if (strcmp(s, "else") == 0) return ELSE;
    if (strcmp(s, "int") == 0) return INT;
    if (strcmp(s, "return") == 0) return RETURN;
    if (strcmp(s, "void") == 0) return VOID;
    if (strcmp(s, "while") == 0) return WHILE;
    return ID;
}

TokenType getToken(FILE* source) {
    int c;
    int tokenIndex = 0;

    while ((c = fgetc(source)) != EOF) {
        // Debugging output: show every character read
        printf("Read character: '%c' (ASCII: %d)\n", c, c);

        if (isspace(c)) {
            if (c == '\n') {
                lineno++;
            }
            continue;
        }

        if (isalpha(c)) {
            tokenString[tokenIndex++] = c;
            while (isalnum(c = fgetc(source))) {
                if (tokenIndex < MAX_TOKEN_LENGTH)
                    tokenString[tokenIndex++] = c;
            }
            tokenString[tokenIndex] = '\0';
            ungetc(c, source); // Put the last character back
            return reservedLookup(tokenString);
        }

        if (isdigit(c)) {
            tokenString[tokenIndex++] = c;
            while (isdigit(c = fgetc(source))) {
                if (tokenIndex < MAX_TOKEN_LENGTH)
                    tokenString[tokenIndex++] = c;
            }
            tokenString[tokenIndex] = '\0';
            ungetc(c, source); // Put the last character back
            return NUM;
        }

        switch (c) {
            case '+': return PLUS;
            case '-': return MINUS;
            case '*': return TIMES;
            case '/':
                if ((c = fgetc(source)) == '*') {
                    int prev = 0;
                    while ((c = fgetc(source)) != EOF) {
                        if (c == '\n') lineno++;
                        if (prev == '*' && c == '/') break;
                        prev = c;
                    }
                    continue;
                } else {
                    ungetc(c, source);
                    return OVER;
                }
            case '<':
                if ((c = fgetc(source)) == '=') return LTE;
                ungetc(c, source); return LT;
            case '>':
                if ((c = fgetc(source)) == '=') return GTE;
                ungetc(c, source); return GT;
            case '=':
                if ((c = fgetc(source)) == '=') return EQ;
                ungetc(c, source); return ASSIGN;
            case '!':
                if ((c = fgetc(source)) == '=') return NEQ;
                ungetc(c, source); return ERROR;
            case ';': return SEMI;
            case ',': return COMMA;
            case '(': return LPAREN;
            case ')': return RPAREN;
            case '[': return LBRACK;
            case ']': return RBRACK;
            case '{': return LBRACE;
            case '}': return RBRACE;
            default:
                return ERROR;
        }
    }

    return ENDFILE;
}

int main() {
    FILE* source = fopen("input.txt", "r");
    if (!source) {
        perror("Error opening file 'input.txt'");
        printf("Press Enter to exit...\n");
        getchar();
        return 1;
    }

    printf("File opened successfully!\n");

    int c;
    while ((c = fgetc(source)) != EOF) {
        printf("Read character: '%c' (ASCII: %d)\n", c, c);
    }

    fclose(source);
    printf("Done reading.\n");
    getchar(); // so the console window stays open
    return 0;
}
