/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025 
 * 
 *  Memory header file.
 *  Defines structs and functions necessary for memory management.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "def.h"

// Structure that holds starting address, size, and reference count.
typedef struct {
    int addr, size, refs;
} Block;

// Structure that holds a string and block pointer.
typedef struct {
    char name[ID_MAX_SIZE]; // Name of variable.
    Block *block;           // Block of memory the variable is referring to.
} Variable;

// Structure representing a singly list node that holds either a block or variable.
typedef struct Node Node;
struct Node {
    union {
        Block block;
        Variable var;
    };
    Node *next;
};

// Memory management operations.
Node *compress(Node *freeList);
Node *associate(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char nameLhs[ID_MAX_SIZE], char nameRhs[ID_MAX_SIZE]);
void dump(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]);
Node *alloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE], const int size);
Node *dealloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]);

// Data structure operations.
int indexName(char name[ID_MAX_SIZE]);
Variable *findST(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]);
void insertST(Node *symbolTable[HASH_TABLE_SIZE], Variable var);
void removeST(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]);
Node *sortFL(Node *freeList);
Node *pushFL(Node *freeList, Block block);
void clearST(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]);
void clearFL(Node *freeList);

#endif
