#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer_dfa.h"
#include "array.h"


array *dfa_start(char *file) {
    array *tokens;
    optional opt = arr_init(32);
    if (!opt.e) {
        return null;
    } else {
        tokens = opt.val;
    }
    int i, c = 0, line = 1;
    for (i = 0; file[i] != '\0'; i++, c++) {
        if (file[i] == '\n') {
            line++;
            c = 0;
            continue;
        } else if (isspace(file[i])) {
            continue;
        } else {
            token *t = malloc(sizeof(token));
            memset(t, 0, sizeof(token));
            int len = (dfa_trans[file[i]])(&file[i], t);

            t->r = line;
            t->c = c;

            c += len;
            i += len;
            arr_push(tokens, t);
        }
    }

    token *t = malloc(sizeof(token));
    memset(t, 0, sizeof(token));
    t->lex = eof_tok;
    t->err = no_err;
    t->c = c;
    t->r = line;

    arr_push(tokens, t);

    return tokens;
}

int dfa_invalid(char *str, token *t) {
    t->lex = invalid_lex;
    t->str[0] = str[0];
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
    if (str[0] == '.' && !isdigit(str[1])) {
        t->str[0] = '.';
        t->lex = invalid_lex;
        t->err = invalid_char;
        return 0;
    }
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
        }  else {
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
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = logic_ne;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = logic_not;
        return 0;
    }
}

int dfa_mod(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_mod;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = arith_mod;
        return 0;
    }
}

int dfa_and(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_and;
        t->str[1] = str[1];
        return 1;
    } else if (str[1] == '&') {
        t->lex = logic_and;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = bit_and;
        return 0;
    }
}

int dfa_mul(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_mul;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = arith_mul;
        return 0;
    }
}

int dfa_add(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_add;
        t->str[1] = str[1];
        return 1;
    } else if (str[1] == '+') {
        t->lex = arith_inc;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = arith_add;
        return 0;
    }
}

int dfa_sub(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_sub;
        t->str[1] = str[1];
        return 1;
    } else if (str[1] == '-') {
        t->lex = arith_dec;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = arith_sub;
        return 0;
    }
}

int dfa_div(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_div;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = arith_div;
        return 0;
    }
}

int dfa_less(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = logic_le;
        t->str[1] = str[1];
        return 1;
    } else if (str[1] == '<' && str[2] == '=') {
        t->str[1] = str[1];
        t->str[2] = str[2];
        t->lex = assn_lsh;
        return 2;
    } else if (str[1] == '<') {
        t->str[1] = str[1];
        t->str[2] = str[2];
        t->lex = bit_lsh;
        return 1;
    } else {
        t->lex = logic_lt;
        return 0;
    }
}

int dfa_greater(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = logic_ge;
        t->str[1] = str[1];
        return 1;
    } else if (str[1] == '>' && str[2] == '=') {
        t->lex = assn_rsh;
        t->str[1] = str[1];
        t->str[2] = str[2];
        return 2;
    } else if (str[1] == '>') {
        t->lex = bit_rsh;
        t->str[1] = str[1];
        t->str[2] = str[2];
        return 1;
    } else {
        t->lex = logic_gt;
        return 0;
    }
}

int dfa_equal(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = logic_eq;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = assn_get;
        return 0;
    }
}

int dfa_carret(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_xor;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = bit_xor;
        return 0;
    }
}

int dfa_pipe(char *str, token *t) {
    t->str[0] = str[0];
    if (str[1] == '=') {
        t->lex = assn_or;
        t->str[1] = str[1];
        return 1;
    } else if (str[1] == '|') {
        t->lex = logic_or;
        t->str[1] = str[1];
        return 1;
    } else {
        t->lex = bit_or;
        return 0;
    }
}

int dfa_oparen(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = open_paren;
    return 0;
}

int dfa_cparen(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = close_paren;
    return 0;
}

int dfa_comma(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = series_op;
    return 0;
}

int dfa_qmark(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = tern_cond;
    return 0;
}

int dfa_colon(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = tern_else;
    return 0;
}

int dfa_semi(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = semi_colon;
    return 0;
}

int dfa_tilde(char *str, token *t) {
    t->str[0] = str[0];
    t->lex = bit_ones;
    return 0;
}

