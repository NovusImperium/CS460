#include <stdio.h>
#include "parser_gen.h"
#include "array.h"
#include "set.h"

static FILE *rules;
static char *fstring;
static size_t fstr_len = 0;
static size_t f_curr = 0;
static FILE *parser;

static array *terminals;
static int t_len = 0;
static array *nonterminals;
static int nt_len = 0;

static char *output;

typedef struct {
    char *nterm;
    set *firsts;
    set *follows;
    array *rules;
} prod_rules;
static array *productions;

// reads in the terminal and nonterminals symbols
inline bool read_syms();

// read in the production rules
inline bool read_rules();

// convert the production rules from c string to ints
inline bool convert_rules();

inline bool generate(char *in, char *out) {
    if ((rules = fopen(in, "r")) == 0 || getdelim(&fstring, &fstr_len, EOF, rules) <= 0) {
        fprintf(stderr, "Error reading input file: %s", in);
        return false;
    }

    if ((parser = fopen(out, "w")) == 0) {
        fprintf(stderr, "Could not open output file for writing: %s", out);
        return false;
    }

    return true;
}

inline bool read_syms() {

    return false;
}

inline bool read_rules() {

    return false;
}

inline bool convert_rules() {

    return false;
}
