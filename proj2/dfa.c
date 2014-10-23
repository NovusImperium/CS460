#include <ctype.h>
#include <stdbool.h>
#include "dfa.h"

static const dfa_func dfa_trans[256] = {
        dfa_invalid,    // 0    Null char
        dfa_invalid,    // 1    Start of Heading
        dfa_invalid,    // 2    Start of Text
        dfa_invalid,    // 3    End of Text
        dfa_invalid,    // 4    End of Transmission
        dfa_invalid,    // 5    Enquiry
        dfa_invalid,    // 6    Acknowledgment
        dfa_invalid,    // 7    Bell
        dfa_invalid,    // 8    Back Space
        dfa_invalid,    // 9    Horizontal Tab
        dfa_invalid,    // 10   Line Feed
        dfa_invalid,    // 11   Vertical Tab
        dfa_invalid,    // 12   Form Feed
        dfa_invalid,    // 13   Carriage Return
        dfa_invalid,    // 14   Shift Out / X-On
        dfa_invalid,    // 15   Shift In / X-Off
        dfa_invalid,    // 16   Data Line Escape
        dfa_invalid,    // 17   Device Control 1 (oft. XON)
        dfa_invalid,    // 18   Device Control 2
        dfa_invalid,    // 19   Device Control 3 (oft. XOFF)
        dfa_invalid,    // 20   Device Control 4
        dfa_invalid,    // 21   Negative Acknowledgement
        dfa_invalid,    // 22   Synchronous Idle
        dfa_invalid,    // 23   End of Transmit Block
        dfa_invalid,    // 24   Cancel
        dfa_invalid,    // 25   End of Medium
        dfa_invalid,    // 26   Substitute
        dfa_invalid,    // 27   Escape
        dfa_invalid,    // 28   File Separator
        dfa_invalid,    // 29   Group Separator
        dfa_invalid,    // 30   Record Separator
        dfa_invalid,    // 31   Unit Separator
        dfa_invalid,    // 32   SPACE
        dfa_not,        // 33   !
        dfa_string,     // 34   "
        dfa_invalid,    // 35   #
        dfa_invalid,    // 36   $
        dfa_mod,        // 37   %
        dfa_and,        // 38   &
        dfa_char,       // 39   '
        dfa_oparen,     // 40   (
        dfa_cparen,     // 41   )
        dfa_mul,        // 42   *
        dfa_add,        // 43   +
        dfa_comma,      // 44   ,
        dfa_sub,        // 45   -
        dfa_num,        // 46   .
        dfa_div,        // 47   /
        dfa_num,        // 48   0
        dfa_num,        // 49   1
        dfa_num,        // 50   2
        dfa_num,        // 51   3
        dfa_num,        // 52   4
        dfa_num,        // 53   5
        dfa_num,        // 54   6
        dfa_num,        // 55   7
        dfa_num,        // 56   8
        dfa_num,        // 57   9
        dfa_colon,      // 58   :
        dfa_semi,       // 59   ;
        dfa_less,       // 60   <
        dfa_equal,      // 61   =
        dfa_greater,    // 62   >
        dfa_qmark,      // 63   ?
        dfa_invalid,    // 64   @
        dfa_word,       // 65   A
        dfa_word,       // 66   B
        dfa_word,       // 67   C
        dfa_word,       // 68   D
        dfa_word,       // 69   E
        dfa_word,       // 70   F
        dfa_word,       // 71   G
        dfa_word,       // 72   H
        dfa_word,       // 73   I
        dfa_word,       // 74   J
        dfa_word,       // 75   K
        dfa_word,       // 76   L
        dfa_word,       // 77   M
        dfa_word,       // 78   N
        dfa_word,       // 79   O
        dfa_word,       // 80   P
        dfa_word,       // 81   Q
        dfa_word,       // 82   R
        dfa_word,       // 83   S
        dfa_word,       // 84   T
        dfa_word,       // 85   U
        dfa_word,       // 86   V
        dfa_word,       // 87   W
        dfa_word,       // 88   X
        dfa_word,       // 89   Y
        dfa_word,       // 90   Z
        dfa_lbrace,     // 91   [
        dfa_invalid,    // 92   \ (backslash)
        dfa_rbrace,     // 93   ]
        dfa_carret,     // 94   ^
        dfa_word,       // 95   _
        dfa_invalid,    // 96   `
        dfa_word,       // 97   a
        dfa_word,       // 98   b
        dfa_word,       // 99   c
        dfa_word,       // 100  d
        dfa_word,       // 101  e
        dfa_word,       // 102  f
        dfa_word,       // 103  g
        dfa_word,       // 104  h
        dfa_word,       // 105  i
        dfa_word,       // 106  j
        dfa_word,       // 107  k
        dfa_word,       // 108  l
        dfa_word,       // 109  m
        dfa_word,       // 110  n
        dfa_word,       // 111  o
        dfa_word,       // 112  p
        dfa_word,       // 113  q
        dfa_word,       // 114  r
        dfa_word,       // 115  s
        dfa_word,       // 116  t
        dfa_word,       // 117  u
        dfa_word,       // 118  v
        dfa_word,       // 119  w
        dfa_word,       // 120  x
        dfa_word,       // 121  y
        dfa_word,       // 122  z
        dfa_lbrack,     // 123  {
        dfa_pipe,       // 124  |
        dfa_rbrack,     // 125  }
        dfa_tilde,      // 126  ~
        dfa_invalid,    // 127  DEL
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
        dfa_invalid,    // extended ascii character
};

inline int dfa_start(char *str, token_t *t) {
    return dfa_trans[str[0]](str, t);
}

inline int dfa_invalid(char *str, token_t *t) {
    *t = err_tok;
    return 0;
}

inline int dfa_word(char *str, token_t *t) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            break;
        }
    }

    *t = id;
    return i - 1;
}

inline int dfa_num(char *str, token_t *t) {
    if (str[0] == '.' && !isdigit(str[1])) {
        *t = err_tok;
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

    *t = num_lit;
    return i - 1;
}

inline int dfa_not(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = log_neq;
        return 1;
    } else {
        *t = op_not;
        return 0;
    }
}

inline int dfa_mod(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_mod;
        return 1;
    } else {
        *t = op_mod;
        return 0;
    }
}

inline int dfa_and(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_and;
        return 1;
    } else if (str[1] == '&') {
        *t = log_and;
        return 1;
    } else {
        *t = op_and;
        return 0;
    }
}

inline int dfa_mul(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_mul;
        return 1;
    } else if (str[1] == '*' && str[2] == '=') {
        *t = assn_pow;
        return 2;
    } else if (str[1] == '*') {
        *t = op_pow;
        return 1;
    } else {
        *t = op_mul;
        return 0;
    }
}

inline int dfa_add(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_plus;
        return 1;
    } else if (str[1] == '+') {
        *t = op_inc;
        return 1;
    } else {
        *t = op_plus;
        return 0;
    }
}

inline int dfa_sub(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_minus;
        return 1;
    } else if (str[1] == '-') {
        *t = op_dec;
        return 1;
    } else {
        *t = op_minus;
        return 0;
    }
}

inline int dfa_div(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_div;
        return 1;
    } else {
        *t = op_div;
        return 0;
    }
}

inline int dfa_less(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = log_lte;
        return 1;
    } else if (str[1] == '<' && str[2] == '=') {
        *t = assn_lsh;
        return 2;
    } else if (str[1] == '<') {
        *t = op_lsh;
        return 1;
    } else {
        *t = log_lt;
        return 0;
    }
}

inline int dfa_greater(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = log_gte;
        return 1;
    } else if (str[1] == '>' && str[2] == '=') {
        *t = assn_rsh;
        return 2;
    } else if (str[1] == '>') {
        *t = op_rsh;
        return 1;
    } else {
        *t = log_gt;
        return 0;
    }
}

inline int dfa_equal(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = log_eq;
        return 1;
    } else {
        *t = assn_get;
        return 0;
    }
}

inline int dfa_carret(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_xor;
        return 1;
    } else {
        *t = op_xor;
        return 0;
    }
}

inline int dfa_pipe(char *str, token_t *t) {
    if (str[1] == '=') {
        *t = assn_or;
        return 1;
    } else if (str[1] == '|') {
        *t = op_or;
        return 1;
    } else {
        *t = log_or;
        return 0;
    }
}

inline int dfa_oparen(char *str, token_t *t) {
    *t = lparen;
    return 0;
}

inline int dfa_cparen(char *str, token_t *t) {
    *t = rparen;
    return 0;
}

inline int dfa_comma(char *str, token_t *t) {
    *t = comma;
    return 0;
}

inline int dfa_qmark(char *str, token_t *t) {
    *t = tern_cond;
    return 0;
}

inline int dfa_colon(char *str, token_t *t) {
    *t = tern_sepr;
    return 0;
}

inline int dfa_semi(char *str, token_t *t) {
    *t = semi;
    return 0;
}

inline int dfa_tilde(char *str, token_t *t) {
    *t = op_tilde;
    return 0;
}

inline int dfa_lbrace(char *str, token_t *t) {
    *t = lbrace;
    return 0;
}

inline int dfa_rbrace(char *str, token_t *t) {
    *t = rbrace;
    return 0;
}

inline int dfa_lbrack(char *str, token_t *t) {
    *t = lbrack;
    return 0;
}

inline int dfa_rbrack(char *str, token_t *t) {
    *t = rbrack;
    return 0;
}

inline int dfa_char(char *str, token_t *t) {
    int i;
    for (i = 0; str[i + 1] != '\0'; i++) {
        if (str[i] != '\\' && str[i + 1] == '\'') {
            *t = char_lit;
            return i;
        }
    }

    *t = err_tok;
    return i;
}

inline int dfa_string(char *str, token_t *t) {
    int i;
    for (i = 0; str[i + 1] != '\0'; i++) {
        if (str[i] != '\\' && str[i + 1] == '\"') {
            *t = str_lit;
            return i;
        }
    }

    *t = err_tok;
    return i;
}
