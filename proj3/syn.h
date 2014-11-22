#ifndef SYN_H
#define SYN_H

#include <stdlib.h>
#include "dfa.h"

void error(char * msg);

void start(char * filename);

int accept(token t);

int expect (token t);

void program(void);

void stmt(void);

void more_stmts(void);

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
