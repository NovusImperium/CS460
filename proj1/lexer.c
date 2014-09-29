#include <stdio.h>
#include "defs.h"
#include "lexer_dfa.h"
#include "array.h"

int main(int argc, char **argv) {
    FILE *in = fopen(argv[1], "r");

    char *line = null;
    size_t len = 1023;
    getline(&line, &len, in);

    arr_print(dfa_start(line, 0), stdout);

    return 0;
}
