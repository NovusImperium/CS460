#ifendf EVAL_H
#define EVAL_H

#include "stdio.h"
#include "sym.h"

typedef enum{
  SYMBOL, OPERAND, VARIABLE, NUMBER, TEMPORARY, OPERATOR
}symbol_type;

typedef enum{
  NONE, INTEGER, DOUBLE
}value_type;

typedef enum{
  LEFT_TO_RIGHT, RIGHT_TO_LEFT
}order_type;

typedef enum{
  POSTPP, POSTMM, PREPP, PREMM, UPLUS, UMINUS, NEGATION, MULTIPLY, DIVIDE,
  REMAINDER, BPLUS, BMINUS, SHIFTLEFT, SHIFTRIGHT, LESSTHAN, LESSTHANEQ,
  GREATERTHAN, GREATERTHANEQ, EQUAL, NOTEQUAL, BITAND, BITXOR, BITOR,
  LOGICALAND, LOGICALOR, ASSIGNMENT, PLUSEQUAL, MINUSEQUAL, MULTEQUAL,
  DIVEQUAL, REMEQUAL, SHIFTLEFTEQUAL, SHIFTRIGHTEQUAL, BITANDEQUAL,
  BITXOREQUAL, BITOREQUAL
}OpCode_type;


void SaveDeclType(token_t T);

void NewDeclaration(char *L);

void NumLitFound(char *N);

void VariableFound(char *N);

void InitSymantic(void);
