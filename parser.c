/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025
 * 
 *  Parser implementation file.
 */

#include "parser.h"

void prog(const char *is, int size) {
    // Construct the free list with one node.
    Node *freeList = (Node *)malloc(sizeof(Node));
    freeList->block.addr = 0;
    freeList->block.refs = 0;
    freeList->block.size = size;
    freeList->next = NULL;

    // Construct the symbol table.
    Node *symbolTable[HASH_TABLE_SIZE];
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        symbolTable[i] = NULL;

    // Get the first token.
    Token token;
    int index = 0;
    index = getToken(&token, is, index);
    slist(&token, is, &index, freeList, symbolTable);

    // Clear the symbol table and free list after program ends.
    clearST(freeList, symbolTable);
    clearFL(freeList);
}

void slist(Token *token, const char *is, int *index, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]) {
    if (token->type == ID) {
        freeList = stmt(token, is, index, freeList, symbolTable);   // Evaluate stmt.
        if (token->type != SEMICOLON)   // Check for semicolon after stmt.
            error("Missing semicolon.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index);
        slist(token, is, index, freeList, symbolTable); // Keep parsing stmt's.
    }
}

Node *stmt(Token *token, const char *is, int *index, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]) {
    if (!strcmp(token->value, "free")) {    // Free call.
        *index = getToken(token, is, *index);
        if (token->type != LPAREN)  // Check for lparen.
            error("Missing opening parenthesis in free call.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index);
        if (token->type != ID)      // Check for ID in free argument.
            error("Missing argument in free call.", token->lineno, freeList, symbolTable);

        freeList = dealloc(freeList, symbolTable, token->value);    // Deallocate variable.
        *index = getToken(token, is, *index);
        if (token->type != RPAREN)  // Check for rparen.
            error("Missing closing parenthesis in free call.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index); 
    }

    else if (!strcmp(token->value, "dump")) {   // Dump call.
        *index = getToken(token, is, *index);
        if (token->type != LPAREN)  // Check for lparen.
            error("Missing opening parenthesis in dump call.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index);
        if (token->type != RPAREN)  // Check for rparen.
            error("Missing closing parenthesis in dump call.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index); 
        dump(freeList, symbolTable);    // Dump.
    }

    else if (!strcmp(token->value, "compress")) {   // Compress call.
        *index = getToken(token, is, *index);
        if (token->type != LPAREN)  // Check for lparen.
            error("Missing opening parenthesis in compress call.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index);
        if (token->type != RPAREN)  // Check for rparen.
            error("Missing closing parenthesis in compress call.", token->lineno, freeList, symbolTable);
        *index = getToken(token, is, *index); 
        freeList = compress(freeList);  // Compress.
    }

    else {  // Alloc call/Association
        char nameLhs[ID_MAX_SIZE];
        strcpy(nameLhs, token->value);  // Store the lhs name for later.

        *index = getToken(token, is, *index);
        if (token->type != ASSIGNOP)    // Check for assign op.
            error("Missing assign operator in variable association.", token->lineno, freeList, symbolTable);
        
        *index = getToken(token, is, *index);
        freeList = rhs(token, is, index, freeList, symbolTable, nameLhs);   // Evaluate rhs, pass lhs name.
    }
    return freeList;
}
Node *rhs(Token *token, const char *is, int *index, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char nameLhs[ID_MAX_SIZE]) {
    if (token->type != ID)  // Check for ID.
        error("Missing RHS.", token->lineno, freeList, symbolTable);

    char name[ID_MAX_SIZE];
    strcpy(name, token->value); // Store rhs name for later.
    *index = getToken(token, is, *index);
    if (!strcmp(name, "alloc")) {   // Alloc call.
        if (token->type != LPAREN)  // Check for lparen.
            error("Missing opening parenthesis in alloc call.", token->lineno, freeList, symbolTable);

        *index = getToken(token, is, *index);
        if (token->type != NUM_INT) // Check for integer literal.
            error("Missing integer literal in alloc call.", token->lineno, freeList, symbolTable);
        
        freeList = alloc(freeList, symbolTable, nameLhs, atoi(token->value));   // Allocate space for variable.

        *index = getToken(token, is, *index);
        if (token->type != RPAREN)  // Check for rparen.
            error("Missing closing parenthesis in alloc call.", token->lineno, freeList, symbolTable); 

        *index = getToken(token, is, *index);
    }
    else    // Association.
        associate(freeList, symbolTable, nameLhs, name);    // Associate lhs with rhs.

    return freeList;
}

void error(const char *msg, const int lineno, Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]) {

    // Output error message.
    printf("Syntax error at line %d\n", lineno);
    printf(msg);
    putchar('\n');

    // Clear symbol table and free list.
    clearST(freeList, symbolTable);
    clearFL(freeList);

    // Terminate program.
    exit(1);
}
