/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025
 * 
 *  Memory implementation file.
 */

#include "memory.h"

// Compress the free list.
Node *compress(Node *freeList) {
    if (freeList && freeList->next) {
        // Check for adjacent nodes and merge them.
        for (Node *right = freeList->next, *left = freeList; right; left = right, right = right->next) {
            if (left->block.addr + left->block.size == right->block.addr) {
                left->block.size += right->block.size;
                left->next = right->next;
                free(right);
                return compress(freeList);
            }
        }
    }
    return freeList;
}

// Associate a variable with another variable.
Node *associate(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char nameLhs[ID_MAX_SIZE], char nameRhs[ID_MAX_SIZE]) {

    // Don't do anything if it's associating itself.
    if (strcmp(nameLhs, nameRhs) == 0)
        return freeList;

    // Find the block of memory that the rhs is referring to.
    Variable *varRhs = findST(symbolTable, nameRhs);
    if (varRhs) {

        // Increment it's reference count.
        varRhs->block->refs++;

        // Deallocate the lhs, copy rhs name, and point to rhs block.
        freeList = dealloc(freeList, symbolTable, nameLhs);
        Variable varLhs;
        strcpy(varLhs.name, nameLhs);
        varLhs.block = varRhs->block;

        // Insert lhs into the symbol table.
        insertST(symbolTable, varLhs);
    }
    return freeList;
}

// Output variables and free list.
void dump(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]) {

    // Output variables.
    printf("Variables:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        for (Node *node = symbolTable[i]; node; node = node->next)
            printf(
                "%s:%d(%d) [%d]\n", 
                node->var.name, 
                node->var.block->addr, 
                node->var.block->size, 
                node->var.block->refs
            );
        
    // Output free list.
    printf("Free List:\n");
    for (Node *node = freeList; node; node = node->next) {
        printf(
            "%d(%d) [%d], ",  
            node->block.addr, 
            node->block.size, 
            node->block.refs
        );
    }
    printf("\n=========================================\n");
}

// Sort free list by address.
Node *sortFL(Node *freeList) {

    // Convert the list into an array.
    int n = 0;
    for (Node *freeNode = freeList; freeNode; freeNode = freeNode->next)
        ++n;
    Block *aux = (Block *)calloc(n, sizeof(Block));
    n = 0;
    for (Node *curr = freeList, *prev = NULL; curr; prev = curr, curr = curr->next) {
        aux[n++] = curr->block;
        if (prev)
            free(prev);
    }
    freeList = NULL;

    // Sort the array.
    bool sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (int i = 0; i < n - 1; ++i) {
            if (aux[i].addr > aux[i + 1].addr) {
                Block temp = aux[i];
                aux[i] = aux[i + 1];
                aux[i + 1] = temp;
                sorted = 0;
            }
        }
    }

    // Convert the array back to a list.
    for (int i = n - 1; i >= 0; --i)
        freeList = pushFL(freeList, aux[i]);
    
    free(aux);
    return freeList;
}

// Allocate space for a variable.
Node *alloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE], const int size) {

    // Deallocate the variable (if it exists).
    freeList = dealloc(freeList, symbolTable, name);

    // Find available space on the heap using first fit.
    for (Node *freeNode = freeList; freeNode; freeNode = freeNode->next) {
        if (size <= freeNode->block.size) {        
            Variable var;
            strcpy(var.name, name);

            // Allocate a new block of memory.
            var.block = (Block *)malloc(sizeof(Block));
            var.block->addr = freeNode->block.addr;
            var.block->refs = 1;
            var.block->size = size;
            insertST(symbolTable, var);

            // Split off from free block.
            freeNode->block.addr += size;
            freeNode->block.size -= size;
            break;
        }
    }
    return freeList;
}

// Deallocate a variable.
Node *dealloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]) {

    // Check if the variable is in the symbol table.
    Variable *var = findST(symbolTable, name);
    if (var) {

        // Decrement reference count of the block the variable is pointing to.
        var->block->refs--;

        // If the block reached a reference count of 0, return it to the free list.
        if (!var->block->refs) {
            Block freeBlock;
            freeBlock.addr = var->block->addr;
            freeBlock.refs = 0;
            freeBlock.size = var->block->size;

            freeList = pushFL(freeList, freeBlock);
            freeList = sortFL(freeList);
            free(var->block);
        }

        // Remove the variable from the symbol table.
        removeST(symbolTable, name);

    }
    return freeList;
}

// Hash name of variable and compress to fit hash table.
int indexName(char name[ID_MAX_SIZE]) {
    unsigned long hash = 5381;
    int c;
    while (c = *name++)
        hash = ((hash << 5) + hash) + c;

    return hash % HASH_TABLE_SIZE;
}

// Find a variable in the symbol table by name.
Variable *findST(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]) {
    Node *front = symbolTable[indexName(name)];
    for (Node *varNode = front; varNode; varNode = varNode->next) 
        if (strcmp(name, varNode->var.name) == 0)
            return &varNode->var;
    
    return NULL;
}

// Insert a variable into the symbol table.
void insertST(Node *symbolTable[HASH_TABLE_SIZE], Variable var) {
    Node **front = &symbolTable[indexName(var.name)],
        *newNode = (Node *)malloc(sizeof(Node));
    
    newNode->var = var;
    if (!*front) {
        newNode->next = NULL;
    }
    else {
        newNode->next = *front;
    }
    *front = newNode;
}

// Remove a variable from the symbol table by name.
void removeST(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]) {
    Node **front = &symbolTable[indexName(name)];
    for (Node *curr = *front, *prev = NULL; curr; prev = curr, curr = curr->next) {
        if (strcmp(name, curr->var.name) == 0) {
            if (prev) 
                prev->next = curr->next;
            else
                *front = curr->next;
            free(curr);
        }
    }
}

// Push block to the free list.
Node *pushFL(Node *freeList, Block block) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->block = block;
    if (!freeList)
        newNode->next = NULL;
    else
        newNode->next = freeList;
    return newNode;
}

// Clear symbol table.
void clearST(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) 
        for (Node *curr = symbolTable[i], *prev = NULL; curr; prev = curr, curr = curr->next) 
            if (prev) {
                dealloc(freeList, symbolTable, prev->var.name);
                free(prev);
            }
}

// Clear free list.
void clearFL(Node *freeList) {
    for (Node *curr = freeList, *prev = NULL; curr; prev = curr, curr = curr->next) 
        if (prev)
            free(prev);
}
