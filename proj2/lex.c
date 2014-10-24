#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defs.h"
#include "lex.h"
#include "dfa.h"

static char *filename;
static char *file;
static size_t len;
static char lexeme[32];
static unsigned line;
static unsigned lpos;
static unsigned curr;
static unsigned errs;

char *token_names[] = {
        "err_tok", "eof_tok", "id", "num_lit", "op_plus", "op_minus", "op_mul", "op_div", "op_mod", "op_pow", "op_not",
        "op_and", "op_or", "op_xor", "op_lsh", "op_rsh", "op_inc", "op_dec", "op_tilde", "assn_get", "assn_plus",
        "assn_minus", "assn_mul", "assn_div", "assn_mod", "assn_pow", "assn_and", "assn_or", "assn_xor", "assn_lsh",
        "assn_rsh", "log_neq", "log_and", "log_or", "log_lt", "log_gt", "log_lte", "log_gte", "log_eq", "rparen", "lparen",
        "semi", "qst_mark", "colon", "comma", "type_int", "type_double"
};

static char *keywords[] = {
        "int", "double"
};
static const unsigned num_kw = 2;

// returns the keyword that matches the string given or 'id' if no keywords match
token_t match_keyword(char *str) {
    unsigned i;
    for (i = 0; i < num_kw; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return rsv_type_int + i;
        }
    }

    return id;
}

// read a single character from the file, increment the current character pointer in the file and the line number if
// a new line is encountered
char read_char() {
    unsigned i;
    for (i = 0; file[curr + i] == '\n'; i++);
    curr += i;

    if (i > 0 && file[curr] != '\0') {
        line++;
        lpos = 0;
        for (i = curr; file[i] != '\n' && file[i] != '\0'; i++);

        if (file[i] == '\n') {
            file[i] = '\0';
            printf("%4d: %s\n", line, &file[curr]);
            file[i] = '\n';
        } else {
            printf("%4d: %s\n", line, &file[curr]);
        }
    }

    lpos++;
    return file[curr++];
}

void init_lex(char *file_name) {
    filename = file_name;
    file = null;
    len = 0;
    line = 1;
    lpos = 0;
    curr = 0;
    errs = 0;

    FILE *in = fopen(file_name, "r");
    if (in == null || getdelim(&file, &len, EOF, in) < 0) {
        fprintf(stderr, "Error reading file: %s\n", file_name);
        exit(1);
    }

    fclose(in);

    unsigned i;
    for (i = 0; file[i] != '\n' && file[i] != '\0'; i++);

    if (file[i] == '\n') {
        file[i] = '\0';
        printf("%4d: %s\n", line, file);
        file[i] = '\n';
    } else {
        printf("%4d: %s\n", line, file);
    }
}

token_t get_token() {
    char c;
    for (c = read_char(); isspace(c); c = read_char());

    if (c == '\0') {
        return eof_tok;
    }

    token_t t;
    unsigned len = (unsigned) dfa_start(&file[curr - 1], &t);

    memset(lexeme, 0, 32);
    unsigned l = 0;
    if (len > 31) {
        l = 31;
        t = err_tok;
    } else {
        l = len + 1;
        lexeme[l] = '\0';
    }
    memcpy(lexeme, &file[curr - 1], (size_t) l);

    if (t == err_tok) {
        char err[64];
        snprintf(err, 64, "Error at %d,%d: invalid character: %s\n", line, lpos, lexeme);
        error_message(err);
        errs++;
    } else if (t == id) {
        t = match_keyword(lexeme);
    }

    curr += len;
    lpos += len;
    return t;
}

char *get_lexeme() {
    return lexeme;
}

void error_message(char *msg) {
    fputs(msg, stderr);
}

void end_lex() {
    fprintf(stderr, "found %d errors in %s\n", errs, filename);
}
