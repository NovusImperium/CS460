#include <stdio.h>
#include <stdlib.h>
#include "SetLimits.h"
#include "lex.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    SetLimits();

    if (argc < 2) {
        printf("format: proj2 <filename>\n");
        exit(1);
    }

    init_lex(argv[1]);
    init_parser();

    token_t t = get_token();
    while (true) {
        switch (parse_token(t)) {
            case parser_err:
                dump_parser();
                printf("\nError reading token {%s : ' %s '}\n", token_names[t], get_lexeme());
                goto shutdown;
            case adv_token:
                t = get_token();
                break;
            case keep_token:
                break;
            case end_token:
                dump_parser();
                goto shutdown;
        }
    }

    shutdown:
    end_lex();
    shutdown_parser();
    return 0;
}
