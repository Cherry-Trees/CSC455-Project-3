#ifndef LEXER_H
#define LEXER_H

#define NUM_STATES 20
// #define NUM_KEYWORDS sizeof(keywords) / sizeof(char *)
#define ID_MAX_SIZE 64

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    ERROR,
    ID, 
    NUM_INT, 
    ASSIGNOP, 
    LPAREN, 
    RPAREN, 
    SEMICOLON,
    EOF_TOK
} TokenType;

typedef struct {
    char value[ID_MAX_SIZE];
    size_t length, lineno;
    TokenType type;
} Token;

void initDFA(int dfa[NUM_STATES][256]);
size_t getToken(Token *token, const char *is, size_t index);

bool isFreeKeyword(char s[ID_MAX_SIZE]);
bool isDumpKeyword(char s[ID_MAX_SIZE]);
bool isCompressKeyword(char s[ID_MAX_SIZE]);
bool isAllocKeyword(char s[ID_MAX_SIZE]);

#endif