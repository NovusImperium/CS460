/* psuedo c interpreter */

%token_type {int}

%left PLUS MINUS.
%left DIVIDE TIMES.
%include {
#include <stdio.h>
#include "lex.h"
}



%start_symbol program

program   ::= stmt SEMI more_stmt EOFT.
program   ::= decl SEMI more_stmt EOFT.
more_stmt ::= stmt SEMI more_stmt.
more_stmt ::= decl SEMI more_stmt.
more_stmt ::=.
stmt      ::= term stmt_tail.
decl      ::= ntype ID decl_tail more_decl.
ntype     ::= INTTYPE.
ntype     ::= DBLTYPE.
decl_tail ::= ASSIGN stmt.
decl_tail ::=.
more_decl ::= COMMA ID decl_tail more_decl.
more_decl ::=.
term      ::= pre var post.
term      ::= LPAREN stmt RPAREN.
pre       ::= uoppre pre.
pre       ::=.
uoppre    ::= PLUS.
uoppre    ::= MINUS.
uoppre    ::= TILDE.
uoppre    ::= NOT.
uoppre    ::= PLUSPLUS.
uoppre    ::= MINUSMINUS.
var       ::= ID.
var       ::= NUMLIT.
post      ::= PLUSPLUS.
post      ::= MINUSMINUS.
post      ::=.
stmt_tail ::= binop stmt.
stmt_tail ::= QST stmt COLON stmt.
stmt_tail ::=.
binop     ::= ASSIGN.
binop     ::= EQUALS.
binop     ::= NOTEQ.
binop     ::= LESS.
binop     ::= LESSEQ.
binop     ::= GTR.
binop     ::= GTREQ.
binop     ::= LOGOR.
binop     ::= LOGAND.
binop     ::= NOT.
binop     ::= PLUSEQ.
binop     ::= MINUSEQ.
binop     ::= TILDEQ.
binop     ::= XOREQ.
binop     ::= OREQ.
binop     ::= DIVEQ.
binop     ::= MULTEQ.
binop     ::= MODEQ.
binop     ::= LSHEQ.
binop     ::= RSHEQ.
binop     ::= POWEQ.
binop     ::= DIV.
binop     ::= MULT.
binop     ::= POW.
binop     ::= MOD.
binop     ::= PLUS.
binop     ::= PLUSPLUS.
binop     ::= MINUS.
binop     ::= MINUSMINUS.
binop     ::= BITXOR.
binop     ::= BITOR.
binop     ::= BITAND.
binop     ::= TILDE.
binop     ::= LSH.
binop     ::= RSH.

