#include <stdio.h>
#include "parser_gen.h"
#include "array.h"

static FILE *rules;
static char *fstring;
static int fstr_len = 0;
static FILE *parser;

static array *terminals;
static int t_len = 0;
static array *nonterminals;
static int nt_len = 0;

typedef struct {
    char *nterm;
    array *firsts;
    array *follows;
    array *rules;
} prod_rules;
static array *productions;

inline bool generate(char *in, char *out) {
    if ((rules = fopen(in, "r")) == 0 || getdelim(&fstring, &fstr_len, EOF, rules) <= 0) {
        fprintf(stderr, "Error reading input file: %s", in);
        return false;
    }

    if ((parser = fopen(out, "w")) == 0) {
        fprintf(stderr, "Could not open output file for writing: %s", out);
        return false;
    }
}
