#include "memory.h"
#include <iostream>

int main() {

    Node *symbolTable[HASH_TABLE_SIZE];
    char name1[64];
    name1[0] = 't';
    name1[1] = 'x';
    name1[2] = 0;

    char name2[64];
    name2[0] = 'm';
    name2[1] = 'y';
    name2[2] = 0;

    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        symbolTable[i] = NULL;

    // Variable var;
    // strcpy(var.name1, name1);
    // var.block = 0;
    // insert(symbolTable, var);
    // // insert(symbolTable, var);

    // Variable *v = find(symbolTable, name1);
    // printf("%p\n", v);

    // for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    //     std::cout << i << " -> " << symbolTable[i] << std::endl;


    const size_t space = 200;
    Node *freeList = (Node *)malloc(sizeof(Node));
    freeList->next = NULL;
    freeList->block.addr = 0;
    freeList->block.refs = 0;
    freeList->block.size = space;

    // tx = alloc(17);
    freeList = alloc(freeList, symbolTable, name1, 17);
    freeList = alloc(freeList, symbolTable, name2, 30);
    dump(freeList, symbolTable);
    
    



    

    return 0;
}