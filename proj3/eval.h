#ifendf EVAL_H
#define EVAL_H

#include "stdio.h"
#include "sym.h"
#include "syn.h"

typedef enum{
  SYMBOL, OPERAND, VARIABLE, NUMBER, TEMPORARY, OPERATOR
}symbol_type;

typedef enum{
  NONE, INTEGER, DOUBLE
}value_type;

typedef enum{
  LEFT_TO_RIGHT, RIGHT_TO_LEFT
}order_type;

void SaveDeclType(token_t T);

void NewDeclaration(char *L);

void NumLitFound(char *N);

void VariableFound(char *N);
