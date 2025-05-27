/*
Recursive Descent Parser with LTD Replacement (280)
Assignment: Programming Languages and Structures (CSC 461)
Semester: Spring 2025
Student Name: Md. Latifur Rahman
Student ID: [Your Full ID Here]
Course Instructor: Md Nazir Ahmed, Lecturer, Dept. of CSE, IUBAT
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LTD_VALUE 280

// --------------------- Token Types -------------------------
typedef enum {
    T_IF, T_ELSE, T_WHILE,
    T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,
    T_SEMICOLON, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_EQ, T_NEQ, T_LT, T_GT, T_LE, T_GE,
    T_NUMBER, T_IDENTIFIER, T_LTD,
    T_EOF, T_INVALID
} TokenType;

// --------------------- Token Struct ------------------------
typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

// --------------------- Globals -----------------------------
char input[1000];
int pos = 0;
char current_char;
Token current_token;

// --------------------- Lexer Helpers -----------------------
void advance() { current_char = input[pos++]; }
void skip_whitespace() { while (isspace(current_char)) advance(); }
int is_letter(char c) { return isalpha(c); }
int is_digit(char c) { return isdigit(c); }

Token get_next_token() {
    skip_whitespace();
    Token token;
    token.lexeme[0] = '\0';

    if (current_char == '\0') {
        token.type = T_EOF;
        return token;
    }

    // Symbols
    switch (current_char) {
        case '{': token.type = T_LBRACE; strcpy(token.lexeme, "{"); advance(); return token;
        case '}': token.type = T_RBRACE; strcpy(token.lexeme, "}"); advance(); return token;
        case '(': token.type = T_LPAREN; strcpy(token.lexeme, "("); advance(); return token;
        case ')': token.type = T_RPAREN; strcpy(token.lexeme, ")"); advance(); return token;
        case ';': token.type = T_SEMICOLON; strcpy(token.lexeme, ";"); advance(); return token;
        case '+': token.type = T_PLUS; strcpy(token.lexeme, "+"); advance(); return token;
        case '-': token.type = T_MINUS; strcpy(token.lexeme, "-"); advance(); return token;
        case '*': token.type = T_MUL; strcpy(token.lexeme, "*"); advance(); return token;
        case '/': token.type = T_DIV; strcpy(token.lexeme, "/"); advance(); return token;
    }

    // Relational operators
    if (current_char == '=') {
        advance();
        if (current_char == '=') { advance(); token.type = T_EQ; strcpy(token.lexeme, "=="); return token; }
    } else if (current_char == '!') {
        advance();
        if (current_char == '=') { advance(); token.type = T_NEQ; strcpy(token.lexeme, "!="); return token; }
    } else if (current_char == '<') {
        advance();
        if (current_char == '=') { advance(); token.type = T_LE; strcpy(token.lexeme, "<="); return token; }
        else { token.type = T_LT; strcpy(token.lexeme, "<"); return token; }
    } else if (current_char == '>') {
        advance();
        if (current_char == '=') { advance(); token.type = T_GE; strcpy(token.lexeme, ">="); return token; }
        else { token.type = T_GT; strcpy(token.lexeme, ">"); return token; }
    }

    // Numbers
    if (is_digit(current_char)) {
        int i = 0;
        while (is_digit(current_char)) token.lexeme[i++] = current_char, advance();
        token.lexeme[i] = '\0'; token.type = T_NUMBER; return token;
    }

    // Identifiers or Keywords
    if (is_letter(current_char)) {
        int i = 0;
        while (is_letter(current_char) || is_digit(current_char)) token.lexeme[i++] = current_char, advance();
        token.lexeme[i] = '\0';
        if (strcmp(token.lexeme, "if") == 0) token.type = T_IF;
        else if (strcmp(token.lexeme, "else") == 0) token.type = T_ELSE;
        else if (strcmp(token.lexeme, "while") == 0) token.type = T_WHILE;
        else if (strcmp(token.lexeme, "LTD") == 0) token.type = T_LTD;
        else token.type = T_IDENTIFIER;
        return token;
    }

    // Invalid token
    token.type = T_INVALID; token.lexeme[0] = current_char; token.lexeme[1] = '\0'; advance(); return token;
}

// --------------------- Parser Helpers ----------------------
void match(TokenType expected) {
    if (current_token.type == expected) current_token = get_next_token();
    else { printf("Syntax Error: Expected token %d, got %d (%s)\n", expected, current_token.type, current_token.lexeme); exit(1); }
}

void expression();
void factor();
void term();

void factor() {
    if (current_token.type == T_NUMBER) {
        printf("[FACTOR: %s]\n", current_token.lexeme);
        match(T_NUMBER);
    } else if (current_token.type == T_LTD) {
        printf("[FACTOR: LTD -> %d]\n", LTD_VALUE);
        match(T_LTD);
    } else if (current_token.type == T_IDENTIFIER) {
        printf("[FACTOR: %s]\n", current_token.lexeme);
        match(T_IDENTIFIER);
    } else if (current_token.type == T_LPAREN) {
        match(T_LPAREN);
        expression();
        match(T_RPAREN);
    } else {
        printf("Syntax Error: Invalid factor '%s'\n", current_token.lexeme);
        exit(1);
    }
}

void term() {
    factor();
    while (current_token.type == T_MUL || current_token.type == T_DIV) {
        Token op = current_token;
        match(current_token.type);
        factor();
        printf("[TERM OP: %s]\n", op.lexeme);
    }
}

void expression() {
    term();
    while (current_token.type == T_PLUS || current_token.type == T_MINUS) {
        Token op = current_token;
        match(current_token.type);
        term();
        printf("[EXPR OP: %s]\n", op.lexeme);
    }
}

void condition() {
    expression();
    if (current_token.type >= T_EQ && current_token.type <= T_GE) {
        printf("[COND OP: %s]\n", current_token.lexeme);
        match(current_token.type);
        expression();
    } else {
        printf("Syntax Error: Expected relational operator, got '%s'\n", current_token.lexeme);
        exit(1);
    }
}

void statement();

void block() {
    match(T_LBRACE);
    while (current_token.type != T_RBRACE && current_token.type != T_EOF) {
        statement();
    }
    match(T_RBRACE);
}

void statement() {
    if (current_token.type == T_IF) {
        match(T_IF); match(T_LPAREN);
        condition();
        match(T_RPAREN);
        block();
        if (current_token.type == T_ELSE) {
            match(T_ELSE);
            block();
        }
    } else if (current_token.type == T_WHILE) {
        match(T_WHILE); match(T_LPAREN);
        condition();
        match(T_RPAREN);
        block();
    } else {
        expression();
        match(T_SEMICOLON);
    }
}

void program() {
    block();
    if (current_token.type != T_EOF) {
        printf("Syntax Error: Extra input after program end.\n");
        exit(1);
    } else {
        printf("Program parsed successfully.\n");
    }
}

// --------------------- Main Function ------------------------
int main() {
    printf("Enter program code below (end with ~ on a new line):\n");
    char line[200];
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '~') break;
        strcat(input, line);
    }
    pos = 0;
    advance();
    current_token = get_next_token();
    program();
    return 0;
}
