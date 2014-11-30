#ifndef EVAL_H
#define EVAL_H

#include "stdio.h"
#include "sym.h"
#include "lex.h"

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
    POSTPP, POSTMM, PREPP, PREMM, UPLUS, UMINUS, NEGATION, MULTIPLY, DIVIDE,
    REMAINDER, BPLUS, BMINUS, SHIFTLEFT, SHIFTRIGHT, LESSTHAN, LESSTHANEQ,
    GREATERTHAN, GREATERTHANEQ, EQUAL, NOTEQUAL, BITAND, BITXOR, BITOR,
    LOGICALAND, LOGICALOR, ASSIGNMENT, PLUSEQUAL, MINUSEQUAL, MULTEQUAL,
    DIVEQUAL, REMEQUAL, SHIFTLEFTEQUAL, SHIFTRIGHTEQUAL, BITANDEQUAL,
    BITXOREQUAL, BITOREQUAL, SCOPESTART, SCOPEEND
} OpCode_type;


void SaveDeclType(token_t T);

bool NewDeclaration(table *t, char *L);

void NumLitFound(char *N);

void VariableFound(char *N);

void InitSymantic(void);

#endif  // EVAL_H
