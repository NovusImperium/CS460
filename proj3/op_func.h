#include "sym.h"

#ifndef OP_FUNC
#define OP_FUNC

typedef sym *(*op_func)(sym *l, sym *r);

extern op_func op_funcs[];

extern int operatorPrecedence[];

extern bool op_func_is_binary[];

extern bool op_func_is_LtoR[];

sym *PPPre(sym *left, sym *right);

sym *PPPost(sym *left, sym *right);

sym *MMPost(sym *left, sym *right);

sym *MMPre(sym *left, sym *right);

sym *UPlus(sym *left, sym *right);

sym *UMinus(sym *left, sym *right);

sym *Negate(sym *left, sym *right);

sym *Mult(sym *left, sym *right);

sym *Div(sym *left, sym *right);

sym *Rem(sym *left, sym *right);

sym *BPlus(sym *left, sym *right);

sym *BMinus(sym *left, sym *right);

sym *LShift(sym *left, sym *right);

sym *RShift(sym *left, sym *right);

sym *LessThan(sym *left, sym *right);

sym *LessThanEq(sym *left, sym *right);

sym *GreaterThan(sym *left, sym *right);

sym *GreaterThanEq(sym *left, sym *right);

sym *EqualTo(sym *left, sym *right);

sym *NotEQ(sym *left, sym *right);

sym *BitAnd(sym *left, sym *right);

sym *BitXor(sym *left, sym *right);

sym *BitOr(sym *left, sym *right);

sym *LogAnd(sym *left, sym *right);

sym *LogOr(sym *left, sym *right);

sym *Assign(sym *left, sym *right);

sym *PlusEq(sym *left, sym *right);

sym *MinEq(sym *left, sym *right);

sym *MulEq(sym *left, sym *right);

sym *DivEq(sym *left, sym *right);

sym *RemEq(sym *left, sym *right);

sym *LShiftEq(sym *left, sym *right);

sym *RShiftEq(sym *left, sym *right);

sym *BitAndEq(sym *left, sym *right);

sym *BitXorEq(sym *left, sym *right);

sym *BitOrEq(sym *left, sym *right);

//TBD the following 3 functions are just useless stubs at the moment
sym *ScpStart(sym *left, sym *right);

sym *ScpEnd(sym *left, sym *right);

sym *Pwr(sym *left, sym *right);

sym *PwrEq(sym *left, sym *right);

sym *TernQuest(sym *left, sym *right);

#endif  // OP_FUNC
