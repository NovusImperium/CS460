#ifndef PARSER_H
#define PARSER_H

#include "defs.h"
#include "lex.h"

typedef enum {
    program = 0, more_stmts, stmt, decl, ntype, decl_tail, term, pre, uoppre, var, post, stmt_tail, binop, pop_token,
} symbol_t;

typedef enum {
    parser_err = -1, adv_token = 0, keep_token
} parser_return_t;

// initialize the parser state, currently does not need initialization variables
inline bool init_parser();

// parse a single token, returns true if the token does not force an error
inline parser_return_t parse_token(token_t t);

#endif  // PARSER_H
