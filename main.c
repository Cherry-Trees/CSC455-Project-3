#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[]) {

    const char *code = "x = 5;y=6 ; free(x);";
    Token token;
    size_t index = 0;
    while (token.type != EOF_TOK) {
        index = getToken(&token, code, index);
        printf("%d -> %s\n", token.type, token.value);
    }

    return 0;
}