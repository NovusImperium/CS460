#ifndef DFA_H
#define DFA_H

#include "lex.h"

// entry point to the lexer dfa, takes a single line of input with the line number
// the input line must terminate in a null-char
// returns an array of tokens that represent the valid lexemes found on the input line
inline int dfa_start(char *str, token_t *t);

// generate an ID or keyword token starting from the beginning of the input string
// returns the length of the ID or keyword, up to and beyond the 31 character length limit
inline int dfa_word(char *str, token_t *t);

// generate an int or float token starting from the beginning of the input string
// returns the length of the int or float, up to and beyond the 31 character length limit
inline int dfa_num(char *str, token_t *t);

// generate an invalid_char token
// returns 0
inline int dfa_invalid(char *str, token_t *t);

// generate logic_not or logic_ne token
// returns 0 if logic_not, 1 if logic_ne
inline int dfa_not(char *str, token_t *t);

// generate arith_mod or assn_mod token
// returns 0 if arith_mod, 1 if assn_mod
inline int dfa_mod(char *str, token_t *t);

// generate bit_and, assn_and, or logic_and token
// returns 0 if bit_and, 1 if assn_and or logic_and
inline int dfa_and(char *str, token_t *t);

// generate an open_paren token
// returns 0
inline int dfa_oparen(char *str, token_t *t);

// generate a close_paren token
// returns 0
inline int dfa_cparen(char *str, token_t *t);

// generate arith_mul or assn_mul token
// returns 0
inline int dfa_mul(char *str, token_t *t);

// generate arith_add, arith_inc, or assn_add token
// returns 0 if arith_add, 1 if arith_inc or assn_add
inline int dfa_add(char *str, token_t *t);

// generate series_op token
// returns 0
inline int dfa_comma(char *str, token_t *t);

// generate arith_sub, arith_dec, or assn_sub token
// returns 0 if arith_sub, 1 if arith_dec or assn_sub
inline int dfa_sub(char *str, token_t *t);

// generate arith_div or assn_div token
// returns 0 if arith_div, 1 if assn_div
inline int dfa_div(char *str, token_t *t);

// generate tern_cond token
// returns 0
inline int dfa_qmark(char *str, token_t *t);

// generate tern_else token
// returns 0
inline int dfa_colon(char *str, token_t *t);

// generate semi_colon token
// returns 0
inline int dfa_semi(char *str, token_t *t);

// generate logic_lt, logic_le, bit_lsh, or assn_lsh token
// returns 0 if logic_lt, 1 if logic_le or arith_lsh, or 2 if assn_lsh
inline int dfa_less(char *str, token_t *t);

// generate logic_gt, logic_ge, bit_rsh, or assn_rsh token
// returns 0 if logic_gt, 1 if logic_ge or arith_rsh, or 2 if assn_rsh
inline int dfa_greater(char *str, token_t *t);

// generate assn_get or logic_eq token
// returns 0 if assn_get, 1 if logic_eq
inline int dfa_equal(char *str, token_t *t);

// generate bit_xor or assn_xor token
// returns 0 if bit_xor, 1 if assn_xor
inline int dfa_carret(char *str, token_t *t);

// generate bit_or, assn_or, or logic_or token
// returns 0 if bit_or, 1 if assn_or or logic_or
inline int dfa_pipe(char *str, token_t *t);

// generate bit_ones token
// returns 0
inline int dfa_tilde(char *str, token_t *t);

// generate string literal token
// returns the length of the string literal - 1
inline int dfa_string(char *str, token_t *t);

// generate char literal token
// returns the length of the char literal - 1
inline int dfa_char(char *str, token_t *t);

// generate right-bracket token
// returns 0
inline int dfa_rbrack(char *str, token_t *t);

// generate left-bracket token
// returns 0
inline int dfa_lbrack(char *str, token_t *t);

// generate right-brace token
// returns 0
inline int dfa_rbrace(char *str, token_t *t);

// generate left-brace token
// returns 0
inline int dfa_lbrace(char *str, token_t *t);

// store an array of dfa state functions to call from the starting state
// the index of each state function corresponds to the ascii value of the character to transition from
typedef int (*dfa_func)(char *, token_t *);

#endif
