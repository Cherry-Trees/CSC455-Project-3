#ifndef MEMORY_H
#define MEMORY_H

#include "global.h"

typedef struct {
    size_t addr, size, refs;
} Block;

typedef struct {
    char name[ID_MAX_SIZE];
    Block *block;
} Variable;

typedef struct Node Node;
struct Node {
    // enum {
    //     BLOCK,
    //     VAR
    // } tag;
    union {
        Block block;
        Variable var;
    };
    Node *next;
};





Node *compress(Node *freeList);
Node *associate(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char nameLhs[ID_MAX_SIZE], char nameRhs[ID_MAX_SIZE]);
void dump(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]);
Node *sort(Node *freeList);
Node *alloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE], const size_t size);
Node *dealloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]);

size_t index(char name[ID_MAX_SIZE]);
Variable *find(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]);
void insert(Node *symbolTable[HASH_TABLE_SIZE], Variable var);
void remove(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]);
Node *push(Node *freeList, Block block);



#endif
