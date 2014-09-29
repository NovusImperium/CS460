#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer_dfa.h"
#include "array.h"


array *dfa_start(char *line, int r) {
    array *tokens = arr_init(32);
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (isspace(line[i])) {
            continue;
        } else {
            token *t = malloc(sizeof(token));
            memset(t, 0, sizeof(token));
            i += (dfa_trans[line[i]])(&line[i], t);

            if (t->err) {
                t->r = r;
                t->c = i;
            }

            arr_push(tokens, t);
        }
    }

    return tokens;
}

int dfa_invalid(char *str, token *t) {
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
            t->err = invalid_char_in_num;
            for (i; isalpha(str[i]) || str[i] == '_'; i++);
            break;
        } else {
            break;
        }
    }

    size_t len = i;
    if (i > 31) {
        len = 31;
        t->err = !t->err ? invalid_num_length : t->err;
    }
    memcpy(t->str, str, len);
    t->lex = flt ? literal_float : literal_int;

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

int dfa_div(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = assn_div;
        return 1;
    } else {
        t->lex = arith_div;
        return 0;
    }
}

int dfa_less(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = logic_le;
        return 1;
    } else if (str[1] == '<' && str[2] == '=') {
        t->lex = assn_lsh;
        return 2;
    } else if (str[1] == '<') {
        t->lex = bit_lsh;
        return 1;
    } else {
        t->lex = logic_lt;
        return 0;
    }
}

int dfa_greater(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = logic_ge;
        return 1;
    } else if (str[1] == '>' && str[2] == '=') {
        t->lex = assn_rsh;
        return 2;
    } else if (str[1] == '>') {
        t->lex = bit_rsh;
        return 1;
    } else {
        t->lex = logic_gt;
        return 0;
    }
}

int dfa_equal(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = logic_eq;
        return 1;
    } else {
        t->lex = assn_get;
        return 0;
    }
}

int dfa_carret(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = assn_xor;
        return 1;
    } else {
        t->lex = bit_xor;
        return 0;
    }
}

int dfa_pipe(char *str, token *t) {
    if (str[1] == '=') {
        t->lex = assn_or;
        return 1;
    } else if (str[1] == '|') {
        t->lex = logic_or;
        return 1;
    } else {
        t->lex = bit_or;
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

int dfa_comma(char *str, token *t) {
    t->lex = series_op;
    return 0;
}

int dfa_qmark(char *str, token *t) {
    t->lex = tern_cond;
    return 0;
}

int dfa_colon(char *str, token *t) {
    t->lex = tern_else;
    return 0;
}

int dfa_semi(char *str, token *t) {
    t->lex = semi_colon;
    return 0;
}

int dfa_tilde(char *str, token *t) {
    t->lex = bit_ones;
    return 0;
}

