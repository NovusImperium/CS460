#include "eval.h"
#include "syn.h"
#include "op_func.h"
#include "defs.h"

extern FILE *sym_file;
extern FILE *dbg_file;
extern table *tab;
extern token_t currentType;
static array *operators;
static array *operands;
static char *opcode_str[] = {
        "OPERR",
        "POSTPP",
        "POSTMM",
        "PREPP",
        "PREMM",
        "UPLUS",
        "UMINUS",
        "NEGATION",
        "MULTIPLY",
        "DIVIDE",
        "REMAINDER",
        "BPLUS",
        "BMINUS",
        "SHIFTLEFT",
        "SHIFTRIGHT",
        "LESSTHAN",
        "LESSTHANEQ",
        "GREATERTHAN",
        "GREATERTHANEQ",
        "EQUAL",
        "NOTEQUAL",
        "BITAND",
        "BITXOR",
        "BITOR",
        "LOGICALAND",
        "LOGICALOR",
        "ASSIGNMENT",
        "PLUSEQUAL",
        "MINUSEQUAL",
        "MULTEQUAL",
        "DIVEQUAL",
        "REMEQUAL",
        "SHIFTLEFTEQUAL",
        "SHIFTRIGHTEQUAL",
        "BITANDEQUAL",
        "BITXOREQUAL",
        "BITOREQUAL",
        "SCOPESTART",
        "SCOPEEND",
        "POWER",
        "POWEREQUAL",
        "TERNQUESTION",
        "TERNCOLON"
};

static inline void eval_current(void);

static inline void *print_opcode(void *);

inline bool NewDeclaration(table *t, char *L) {
    value val;
    if (currentType == INTTYPE) {
        val.flag = true;
        val.ival = 0;
    } else {
        val.flag = false;
        val.dval = 0.0;
    }

    optional opt;
    if (!(opt = create_sym(t, L, val)).e) {
        //printf("Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
        fprintf(sym_file, "Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
        stop();
        return false;
    } else {
        arr_push(operands, opt.val);
        return true;
    }
}

inline void NumLitFound(char *num) {
    value val;
    val.flag = true;
    char *str = num;
    char c;
    while ((c = *str++)) {
        if (c == '.') {
            val.flag = false;
            val.dval = atof(num);
            break;
        }
    }
    if (val.flag) {
        val.ival = atoi(num);
    }

    optional opt;
    if (!(opt = create_sym(tab, num, val)).e) {
        stop();
    }

    arr_push(operands, opt.val);
}

inline void VariableFound(char *var) {
    optional opt = get_sym(tab, var);
    if (opt.e) {
        arr_push(operands, opt.val);
    } else {
        //printf("Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        fprintf(sym_file, "Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        stop();
    }
}

inline void OperatorFound(OpCode_type op) {
    /*
    if (op_func_is_binary[op]) {
        //printf("add operator %s\n", opcode_str[op]);
    }
    */
    optional opt;
    while (arr_size(operators) > 0 && (opt = arr_peek(operators)).e &&
            operatorPrecedence[(OpCode_type) opt.val] < operatorPrecedence[op]) {
        eval_current();
    }
    if (!opt.e) {
        stop();
    }

    arr_push(operators, (void *) op);
}

inline void NewScopeFound(void) {
    arr_push(operators, (void *) SCOPESTART);
}

inline void EndScopeFound(void) {
    //printf("inside EndScope\n");
    optional opt;
    while ((opt = arr_peek(operators)).e && (OpCode_type)opt.val != SCOPESTART) {
        eval_current();
    }
    // arr_pop(operators);
    //printf("exiting EndScope\n");
}

inline void InitSemantic(void) {
    optional opt = init_sym();
    if (!opt.e) {
        fprintf(dbg_file, "Initialization of symbol table failed.\n");
        exit(1);
    } else {
        tab = opt.val;
    }

    opt = arr_init(32);
    if (!opt.e) {
        fprintf(dbg_file, "Initializiation of operator array failed.\n");
        exit(1);
    } else {
        operators = opt.val;
    }

    opt = arr_init(32);
    if (!opt.e) {
        fprintf(dbg_file, "Initilization of operand array failed.\n");
        exit(1);
    } else {
        operands = opt.val;
    }
}

inline void Evaluate(void) {
    while (arr_size(operators) != 0) {
        eval_current();
    }
    arr_reset(operands);
    arr_reset(operators);

    //printf("Reset operators and operands\n");
}

inline void *print_opcode(void *a) {
    //printf(" * %s\n", opcode_str[(OpCode_type) a]);

    return a;
}

inline void eval_current(void) {
    //printf("\n\ncurrent operators:\n");
    //arr_foreach(operators, print_opcode);
    //printf("current operands:\n");
    //arr_foreach(operands, print_sym);
    //printf("continue eval\n");

    optional opt;
    if (!(opt = arr_pop(operators)).e) {
        stop();
    } else if ((OpCode_type)opt.val != SCOPEEND) {
        OpCode_type op = (OpCode_type) opt.val;

        sym *l, *r;
        if (op_is_binary[op]) {
            if (!(opt = arr_pop(operands)).e) {
                stop();
            }
            r = opt.val;

            if (!(opt = arr_pop(operands)).e) {
                stop();
            }
            l = opt.val;
        } else {
            if (!(opt = arr_pop(operands)).e) {
                stop();
            }
            l = opt.val;
            r = null;
        }

        arr_push(operands, op_funcs[op](l, r));
    }
}

bool ConditionalFound() {
    optional opt;
    //printf("inside conditional");
    while ((opt = arr_peek(operators)).e && !op_is_assign[(OpCode_type)opt.val]) {
        eval_current();
    }

    //printf("finished eval with operands: \n");
    //arr_foreach(operands, print_sym);

    if (opt.e && (opt = arr_pop(operands)).e) {
        return get_value(opt.val).ival != 0;
    } else {
        stop();
    }
}
