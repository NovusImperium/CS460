#include <math.h>
#include "eval.h"
#include "syn.h"

extern FILE *sym_file;
extern FILE *dbg_file;
extern table *tab;
extern token_t currentType;
static array *operators;
static array *operands;
static double tmp = 1.5;

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

static const eval_func eval_funcs[] = {
        PPPre, // POSTPP,
        MMPost, // POSTMM,
        PPPre, // PREPP,
        MMPre, // PREMM,
        UPlus, // UPLUS,
        UMinus, // UMINUS,
        Negate, // NEGATION,
        Mult, // MULTIPLY,
        Div, // DIVIDE,
        Rem, // REMAINDER,
        BPlus, // BPLUS,
        BMinus, // BMINUS,
        LShift, // SHIFTLEFT,
        RShift, // SHIFTRIGHT,
        LessThan, // LESSTHAN,
        LessThanEq, // LESSTHANEQ,
        GreaterThan, // GREATERTHAN,
        GreaterThanEq, // GREATERTHANEQ,
        EqualTo, // EQUAL,
        NotEQ, // NOTEQUAL,
        BitAnd, // BITAND,
        BitXor, // BITXOR,
        BitOr, // BITOR,
        LogAnd, // LOGICALAND
        LogOr, // LOGICALOR,
        Assign, // ASSIGNMENT
        PlusEq, // PLUSEQUAL,
        MinEq, // MINUSEQUAL,
        MulEq, // MULTEQUAL,
        DivEq, // DIVEQUAL,
        RemEq, // REMEQUAL,
        LShiftEq, // SHIFTLEFTEQUAL,
        RShiftEq, // SHIFTRIGHTEQUAL,
        BitAndEq, // BITANDEQUAL,
        BitXorEq, // BITXOREQUAL,
        BitOrEq, // BITOREQUAL,
        ScpStart,  // SCOPESTART,
        ScpEnd, // SCOPEEND,
        Pwr, // POWER,
        PwrEq,// POWEREQUAL,
        TernQuest, // TERNQUESTION,
        null // TERNCOLON
};

static int operatorPrecedence[] = {
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

//TBD the following 3 functions are just useless stubs at the moment
value ScpStart(sym *left, sym *right) {

    value lval = get_value(left);
    return lval;
}

value ScpEnd(sym *left, sym *right) {

    value lval = get_value(left);
    return lval;
}

value TernQuest(sym *left, sym *right) {

    value lval = get_value(left);
    return lval;

}

value PPPre(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival + 1;
    } else {
        fprintf(sym_file, "Error operator ++ must have int lvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value PPPost(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival + 1;
    } else {
        fprintf(sym_file, "Error operator ++ must have int lvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value MMPre(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival - 1;
    } else {
        fprintf(sym_file, "Error operator -- must have int lvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value MMPost(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival - 1;
    } else {
        fprintf(sym_file, "Error operator -- must have int lvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value UPlus(sym *left, sym *right) {
    value val = get_value(left);
    return val;
}

value UMinus(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival * -1;
    } else {
        val.dval = lval.dval * -1;
    }

    return val;
}

value Negate(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    val.flag = true;
    val.ival = lval.ival == 0 ? 1 : 0;

    return val;
}

value Mult(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival * rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.dval = lval.ival * rval.dval;
    } else if (!lval.flag && rval.flag) {
        val.dval = lval.dval * rval.ival;
    } else {
        val.dval = lval.dval * rval.dval;
    }

    return val;
}

value Div(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if (rval.flag && rval.ival == 0 || !rval.flag && rval.dval == 0.0) {
        fprintf(sym_file, "Error Div by 0\n");
        stop(tab, sym_file);
    }

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival / rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.dval = lval.dval / rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.dval = lval.ival / rval.dval;
    } else {
        val.dval = lval.dval / rval.dval;
    }

    return val;
}

value Rem(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival % rval.ival;
    } else {
        fprintf(sym_file, "Error operator % requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value BPlus(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival + rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.dval = lval.dval + rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.dval = lval.ival + rval.dval;
    } else {
        val.dval = lval.dval + rval.dval;
    }

    return val;
}

value BMinus(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival - rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.dval = lval.dval - rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.dval = lval.ival - rval.dval;
    } else {
        val.dval = lval.dval - rval.dval;
    }

    return val;
}

value LShift(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);
    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival << rval.ival;
    } else {
        fprintf(sym_file, "Error operator << requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value RShift(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival >> rval.ival;
    } else {
        fprintf(sym_file, "Error operator >> requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value LessThan(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    if (lval.flag && rval.flag) {
        val.ival = lval.ival < rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.ival = lval.dval < rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.ival = lval.ival < rval.dval;
    } else {
        val.ival = lval.dval < rval.dval;
    }

    return val;
}

value LessThanEq(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    if (lval.flag && rval.flag) {
        val.ival = lval.ival <= rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.ival = lval.dval <= rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.ival = lval.ival <= rval.dval;
    } else {
        val.ival = lval.dval <= rval.dval;
    }

    return val;
}

value GreaterThan(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    if (lval.flag && rval.flag) {
        val.ival = lval.ival > rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.ival = lval.dval > rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.ival = lval.ival > rval.dval;
    } else {
        val.ival = lval.dval > rval.dval;
    }

    return val;
}

value GreaterThanEq(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    if (lval.flag && rval.flag) {
        val.ival = lval.ival >= rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.ival = lval.dval >= rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.ival = lval.ival >= rval.dval;
    } else {
        val.ival = lval.dval >= rval.dval;
    }

    return val;
}

value EqualTo(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    if (lval.flag && rval.flag) {
        val.ival = lval.ival == rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.ival = lval.dval == rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.ival = lval.ival == rval.dval;
    } else {
        val.ival = lval.dval == rval.dval;
    }

    return val;
}

value NotEQ(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    if (lval.flag && rval.flag) {
        val.ival = lval.ival != rval.ival;
    } else if (!lval.flag && rval.flag) {
        val.ival = lval.dval != rval.ival;
    } else if (lval.flag && !rval.flag) {
        val.ival = lval.ival != rval.dval;
    } else {
        val.ival = lval.dval != rval.dval;
    }

    return val;
}

value BitAnd(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival & rval.ival;
    } else {
        fprintf(sym_file, "Error operator & requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value BitXor(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival ^ rval.ival;
    } else {
        fprintf(sym_file, "Error operator ^ requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value BitOr(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival | rval.ival;
    } else {
        fprintf(sym_file, "Error operator | requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    return val;
}

value LogAnd(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    val.ival = lval.ival && rval.ival;
    return val;
}

value LogOr(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    val.ival = lval.ival || rval.ival;
    return val;
}

value Assign(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = rval.ival;
    } else if (!lval.flag && rval.flag) {
        lval.dval = rval.ival;
    } else if (lval.flag && !rval.flag) {
        lval.ival = (long long int) rval.dval;
    } else {
        lval.dval = rval.dval;
    }

    insert_sym(tab, get_id(left), lval);
    return lval;
}

value PlusEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival += rval.ival;
    } else if (!lval.flag && rval.flag) {
        lval.dval += rval.ival;
    } else if (lval.flag && !rval.flag) {
        lval.ival += rval.dval;
    } else {
        lval.dval += rval.dval;
    }

    insert_sym(tab, get_id(left), lval);
    return lval;
}

value MinEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival -= rval.ival;
    } else if (!lval.flag && rval.flag) {
        lval.dval -= rval.ival;
    } else if (lval.flag && !rval.flag) {
        lval.ival -= rval.dval;
    } else {
        lval.dval -= rval.dval;
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value MulEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival *= rval.ival;
    } else if (!lval.flag && rval.flag) {
        lval.dval *= rval.ival;
    } else if (lval.flag && !rval.flag) {
        lval.ival *= rval.dval;
    } else {
        lval.dval *= rval.dval;
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}


value DivEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (rval.flag && rval.ival == 0 || !rval.flag && rval.dval == 0.0) {
        fprintf(sym_file, "Error Div by 0\n");
        stop(tab, sym_file);
    }

    if (lval.flag && rval.flag) {
        lval.ival /= rval.ival;
    } else if (!lval.flag && rval.flag) {
        lval.dval /= rval.ival;
    } else if (lval.flag && !rval.flag) {
        lval.ival /= rval.dval;
    } else {
        lval.dval /= rval.dval;
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value RemEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival % rval.ival;
    } else {
        fprintf(sym_file, "Error operator % requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value LShiftEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);
    if (lval.flag && rval.flag) {
        lval.ival = lval.ival << rval.ival;
    } else {
        fprintf(sym_file, "Error operator <<= requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value RShiftEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);
    if (lval.flag && rval.flag) {
        lval.ival = lval.ival >> rval.ival;
    } else {
        fprintf(sym_file, "Error operator >>= requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value BitAndEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival & rval.ival;
    } else {
        fprintf(sym_file, "Error operator &= requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value BitXorEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival ^ rval.ival;
    } else {
        fprintf(sym_file, "Error operator ^= requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value BitOrEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival | rval.ival;
    } else {
        fprintf(sym_file, "Error operator |= requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}

value Pwr(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = (long long int) pow((double) lval.ival, (double) rval.ival);
    } else if (lval.flag && !rval.flag) {
        val.dval = pow((double) lval.ival, rval.dval);
    } else if (!lval.flag && rval.flag) {
        val.dval = pow(lval.dval, (double) rval.ival);
    } else {
        val.dval = pow(lval.dval, rval.dval);
    }

    return val;
}

value PwrEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = (long long int) pow((double) lval.ival, (double) rval.ival);
    } else if (lval.flag && !rval.flag) {
        lval.dval = pow((double) lval.ival, rval.dval);
    } else if (!lval.flag && rval.flag) {
        lval.dval = pow(lval.dval, (double) rval.ival);
    } else {
        lval.dval = pow(lval.dval, rval.dval);
    }
    insert_sym(tab, get_id(left), lval);

    return lval;
}
