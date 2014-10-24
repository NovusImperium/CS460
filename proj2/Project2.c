#include <stdio.h>
#include <stdlib.h>
#include "SetLimits.h"
#include "lex.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    SetLimits();
    token_t t;

    if (argc < 2) {
        printf("format: proj2 <filename>\n");
        exit(1);
    }

    init_lex(argv[1]);
    init_parser();

    t = get_token();
    while (true) {
        switch (parse_token(t)) {
            case parser_err:
                fputs("Error reading token\n", stderr);
                goto shutdown;
            case adv_token:
                t = get_token();
                break;
            case keep_token:
                break;
            case end_token:
                goto shutdown;
        }
    }

    shutdown:
    end_lex();
    shutdown_parser();
    return 0;
}
