#ifndef EVAL_H
#define EVAL_H

#include "stdio.h"
#include "sym.h"
#include "lex.h"
#include "defs.h"

typedef enum {
    OPERR = 0,
    POSTPP,
    POSTMM, 
    PREPP, 
    PREMM, 
    UPLUS, 
    UMINUS, 
    NEGATION, 
    MULTIPLY, 
    DIVIDE,
    REMAINDER, 
    BPLUS, 
    BMINUS, 
    SHIFTLEFT, 
    SHIFTRIGHT, 
    LESSTHAN, 
    LESSTHANEQ,
    GREATERTHAN, 
    GREATERTHANEQ, 
    EQUAL, 
    NOTEQUAL, 
    BITAND, 
    BITXOR, 
    BITOR,
    LOGICALAND, 
    LOGICALOR, 
    ASSIGNMENT, 
    PLUSEQUAL, 
    MINUSEQUAL, 
    MULTEQUAL,
    DIVEQUAL, 
    REMEQUAL, 
    SHIFTLEFTEQUAL, 
    SHIFTRIGHTEQUAL, 
    BITANDEQUAL,
    BITXOREQUAL, 
    BITOREQUAL, 
    SCOPESTART, 
    SCOPEEND, 
    POWER, 
    POWEREQUAL, 
    TERNQUESTION, 
    TERNCOLON
} OpCode_type;

bool NewDeclaration(table *t, char *L);

void NumLitFound(char *num);

void VariableFound(char *var);

void OperatorFound(OpCode_type op);

void NewScopeFound(void);

void EndScopeFound(void);

void InitSemantic(void);

void Evaluate(void);

#endif  // EVAL_H
