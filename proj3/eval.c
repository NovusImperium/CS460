#include "eval.h"
#include "syn.h"

extern FILE *sym_file;
extern FILE *dbg_file;
extern table *tab;
extern token_t currentType;
static array *operators;
static array *operands;

bool NewDeclaration(table *t, char *L) {
    optional opt = get_sym(t, L);
    if (opt.e) {
        printf("Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
        fprintf(sym_file, "Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
        stop(t, sym_file);
        return false;
    } else {
        value val;
        if ((val.flag = currentType == INTTYPE)) {
            val.ival = 0;
        } else {
            val.dval = 0.0;
        }
        return insert_sym(t, L, val);
    }
}

void NumLitFound(char *num) {
    optional opt = get_sym(tab, num);
    if (opt.e) {
        arr_push(operands, num);
    } else {
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
        insert_sym(tab, num, val);
    }
}

void VariableFound(char *var) {
    optional opt = get_sym(tab, var);
    if (opt.e) {
        arr_push(operands, var);
    } else {
        printf("Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        fprintf(sym_file, "Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        stop(tab, sym_file);
    }
}

void OperatorFound(OpCode_type op) {
    if (arr_size(operators) == 0)
        arr_push(operators, (void *) op);
}

void InitSemantic(void) {
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

