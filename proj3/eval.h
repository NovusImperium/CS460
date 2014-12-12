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

extern inline bool NewDeclaration(table *t, char *L);

extern inline void NumLitFound(char *num);

extern inline void VariableFound(char *var);

extern inline void OperatorFound(OpCode_type op);

extern inline bool ConditionalFound();

extern inline void NewScopeFound(void);

extern inline void EndScopeFound(void);

extern inline void InitSemantic(void);

extern inline void Evaluate(void);

#endif  // EVAL_H
