/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025
 * 
 *  Lexer header file.
 */

#ifndef LEXER_H
#define LEXER_H

#include "def.h"

// Token types.
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

// Structure holding token value, length, line number, and type.
typedef struct {
    char value[ID_MAX_SIZE];
    int length, lineno;
    TokenType type;
} Token;

void initDFA(int dfa[NUM_STATES][256]);
int getToken(Token *token, const char *is, int index);

#endif
