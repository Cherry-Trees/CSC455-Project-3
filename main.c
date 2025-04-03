/**
 *  Author: Jamie Miles
 *  Date:   4/1/2025
 * 
 *  Driver code.
 */

#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[]) {

    // Heap size and file name.
    int size;
    char fname[FNAME_MAX_SIZE];

    // Prompt for heap size and input file name.
    printf("Please enter the initial freelist (heap) size: ");
    scanf("%d", &size);
    printf("Please enter the name of an input file: ");
    scanf("%s", fname);

    FILE *file = fopen(fname, "r"); // Pointer to file.
    char rbuf[RBUF_SIZE];           // Read buffer.
    char *input = NULL;             // Input code.
    
    if (file) {
        
        // Read file into buffer.
        while(fgets(rbuf, RBUF_SIZE, file)) {
            
            // Append buffer to end of input.
            char *prev = input;
            int inputSize = strlen(rbuf) + 1;
            if (input)
                inputSize += strlen(input);
            input = malloc(inputSize);
            input[0] = '\0';
            if (prev) {
                strcat(input, prev);
                free(prev);
            }
            strcat(input, rbuf);
        }

        // Close file.
        fclose(file);
    }

    // Run the interpreter.
    if (input) 
        prog(input, size);

    return 0;
}
