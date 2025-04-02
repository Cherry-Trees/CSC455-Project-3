#include "memory.h"

Node *compress(Node *freeList);
void dump(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE]) {
    printf("Variables:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        for (Node *node = symbolTable[i]; node; node = node->next)
            printf(
                "%s:%d(%d) [%d], ", 
                node->var.name, 
                node->var.block->addr, 
                node->var.block->size, 
                node->var.block->refs
            );
    
    printf("\nFree List:\n");
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

Node *sort(Node *freeList) {
    size_t n = 0;
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

    for (int i = n - 1; i >= 0; --i)
        freeList = push(freeList, aux[i]);
    
    free(aux);
    return freeList;
}

Node *alloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE], const size_t size) {
    freeList = dealloc(freeList, symbolTable, name);

    Variable var;
    strcpy(var.name, name);

    // First fit.
    // Assumed sorted by addr, sort here.
    for (Node *freeNode = freeList; freeNode; freeNode = freeNode->next) {
        if (size <= freeNode->block.size) {
            var.block = (Block *)malloc(sizeof(Block));
            var.block->addr = freeNode->block.addr;
            var.block->refs = 1;
            var.block->size = size;
            insert(symbolTable, var);

            freeNode->block.addr += size;
            freeNode->block.size -= size;
            break;
        }
    }
    return freeList;
}

Node *dealloc(Node *freeList, Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]) {
    Variable *var = find(symbolTable, name);
    if (var) {
        printf("FOUND\n");
        var->block->refs--;
        if (!var->block->refs) {
            Block freeBlock;
            freeBlock.addr = var->block->addr;
            freeBlock.refs = 0;
            freeBlock.size = var->block->size;

            freeList = push(freeList, freeBlock);
            freeList = sort(freeList);
            free(var->block);
            // sort.
        }
        remove(symbolTable, name);

    }
    return freeList;
}

size_t index(char name[ID_MAX_SIZE]) {
    unsigned long hash = 5381;
    int c;
    while (c = *name++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASH_TABLE_SIZE;
}

Variable *find(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]) {
    Node *front = symbolTable[index(name)];
    for (Node *varNode = front; varNode; varNode = varNode->next) 
        if (strcmp(name, varNode->var.name) == 0)
            return &varNode->var;
    
    return NULL;
}
void insert(Node *symbolTable[HASH_TABLE_SIZE], Variable var) {
    Node **front = &symbolTable[index(var.name)],
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

void remove(Node *symbolTable[HASH_TABLE_SIZE], char name[ID_MAX_SIZE]) {
    Node **front = &symbolTable[index(name)];
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


void associate(Node *symbolTable[HASH_TABLE_SIZE]);

Node *push(Node *freeList, Block block) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->block = block;
    if (!freeList) {
        newNode->next = NULL;
    }
    else {
        newNode->next = freeList;
    }
    return newNode;
}