#include <stdio.h>
#include <stdlib.h>
#include "SetLimits.h"
#include "lex.h"

int main(int argc, char *argv[]) {
    SetLimits();
    token_t t;

    if (argc < 2) {
        printf("format: proj2 <filename>\n");
        exit(1);
    }

    init_lex(argv[1]);
    while ((t = get_token()) != EOFT)
        printf("\t%-12s%s\n", token_names[t], get_lexeme());
    end_lex();
    return 0;
}
