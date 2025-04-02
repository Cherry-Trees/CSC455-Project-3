#include "lexer.h"
#include <stdio.h>

static bool initedDFA = 0;
static int dfa[NUM_STATES][256];
static size_t currentLineno = 1;

// static char *keywords[] = {
//     "int", 
//     "float", 
//     "while", 
//     "if", 
//     "then", 
//     "else", 
//     "void", 
//     "begin", 
//     "end" 
// };

void initDFA(int dfa[NUM_STATES][256]) {

    // Default every transition to -1.
    for (int i = 0; i < NUM_STATES; i++)
        for (int j = 0; j < 256; j++)
            dfa[i][j] = -1;

    for (char ch = 'A'; ch <= 'Z'; ch++) dfa[0][ch] = 1; // 0 -> 1
    for (char ch = 'a'; ch <= 'z'; ch++) dfa[0][ch] = 1;
    for (char ch = 'A'; ch <= 'Z'; ch++) dfa[1][ch] = 1; // 1 -> 1
    for (char ch = 'a'; ch <= 'z'; ch++) dfa[1][ch] = 1;
    for (char ch = '0'; ch <= '9'; ch++) dfa[1][ch] = 1;

    for (char ch = '0'; ch <= '9'; ch++) dfa[0][ch] = 2; // 0 -> 2
    for (char ch = '0'; ch <= '9'; ch++) dfa[2][ch] = 2; // 2 -> 2

    dfa[0]['='] = 3;                                     // 0 -> 9

    dfa[0]['('] = 4;                                    // 0 -> 10
    dfa[0][')'] = 5;                                    // 0 -> 11

    dfa[0][';'] = 6;                                    // 0 -> 16

    initedDFA = 1;
}

// TokenType getTokenType(int state) {
//     switch (state) {
//       case  0: return ERROR;
//       case  1: return ID;
//       case  2: return NUM_INT;
//       case  3: return ERROR;
//       case  9: return ASSIGNOP;
//       case 10: return LPAREN;
//       case 11: return RPAREN;
//       case 12: return ERROR;
//       case 14: return ERROR;
//       case 16: return SEMICOLON;
//       default: return EOF_TOK;
//     }
//   }
  
//   // Reserved array index -> Token type.
//   TokenType getKeywordTokenType(int index) {
//     switch (index) {
//       case  0: return FREE;
//       case  1: return DUMP;
//       case  2: return COMPRESS;
//       case  3: return ALLOC;
//       case  4: return THEN;
//       case  5: return ELSE;
//       case  6: return VOID;
//       case  7: return BEGIN;
//       case  8: return END;
  
//       default: return ERROR;
//     }
//   }
  













size_t getToken(Token *token, const char *is, size_t index) {
    
    token->length = 0;
    token->lineno = currentLineno;
    token->type = ERROR;
    token->value[0] = 0;
    
    if (!is || !token)
        return 0xFFFFFFFFFFFFFFFF;
    
    // Remove ws
    char ch = is[index];
    
    while (ch == ' ' || ch == '\t' || ch == '\n') {
        if (ch == '\n') 
            token->lineno = ++currentLineno;
        ch = is[++index];
    }
    
    ch = 0;
    int curr_state = 0;
    int prev_state = -1;
    if (!initedDFA)
        initDFA(dfa);
    while (curr_state != -1) {
        ch = is[index++];
        if (ch == '#') {
            while (ch != '\n') {
                ch = is[index++];
            }
            return getToken(token, is, index);
        }
        
        token->value[token->length++] = ch;
        prev_state = curr_state;
        curr_state = dfa[curr_state][(int)ch];
    }
    --index;
    token->value[--token->length] = 0;
    if (*token->value)
        token->type = prev_state;
    else
        token->type = EOF_TOK;

    return index;
}

bool isFreeKeyword(char s[ID_MAX_SIZE]) {
    return 
        s[0] == 'f' &&
        s[1] == 'r' &&
        s[2] == 'e' &&
        s[3] == 'e' &&
        s[4] == '\0';
}

bool isDumpKeyword(char s[ID_MAX_SIZE]) {
    return 
        s[0] == 'd' &&
        s[1] == 'u' &&
        s[2] == 'm' &&
        s[3] == 'p' &&
        s[4] == '\0';
}

bool isCompressKeyword(char s[ID_MAX_SIZE]) {
    return 
        s[0] == 'c' &&
        s[1] == 'o' &&
        s[2] == 'm' &&
        s[3] == 'p' &&
        s[4] == 'r' &&
        s[5] == 'e' &&
        s[6] == 's' &&
        s[7] == 's' &&
        s[8] == '\0';
}

bool isAllocKeyword(char s[ID_MAX_SIZE]) {
    return 
        s[0] == 'a' &&
        s[1] == 'l' &&
        s[2] == 'l' &&
        s[3] == 'o' &&
        s[4] == 'c' &&
        s[5] == '\0';
}