#ifndef LEX_H
#define LEX_H

typedef enum {
    // meta tokens
            err_tok = 0, eof_tok,
    // user-defined
            id,
    // literal tokens
            num_lit,
    // operator tokens
            op_plus, op_minus, op_mul, op_div, op_mod, op_pow, op_not, op_and, op_or, op_xor, op_lsh, op_rsh, op_inc, op_dec,
    op_tilde,
    // assignment tokens
            assn_get, assn_plus, assn_minus, assn_mul, assn_div, assn_mod, assn_pow, assn_and, assn_or, assn_xor, assn_lsh,
    assn_rsh,
    // logical tokens
            log_neq, log_and, log_or, log_lt, log_gt, log_lte, log_gte, log_eq,
    // separator tokens
            rparen, lparen, semi, qst_mark, colon, comma,
    // reserved keyword tokens
            rsv_type_int, rsv_type_double
} token_t;

extern char *token_names[];

void init_lex(char *file_name);

token_t get_token();

char *get_lexeme();

void error_message(char *msg);

void end_lex();

#endif  // LEX_H
