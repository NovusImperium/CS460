#ifndef SYN_H
#define SYN_H

#include <stdlib.h>
#include "dfa.h"
//#include "sym.h"
#include "lex.h"

void error(char * msg);

void start(char * filename);

int accept(token_t t);

int expect (token_t t);

void program(void);

void stmt(void);

void more_stmts(void);

void more_decls(void);

void decl(void);

void decl_tail(void);

void ntype(void);

void term(void);

void pre(void);

void post(void);

void var(void);

void binop(void);

void stmt_tail(void);

#endif
