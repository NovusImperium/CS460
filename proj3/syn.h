#include "dfa.h"
#include "lex.h"
#include "eval.h"

#include <stdlib.h>

#ifndef SYN_H
#define SYN_H

extern void error(char *msg);

extern void start(char *filename);

extern void stop(void);

extern int accept(token_t t);

extern int expect(token_t t);

extern void program(void);

extern void stmt(void);

extern void more_stmts(void);

extern void more_decls(void);

extern void decl(void);

extern void decl_tail(void);

extern void ntype(void);

extern void term(void);

extern void pre(void);

extern void post(void);

extern void var(void);

extern void binop(void);

extern void stmt_tail(void);

#endif
