#include "lexer.h"

void prog(Token *token, const char *is, size_t *index) {
    *index = getToken(token, is, *index);
    slist(token, is, index);
}
void slist(Token *token, const char *is, size_t *index) {
    if (token->type == ID) {
        stmt(token, is, index);
        if (token->type != SEMICOLON)
            exit(1);
        *index = getToken(token, is, *index);
        slist(token, is, index);
    }
}
void stmt(Token *token, const char *is, size_t *index) {
    if (isFreeKeyword(token->value)) {
        *index = getToken(token, is, index);
        if (token->type != LPAREN)
            exit(1);
        *index = getToken(token, is, index);
        if (token->type != ID)
            exit(1);
        *index = getToken(token, is, index);
        if (token->type != RPAREN)
            exit(1);
        *index = getToken(token, is, index); 
    }

    else if (isDumpKeyword(token->value)) {
        *index = getToken(token, is, index);
        if (token->type != LPAREN)
            exit(1);
        *index = getToken(token, is, index);
        if (token->type != RPAREN)
            exit(1);
        *index = getToken(token, is, index); 
    }
}
void rhs(Token *token, const char *is, size_t index);