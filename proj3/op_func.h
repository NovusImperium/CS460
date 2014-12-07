#include "sym.h"

#ifndef OP_FUNC
#define OP_FUNC

typedef value (*op_func)(sym *l, sym *r);

extern op_func op_funcs[];

extern int operatorPrecedence[];

value PPPre(sym *left, sym *right);

value PPPost(sym *left, sym *right);

value MMPost(sym *left, sym *right);

value MMPre(sym *left, sym *right);

value UPlus(sym *left, sym *right);

value UMinus(sym *left, sym *right);

value Negate(sym *left, sym *right);

value Mult(sym *left, sym *right);

value Div(sym *left, sym *right);

value Rem(sym *left, sym *right);

value BPlus(sym *left, sym *right);

value BMinus(sym *left, sym *right);

value LShift(sym *left, sym *right);

value RShift(sym *left, sym *right);

value LessThan(sym *left, sym *right);

value LessThanEq(sym *left, sym *right);

value GreaterThan(sym *left, sym *right);

value GreaterThanEq(sym *left, sym *right);

value EqualTo(sym *left, sym *right);

value NotEQ(sym *left, sym *right);

value BitAnd(sym *left, sym *right);

value BitXor(sym *left, sym *right);

value BitOr(sym *left, sym *right);

value LogAnd(sym *left, sym *right);

value LogOr(sym *left, sym *right);

value Assign(sym *left, sym *right);

value PlusEq(sym *left, sym *right);

value MinEq(sym *left, sym *right);

value MulEq(sym *left, sym *right);

value DivEq(sym *left, sym *right);

value RemEq(sym *left, sym *right);

value LShiftEq(sym *left, sym *right);

value RShiftEq(sym *left, sym *right);

value BitAndEq(sym *left, sym *right);

value BitXorEq(sym *left, sym *right);

value BitOrEq(sym *left, sym *right);

//TBD the following 3 functions are just useless stubs at the moment
value ScpStart(sym *left, sym *right);

value ScpEnd(sym *left, sym *right);

value Pwr(sym *left, sym *right);

value PwrEq(sym *left, sym *right);

value TernQuest(sym *left, sym *right);

#endif  // OP_FUNC