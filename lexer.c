/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025
 * 
 *  Lexer implementation file.
 */

#include "lexer.h"

static bool initedDFA = 0;
static int dfa[NUM_STATES][256];
static int currentLineno = 1;

// Initialize DFA.
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

    dfa[0]['='] = 3;                                     // 0 -> 3
    dfa[0]['('] = 4;                                    // 0 -> 4
    dfa[0][')'] = 5;                                    // 0 -> 5
    dfa[0][';'] = 6;                                    // 0 -> 6

    initedDFA = 1;
}

int getToken(Token *token, const char *is, int index) {
    
    // Initialize token attributes.
    token->length = 0;
    token->lineno = currentLineno;
    token->type = ERROR;
    token->value[0] = 0;
    
    if (!is || !token)
        return -1;
    
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

    // Initialize the DFA if not already.
    if (!initedDFA)
        initDFA(dfa);

    // Traverse the DFA to get token.
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
        token->type = (TokenType)prev_state;
    else
        token->type = EOF_TOK;

    // Return index of next token.
    return index;
}
