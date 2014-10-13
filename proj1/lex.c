#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "array.h"

char * token_names[] = {
        "KEYWORD",
        "IDENT",
        "LIT_INT",
        "LIT_FLOAT",
        "LIT_STR",
        "LIT_CHAR",
        "ARITH_ADD",
        "ARITH_SUB",
        "ARITH_MUL",
        "ARITH_DIV",
        "ARITH_MOD",
        "ARITH_INC",
        "ARITH_DEC",
        "ASSN_GET",
        "ASSN_ADD",
        "ASSN_SUB",
        "ASSN_MUL",
        "ASSN_DIV",
        "ASSN_MOD",
        "ASSN_LSH",
        "ASSN_RSH",
        "ASSN_AND",
        "ASSN_XOR",
        "ASSN_OR",
        "LOGIC_AND",
        "LOGIC_NOT",
        "LOGIC_OR",
        "LOGIC_NE",
        "LOGIC_EQ",
        "LOGIC_GT",
        "LOGIC_GE",
        "LOGIC_LT",
        "LOGIC_LE",
        "BIT_AND",
        "BIT_OR",
        "BIT_XOR",
        "BIT_LSH",
        "BIT_RSH",
        "BIT_ONES",
        "OPEN_PAREN",
        "CLOSE_PAREN",
        "SEMI_COLON",
        "TERN_COND",
        "TERN_ELSE",
        "SERIES_OP",
        "INVALID_LEX",
};

struct token_struct {
    array *ts;
    unsigned curr;
    token t;
} static tokens;

struct error_struct {
    array *errs;
} static errors;

void init_lex(char *filename) {
    FILE *in = fopen(filename, "r");
    char *file = null;
    size_t s = 0;
    getdelim(&file, &s, EOF, in);

    tokens.ts = dfa_start(file);
    tokens.curr = 0;

    optional opt = arr_init(32);
    if (opt.e) {
        errors.errs = opt.val;
    } else {
        exit(1);
    }
}

lexical_t get_token() {
    optional opt = arr_get(tokens.ts, tokens.curr);
    if (opt.e) {
        tokens.curr++;
        tokens.t = **((token**)opt.val);
        if (tokens.t.err != no_err) {
            token *t = malloc(sizeof(token));
            memcpy(t, *((token**)opt.val), sizeof(token));
            arr_push(errors.errs, t);
        }
        return tokens.t.lex;
    } else {
        return invalid_lex;
    }
}

char *get_lexeme() {
    return tokens.t.str;
}

void error_message(char *msg) {
    printf("%s", msg);
}

void end_lex() {
    printf("Errors found: %d\n", arr_size(errors.errs));
    char *fmt = "%s:%s, err:%s at %d:%d\n";
    optional opt = arr_pop(errors.errs);
    for (opt; opt.e; opt = arr_pop(errors.errs)) {
        token t = *((token*)opt.val);
        char *str = malloc(64);
        sprintf(str, fmt, token_names[t.lex], t.str, err_str[t.err], t.r, t.c);
        error_message(str);
        free(str);
    }
}
