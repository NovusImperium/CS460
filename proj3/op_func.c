#include "op_func.h"
#include "syn.h"
#include "defs.h"
#include <math.h>

extern FILE *sym_file;
extern FILE *dbg_file;
extern table *tab;
extern token_t currentType;

bool op_func_is_binary[] = {
        false, // POSTPP,
        false, // POSTMM,
        false, // PREPP,
        false, // PREMM,
        false, // UPLUS,
        false, // UMINUS,
        false, // NEGATION,
        true, // MULTIPLY,
        true, // DIVIDE,
        true, // REMAINDER,
        true, // BPLUS,
        true, // BMINUS,
        true, // SHIFTLEFT,
        true, // SHIFTRIGHT,
        true, // LESSTHAN,
        true, // LESSTHANEQ,
        true, // GREATERTHAN,
        true, // GREATERTHANEQ,
        true, // EQUAL,
        true, // NOTEQUAL,
        true, // BITAND,
        true, // BITXOR,
        true, // BITOR,
        true, // LOGICALAND,
        true, // LOGICALOR,
        true, // ASSIGNMENT,
        true, // PLUSEQUAL,
        true, // MINUSEQUAL,
        true, // MULTEQUAL,
        true, // DIVEQUAL,
        true, // REMEQUAL,
        true, // SHIFTLEFTEQUAL,
        true, // SHIFTRIGHTEQUAL,
        true, // BITANDEQUAL,
        true, // BITXOREQUAL,
        true, // BITOREQUAL,
        false, // SCOPESTART,
        false, // SCOPEEND,
        true, // POWER,
        true, // POWEREQUAL,
        false, // TERNQUESTION,
        false // TERNCOLON
};

bool op_func_is_LtoR[] = {
        true, // POSTPP,
        true, // POSTMM,
        false, // PREPP,
        false, // PREMM,
        false, // UPLUS,
        false, // UMINUS,
        false, // NEGATION,
        true, // MULTIPLY,
        true, // DIVIDE,
        true, // REMAINDER,
        true, // BPLUS,
        true, // BMINUS,
        true, // SHIFTLEFT,
        true, // SHIFTRIGHT,
        true, // LESSTHAN,
        true, // LESSTHANEQ,
        true, // GREATERTHAN,
        true, // GREATERTHANEQ,
        true, // EQUAL,
        true, // NOTEQUAL,
        true, // BITAND,
        true, // BITXOR,
        true, // BITOR,
        true, // LOGICALAND,
        true, // LOGICALOR,
        false, // ASSIGNMENT,
        false, // PLUSEQUAL,
        false, // MINUSEQUAL,
        false, // MULTEQUAL,
        false, // DIVEQUAL,
        false, // REMEQUAL,
        false, // SHIFTLEFTEQUAL,
        false, // SHIFTRIGHTEQUAL,
        false, // BITANDEQUAL,
        false, // BITXOREQUAL,
        false, // BITOREQUAL,
        false, // SCOPESTART,
        false, // SCOPEEND,
        true, // POWER,
        true, // POWEREQUAL,
        false, // TERNQUESTION,
        false // TERNCOLON
};

int operatorPrecedence[] = {
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
        0,  // SCOPESTART,
        0, // SCOPEEND,
        4, // POWER,
        15,// POWEREQUAL,
        15, // TERNQUESTION,
        15 // TERNCOLON
};

op_func op_funcs[] = {
        PPPost, // POSTPP,
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
        NULL // TERNCOLON
};

//TBD the following 3 functions are just useless stubs at the moment
sym *ScpStart(sym *left, sym *right) {
    return left;
}

sym *ScpEnd(sym *left, sym *right) {
    return left;
}

sym *TernQuest(sym *left, sym *right) {
    return left;
}

sym *PPPre(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival + 1;
    } else {
        fprintf(sym_file, "Error operator ++ must have int lvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, val)) {
        stop(tab, sym_file);
    }
    return left;
}

sym *PPPost(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival + 1;
    } else {
        fprintf(sym_file, "Error operator ++ must have int lvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *MMPre(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival - 1;
    } else {
        fprintf(sym_file, "Error operator -- must have int lvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, val)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *MMPost(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival - 1;
    } else {
        fprintf(sym_file, "Error operator -- must have int lvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}


sym *UPlus(sym *left, sym *right) {
    return left;
}

sym *UMinus(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    if ((val.flag = lval.flag)) {
        val.ival = lval.ival * -1;
    } else {
        val.dval = lval.dval * -1;
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *Negate(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    val.flag = true;
    val.ival = lval.ival == 0 ? 1 : 0;

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *Mult(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *Div(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *Rem(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival % rval.ival;
    } else {
        fprintf(sym_file, "Error operator % requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *BPlus(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *BMinus(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *LShift(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);
    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival << rval.ival;
    } else {
        fprintf(sym_file, "Error operator << requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *RShift(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival >> rval.ival;
    } else {
        fprintf(sym_file, "Error operator >> requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *LessThan(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *LessThanEq(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *GreaterThan(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *GreaterThanEq(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *EqualTo(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *NotEQ(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *BitAnd(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival & rval.ival;
    } else {
        fprintf(sym_file, "Error operator & requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *BitXor(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival ^ rval.ival;
    } else {
        fprintf(sym_file, "Error operator ^ requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *BitOr(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    if ((val.flag = lval.flag && rval.flag)) {
        val.ival = lval.ival | rval.ival;
    } else {
        fprintf(sym_file, "Error operator | requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *LogAnd(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    val.ival = lval.ival && rval.ival;

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *LogOr(sym *left, sym *right) {
    value val;
    value lval = get_value(left);
    value rval = get_value(right);

    val.flag = true;
    val.ival = lval.ival || rval.ival;

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *Assign(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag) {
        lval.ival = rval.flag ? rval.ival : (long long) rval.dval;
    } else {
        lval.dval = rval.flag ? (double) rval.ival : rval.dval;
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *PlusEq(sym *left, sym *right) {
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

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *MinEq(sym *left, sym *right) {
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

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *MulEq(sym *left, sym *right) {
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

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}


sym *DivEq(sym *left, sym *right) {
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

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *RemEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival % rval.ival;
    } else {
        fprintf(sym_file, "Error operator % requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *LShiftEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);
    if (lval.flag && rval.flag) {
        lval.ival = lval.ival << rval.ival;
    } else {
        fprintf(sym_file, "Error operator <<= requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *RShiftEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);
    if (lval.flag && rval.flag) {
        lval.ival = lval.ival >> rval.ival;
    } else {
        fprintf(sym_file, "Error operator >>= requires int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *BitAndEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival & rval.ival;
    } else {
        fprintf(sym_file, "Error operator &= requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *BitXorEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival ^ rval.ival;
    } else {
        fprintf(sym_file, "Error operator ^= requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *BitOrEq(sym *left, sym *right) {
    value lval = get_value(left);
    value rval = get_value(right);

    if (lval.flag && rval.flag) {
        lval.ival = lval.ival | rval.ival;
    } else {
        fprintf(sym_file, "Error operator |= requires an int lvalue and int rvalue\n");
        stop(tab, sym_file);
    }

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}

sym *Pwr(sym *left, sym *right) {
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

    optional opt;
    if (!(opt = create_temp(tab, val)).e) {
        stop(tab, sym_file);
    }
    return opt.val;
}

sym *PwrEq(sym *left, sym *right) {
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

    if (!update_sym(tab, left, lval)) {
        stop(tab, sym_file);
    }

    return left;
}
