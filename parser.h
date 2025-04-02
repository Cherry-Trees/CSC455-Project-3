#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

void prog(Token *token, const char *is, size_t *index);
void slist(Token *token, const char *is, size_t *index);
void stmt(Token *token, const char *is, size_t *index);
void rhs(Token *token, const char *is, size_t *index);

#endif