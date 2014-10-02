#include <stdio.h>
#include "defs.h"
#include "lexer_dfa.h"
#include "array.h"

int main(int argc, char **argv) {
    FILE *in = fopen(argv[1], "r");

    size_t len = 0;
    char *line = null;
    getdelim(&line, &len, EOF, in);

    // arr_print(dfa_start(line, 0), stdout);

    return 0;
}
