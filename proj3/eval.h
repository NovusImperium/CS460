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

void SaveDeclType(token_t T);

bool NewDeclaration(table *t, char *L);

void NumLitFound(char *num);

void VariableFound(char *var);

void OperatorFound(OpCode_type op);

void NewScopeDound(void);

void InitSemantic(void);

#endif  // EVAL_H
