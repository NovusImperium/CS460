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

static int operatorPrecidence[] = {    

  2, // POSTPP, 
  2, // POSTMM, 
  3, // PREPP, 
  3, // PREMM, 
  3, // UPLUS, 
  3, // UMINUS, 
  3, // NEGATION, 
  5, // MULTIPLY, 
  5, // DIVIDE,
  5, // REMAINDER, 
  6, // BPLUS, 
  6, // BMINUS, 
  7, // SHIFTLEFT, 
  7, // SHIFTRIGHT, 
  8, // LESSTHAN, 
  8, // LESSTHANEQ,
  8, // GREATERTHAN, 
  8, // GREATERTHANEQ, 
  9, // EQUAL, 
  9, // NOTEQUAL, 
  10, // BITAND, 
  11, // BITXOR, 
  12, // BITOR,
  13, // LOGICALAND, 
  14, // LOGICALOR, 
  15, // ASSIGNMENT, 
  15, // PLUSEQUAL, 
  15, // MINUSEQUAL, 
  15, // MULTEQUAL,		       
  15, // DIVEQUAL, 
  15, // REMEQUAL, 
  15, // SHIFTLEFTEQUAL, 
  15, // SHIFTRIGHTEQUAL, 
  15, // BITANDEQUAL,
  15, // BITXOREQUAL, 
  15, // BITOREQUAL, 
  30,  // SCOPESTART, 
  1, // SCOPEEND,
  4, // POWER, 
  15,// POWEREQUAL, 
  15, // TERNQUESTION, 
  15 // TERNCOLON

};


void SaveDeclType(token_t T);

bool NewDeclaration(table *t, char *L);

void NumLitFound(char *num);

void VariableFound(char *var);

void OperatorFound(OpCode_type op);

void InitSemantic(void);

#endif  // EVAL_H
