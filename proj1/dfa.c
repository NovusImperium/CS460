#include <ctype.h>
#include <stdbool.h>
#include "dfa.h"

int dfa_start(char *str, token_type *t) {
    return (dfa_trans[str[0]])(str, t);
}

int dfa_invalid(char *str, token_type *t) {
    *t = ERROR;
    return 0;
}

int dfa_word(char *str, token_type *t) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            break;
        }
    }

    *t = IDENT;
    return i - 1;
}

int dfa_num(char *str, token_type *t) {
    if (str[0] == '.' && !isdigit(str[1])) {
        *t = ERROR;
        return 0;
    }

    int i;
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
        } else {
            break;
        }
    }

    *t = NUMLIT;
    return i - 1;
}

int dfa_not(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = NOTEQ;
        return 1;
    } else {
        *t = NOT;
        return 0;
    }
}

int dfa_mod(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = MODEQ;
        return 1;
    } else {
        *t = MOD;
        return 0;
    }
}

int dfa_and(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = ANDEQ;
        return 1;
    } else if (str[1] == '&') {
        *t = LOGAND;
        return 1;
    } else {
        *t = AND;
        return 0;
    }
}

int dfa_mul(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = MULTEQ;
        return 1;
    } else if (str[1] == '*' && str[2] == '=') {
        *t = EXPEQ;
        return 2;
    } else if (str[1] == '*') {
        *t = EXP;
        return 1;
    } else {
        *t = MULT;
        return 0;
    }
}

int dfa_add(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = PLUSEQ;
        return 1;
    } else if (str[1] == '+') {
        *t = PLUSPLUS;
        return 1;
    } else {
        *t = PLUS;
        return 0;
    }
}

int dfa_sub(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = MINUSEQ;
        return 1;
    } else if (str[1] == '-') {
        *t = MINUSMINUS;
        return 1;
    } else {
        *t = MINUS;
        return 0;
    }
}

int dfa_div(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = DIVEQ;
        return 1;
    } else {
        *t = DIV;
        return 0;
    }
}

int dfa_less(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = LTE;
        return 1;
    } else if (str[1] == '<' && str[2] == '=') {
        *t = SHIFTLEQ;
        return 2;
    } else if (str[1] == '<') {
        *t = SHIFTL;
        return 1;
    } else {
        *t = LT;
        return 0;
    }
}

int dfa_greater(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = GTE;
        return 1;
    } else if (str[1] == '>' && str[2] == '=') {
        *t = SHIFTREQ;
        return 2;
    } else if (str[1] == '>') {
        *t = SHIFTR;
        return 1;
    } else {
        *t = GT;
        return 0;
    }
}

int dfa_equal(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = EQUALTO;
        return 1;
    } else {
        *t = ASSIGN;
        return 0;
    }
}

int dfa_carret(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = XOREQ;
        return 1;
    } else {
        *t = XOR;
        return 0;
    }
}

int dfa_pipe(char *str, token_type *t) {
    if (str[1] == '=') {
        *t = OREQ;
        return 1;
    } else if (str[1] == '|') {
        *t = OR;
        return 1;
    } else {
        *t = LOGOR;
        return 0;
    }
}

int dfa_oparen(char *str, token_type *t) {
    *t = LPAREN;
    return 0;
}

int dfa_cparen(char *str, token_type *t) {
    *t = RPAREN;
    return 0;
}

int dfa_comma(char *str, token_type *t) {
    *t = COMMA;
    return 0;
}

int dfa_qmark(char *str, token_type *t) {
    *t = QUEST;
    return 0;
}

int dfa_colon(char *str, token_type *t) {
    *t = COLON;
    return 0;
}

int dfa_semi(char *str, token_type *t) {
    *t = SEMI;
    return 0;
}

int dfa_tilde(char *str, token_type *t) {
    *t = TILDE;
    return 0;
}

