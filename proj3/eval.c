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

static void eval_current(void);

bool NewDeclaration(table *t, char *L) {
    value val;
    if ((val.flag = currentType == INTTYPE)) {
      val.ival = 0;
    } else {
        val.dval = 0.0;
    }

    optional opt;
    if (!(opt = create_sym(t, L, val)).e) {
        printf("Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
        fprintf(sym_file, "Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
        stop(t, sym_file);
        return false;
    } else {
        arr_push(operands, opt.val);
        return true;
    }
}

void NumLitFound(char *num) {
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
        stop(tab, sym_file);
    }

    arr_push(operands, opt.val);
}

void VariableFound(char *var) {
    optional opt = get_sym(tab, var);
    if (opt.e) {
        arr_push(operands, opt.val);
    } else {
        printf("Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        fprintf(sym_file, "Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        stop(tab, sym_file);
    }
}

void OperatorFound(OpCode_type op) {
    optional opt;
    while (arr_size(operators) > 0 && (opt = arr_peek(operators)).e &&
            operatorPrecedence[(OpCode_type) opt.val] < operatorPrecedence[op]) {
        eval_current();
    }
    if (!opt.e) {
        stop(tab, sym_file);
    }

    arr_push(operators, (void *) op);
}

void NewScopeFound(void) {
    arr_push(operators, (void *) SCOPESTART);
}

void EndScopeFound(void) {
    optional opt;
    while ((opt = arr_peek(operators)).e && opt.val != (void *) SCOPESTART) {
        eval_current();
    }
    arr_pop(operators);
}

void eval_current(void) {
    optional opt;
    if (!(opt = arr_pop(operators)).e) {
        stop(tab, sym_file);
    }
    OpCode_type op = (OpCode_type) opt.val;

    sym *l, *r;
    if (op_func_is_binary[op]) {
        if (!(opt = arr_pop(operands)).e) {
            stop(tab, sym_file);
        }
        r = opt.val;

        if (!(opt = arr_pop(operands)).e) {
            stop(tab, sym_file);
        }
        l = opt.val;
    } else {
        if (!(opt = arr_pop(operands)).e) {
            stop(tab, sym_file);
        }
        l = opt.val;
        r = null;
    }

    arr_push(operands, op_funcs[op](l, r));

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

void Evaluate(void) {
    while (arr_size(operators) != 0) {
        eval_current();
    }
    arr_reset(operands);
    arr_reset(operators);
}
