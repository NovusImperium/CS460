#ifndef PARSER_H
#define PARSER_H

#include "defs.h"
#include "lex.h"

typedef enum {
    program, more_stmts, stmt, decl, ntype, decl_tail, term,
    pre, uoppre, var, post, stmt_tail, binop
} nterm_t;

// initialize the parser state, currently does not need initialization variables
inline bool init_parser();

// parse a single token, returns true if the token does not force an error
inline bool parse_token(token_t t);

#endif  // PARSER_H
