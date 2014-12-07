#ifndef EVAL_H
#define EVAL_H

#include "stdio.h"
#include "sym.h"
#include "lex.h"
#include "defs.h"

typedef enum {
    SYMBOL, OPERAND, VARIABLE, NUMBER, TEMPORARY, OPERATOR
} symbol_type;

typedef enum {
    NONE, INTEGER, DOUBLE
} value_type;

typedef enum {
    LEFT_TO_RIGHT, RIGHT_TO_LEFT
} order_type;

typedef enum {
    OPERR = -1, POSTPP = 0, POSTMM, PREPP, PREMM, UPLUS, UMINUS, NEGATION, MULTIPLY, DIVIDE,
    REMAINDER, BPLUS, BMINUS, SHIFTLEFT, SHIFTRIGHT, LESSTHAN, LESSTHANEQ,
    GREATERTHAN, GREATERTHANEQ, EQUAL, NOTEQUAL, BITAND, BITXOR, BITOR,
    LOGICALAND, LOGICALOR, ASSIGNMENT, PLUSEQUAL, MINUSEQUAL, MULTEQUAL,
    DIVEQUAL, REMEQUAL, SHIFTLEFTEQUAL, SHIFTRIGHTEQUAL, BITANDEQUAL,
    BITXOREQUAL, BITOREQUAL, SCOPESTART, SCOPEEND, POWER, POWEREQUAL, TERNQUESTION, TERNCOLON
} OpCode_type;

typedef value (*eval_func)(sym *l, sym *r);

extern int operatorPrecidence[];

void SaveDeclType(token_t T);

bool NewDeclaration(table *t, char *L);

void NumLitFound(char *num);

void VariableFound(char *var);

void OperatorFound(OpCode_type op);

void InitSemantic(void);

value MulEq(sym *left, sym *right);

value BitXorEq(sym *left, sym *right);

//TBD the following 3 functions are just useless stubs at the moment
value ScpStart(sym *left, sym *right);

value TernQuest(sym *left, sym *right);

value ScpEnd(sym *left, sym *right);

value PPPre(sym *left, sym *right);

value PPPost(sym *left, sym *right);

value MMPre(sym *left, sym *right);

value MMPost(sym *left, sym *right);

value UPlus(sym *left, sym *right);

value UMinus(sym *left, sym *right);

value Negate(sym *left, sym *right);

value Mult(sym *left, sym *right);

value Div(sym *left, sym *right);

value Rem(sym *left, sym *right);

value BPlus(sym *left, sym *right);

value BMinus(sym *left, sym *right);

value LShift(sym *left, sym *right);

value RShift(sym *left, sym *right);

value LessThan(sym *left, sym *right);

value LessThanEq(sym *left, sym *right);

value GreaterThan(sym *left, sym *right);

value GreaterThanEq(sym *left, sym *right);

value EqualTo(sym *left, sym *right);

value NotEQ(sym *left, sym *right);

value BitAnd(sym *left, sym *right);

value BitXor(sym *left, sym *right);

value BitOr(sym *left, sym *right);

value LogAnd(sym *left, sym *right);

value LogOr(sym *left, sym *right);

value Assign(sym *left, sym *right);

value PlusEq(sym *left, sym *right);

value MinEq(sym *left, sym *right);

value DivEq(sym *left, sym *right);

value RemEq(sym *left, sym *right);

value LShiftEq(sym *left, sym *right);

value RShiftEq(sym *left, sym *right);

value BitAndEq(sym *left, sym *right);

value PwrEq(sym *left, sym *right);

value Pwr(sym *left, sym *right);

value BitOrEq(sym *left, sym *right);

#endif  // EVAL_H
