#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer_dfa.h"

array *dfa_start(char *line) {
    array *tokens = arr_init(32);
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] < '!') {
            continue;
        } else if (line[i] > '~') {
            token *t = malloc(sizeof(token));
            t->lex = invalid_lex;
            memset(t->str, 0, 32);
            t->c = i;
            t->err = invalid_char;
            arr_push(tokens, t);
        } else {
            token *t = malloc(sizeof(token));
            memset(t, 0, sizeof(token));
            i += (dfa_trans[line[i] - '!'])(&line[i], t);

            if (t->err) {
                t->c = i;
            }

            arr_push(tokens, t);
        }
    }

    return tokens;
}

int dfa_invchar(char *str, token *t) {
    t->lex = invalid_lex;
    memset(t->str, 0, 32);
    t->err = invalid_char;

    return 0;
}

int dfa_word(char *str, token *t) {
    size_t i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            break;
        }
    }

    size_t len = i;
    if (i > 31) {
        len = 31;
        t->err = invalid_id_length;
    }
    memcpy(t->str, str, len);
    // TODO: determine if the word string matches a keyword
    t->lex = ident;

    return (int) i - 1;
}

int dfa_num(char *str, token *t) {
    size_t i;
    bool flt = false;
    for (i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            continue;
        } else if (str[i] == '.') {
            if (flt) {
                break;
            } else {
                flt = true;
            }
        } else if (isalpha(str[i]) || str[i] == '_') {
            t->err = invalid_char;
            break;
        }
    }

    size_t len = i;
    if (i > 31) {
        len = 31;
        t->err = !t->err ? invalid_id_length : t->err;
    }
    memcpy(t->str, str, len);
    t->lex = flt ? num_float : num_int;

    return (int) i - 1;
}

int dfa_not(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = logic_ne;
        return 1;
    } else {
        t->lex = logic_not;
        return 0;
    }
}

int dfa_mod(char *str, token *t) { 
    if (str[1] == '=') {
        t->lex = assn_mod;
        return 1;
    } else {
        t->lex = arith_mod;
        return 0;
    }
}

int dfa_and(char *str, token *t) { 
    if (str[1] == '=') {
        t->lex = assn_and;
        return 1;
    } else if (str[1] == '&') {
        t->lex = logic_and;
        return 1;
    } else {
        t->lex = bit_and;
        return 0;
    }
}

int dfa_mul(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = assn_mul;
        return 1;
    } else {
        t->lex = arith_mul;
        return 0;
    }
}

int dfa_add(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = assn_add;
        return 1;
    } else if (str[1] == '+') {
        t->lex = arith_inc;
        return 1;
    } else {
        t->lex = arith_add;
        return 0;
    }
}

int dfa_sub(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = assn_sub;
        return 1;
    } else if (str[1] == '-') {
        t->lex = arith_dec;
        return 1;
    } else {
        t->lex = arith_sub;
        return 0;
    }
}

int dfa_oparen(char *str, token *t) {
    t->lex = open_paren;
    return 0;
}

int dfa_cparen(char *str, token *t) {
    t->lex = close_paren;
    return 0;
}

int dfa_series(char *str, token *t) {
    t->lex = series_op;
    return 0;
}

