/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025
 * 
 *  Parser header file.
 *  Defines functions representing nonterminals in the grammar.
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "memory.h"

// Nonterminals.
void prog(const char *is, int size);
void slist(Token *token, const char *is, int *index, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]);
Node *stmt(Token *token, const char *is, int *index, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]);
Node *rhs(Token *token, const char *is, int *index, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char nameLhs[ID_MAX_SIZE]);

// Throw error.
void error(const char *msg, const int lineno, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]);

#endif
