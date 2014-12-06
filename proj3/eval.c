#include "eval.h"
#include "syn.h"
#include <cmath>

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

void OperatorFound(OpCode_type op){

  if(sizeof(operators) == 0)
    arr_push(operators, (void*)op);
}

void InitSymantic(void) {
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

value PPPre(sym *left, sym *right){

  value val;
  value lval = get_value(left);;
  if(val.flag = lval.flag){
    val.ival = lval.ival + 1;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator ++ must have int lvalue\n");
    stop(tab, sym_file);
  }
}

value PPPost(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  if(val.flag = lval.flag){
    val.ival = lval.ival + 1;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator ++ must have int lvalue\n");
    stop(tab, sym_file);
  }
}

value MMPre(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  if(val.flag = lval.flag){
    val.ival = lval.ival - 1;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator -- must have int lvalue\n");
    stop(tab, sym_file);
  }
}

value MMPost(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  if(val.flag = lval.flag){
    val.ival = lval.ival - 1;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator -- must have int lvalue\n");
    stop(tab, sym_file);
  }
}



value UPlus(sym *left, sym *right){
  
  value val = get_value(left);
  return val;

}

value UMinus(sym *left, sym *right){

  value val;
  vlaue lval = get_value(left);
  if(val.flag = left.flag){
    val.ival = lval.ival * -1;
    return val;
  }
  else{
    val.dval = lval.dval * -1;
    return val;
  }
}

value Negate(sym *left, sym *right){
  
  value val;
  value lval = get_value(left);
  val.flag = true;
  val.ival = lval.ival == 0 ? 1 : 0;
  return val;
}

value Mult(sym *left, sym *right){
  
  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival * rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.dval = lval.ival * rval.dval;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.dval = lval.dval * rval.ival;
    return val;
  }
  else{
    val.dval = lval.dval * rval.dval;
    return val;
  }
}

value Div(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(rval.flag && rval.ival == 0 || !rval.flag && rval.dval == 0.0){
    fprintf(sym_file, "Error Div by 0\n");
    stop(tab, sym_file);
  }

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival/rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.dval = lval.dval/ival;
    return val;
  }
  else if(lval.flag && ! rval.flag){
    val.dval = lval.ival/rval.dval;
    return val;
  }
  else{
    val.dval = lval.dval/rval.dval;
    return val;
  }
}

value Rem(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival%rval.ival;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator % requires int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value BPlus(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival + rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.dval = lval.dval + rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.dval = lval.ival + rval.dval;
    return val;
  }
  else{
    val.dval = lval.dval + rval.dval;
    return val;
  }
}

value BMinus(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value right = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival - rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.dval = lval.dval - rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.dval = lval.ival - rval.dval;
    return val;
  }
  else{
    val.dval = lval.dval - rval.dval;
    return val;
  }
}

value LShift(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);
  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival << rval.ival;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator << requires int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value RShift(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value right = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival >> rval.ival;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator >> requires int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value LT(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  vlaue rval = get_value(right);

  val.flag = true;
  if(lval.flag && rval.flag){
    val.ival = lval.ival < rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.ivalue = lval.dval < rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.ival = lval.ival < rval.dval;
    return val;
  }
  else{
    val.ival = lval.dval < rval.dval;
    return val;
  }
}

value LTE(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  if(lval.flag && rval.flag){
    val.ival = lval.ival <= rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.ivalue = lval.dval <= rval.ival;
    return val;
  }
  else if(lval.flag && ! rval.flag){
    val.ival = lval.ival <= rval.dval;
    return val;
  }
  else{
    val.ival = lval.dval <= rval.dval;
    return val;
  }
}

value GT(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  if(lval.flag && rval.flag){
    val.ival = lval.ival > rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.ivalue = lval.dval > rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.ival = lval.ival > rval.dval;
    return val;
  }
  else{
    val.ival = lval.dval > rval.dval;
    return val;
  }
}

value GTE(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  if(lval.flag && rval.flag){
    val.ival = lval.ival >= rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.ivalue = lval.dval >= rval.ival;
    return val;
  }
  else if(lval.flag && ! rval.flag){
    val.ival = lval.ival >= rval.dval;
    return val;
  }
  else{
    val.ival = lval.dval >= rval.dval;
    return val;
  }
}

value EqualTo(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  if(lval.flag && rval.flag){
    val.ival = lval.ival == rval.ival;
    return val
  }
  else if(!lval.flag && rval.flag){
    val.ivalue = lval.dval == rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.ival = lval.ival == rval.dval;
    return val;
  }
  else{
    val.ival = lval.dval == rval.dval;
    return val;
  }
}

value NotEQ(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  if(lval.flag && rval.flag){
    val.ival = lval.ival != rval.ival;
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.ivalue = lval.dval != rval.ival;
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.ival = lval.ival != rval.dval;
    return val;
  }
  else{
    val.ival = lval.dval != rval.dval;
    return val;
  }
}

value BitAnd(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival & rval.ival;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator & requires an int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value BitXor(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival ^ rval.ival;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator ^ requires an int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value BitOr(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = lval.ival | rval.ival;
    return val;
  }
  else{
    fprintf(sym_file, "Error operator | requires an int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value LogAnd(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  val.ival = lval.ival && rval.ival;
  return val;
}

value LogOr(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  val.flag = true;
  val.ival = lval.ival || rval.ival;
  return val;
}

value Assign(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival = rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(!lval.flag && rval.flag){
    lval.dval = rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(lval.flag && !rval.flag){
    lval.ival = rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    lval.dval = rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
}

value PlusEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival += rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(!lval.flag && rval.flag){
    lval.dval += rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(lval.flag && !rval.flag){
    lval.ival += rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    lval.dval += rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
}

value MinEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival -= rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(!lval.flag && rval.flag){
    lval.dval -= rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(lval.flag && !rval.flag){
    lval.ival -= rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    lval.dval -= rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
}

value DivEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(rval.flag && rval.ival == 0 || !rval.flag && rval.dval == 0.0){
    fprintf(sym_file, "Error Div by 0\n");
    stop(tab, sym_file);
  }

  if(lval.flag && rval.flag){
    lval.ival /= rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(!lval.flag && rval.flag){
    lval.dval /= rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(lval.flag && !rval.flag){
    lval.ival /= rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    lval.dval /= rval.dval;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
}

value RemEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival = lval.ival%rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    fprintf(sym_file, "Error operator % requires int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value LShiftEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);
  if(lval.flag && rval.flag){
    lval.ival = lval.ival << rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    fprintf(sym_file, "Error operator <<= requires int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value RShiftEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);
  if(lval.flag && rval.flag){
    lval.ival = lval.ival >> rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    fprintf(sym_file, "Error operator >>= requires int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value BitAndEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival = lval.ival & rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    fprintf(sym_file, "Error operator &= requires an int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value BitXorEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival = lval.ival ^ rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    fprintf(sym_file, "Error operator ^= requires an int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value BitOrEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival = lval.ival | rval.ival;
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    fprintf(sym_file, "Error operator |= requires an int lvalue and int rvalue\n");
    stop(tab, sym_file);
  }
}

value Pwr(sym *left, sym *right){

  value val;
  value lval = get_value(left);
  value rval = get_value(right);

  if(val.flag = lval.flag && rval.flag){
    val.ival = pow(lval.ival, rval.ival);
    return val;
  }
  else if(lval.flag && !rval.flag){
    val.dval = pow(lval.ival, rval.dval);
    return val;
  }
  else if(!lval.flag && rval.flag){
    val.dval = pow(lval.dval, rval.ival);
    return val;
  }
  else{
    val.dval = pow(lval.dval, rval.dval);
    return val;
  }
}

value PwrEq(sym *left, sym *right){

  value lval = get_value(left);
  value rval = get_value(right);

  if(lval.flag && rval.flag){
    lval.ival = pow(lval.ival, rval.ival);
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(lval.flag && !rval.flag){
    lval.dval = pow(lval.ival, rval.dval);
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else if(!lval.flag && rval.flag){
    lval.dval = pow(lval.dval, rval.ival);
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
  else{
    lval.dval = pow(lval.dval, rval.dval);
    insert_sym(tab, get_id(left), lval);
    return lval;
  }
}

//The code below is borrowed from http://www.strchr.com/expression_evaluator
//I am using it to get the flow of how the evaluator should work.

/*
double ParseAtom(char*& expr) {
  // Read the number from string
  char* end_ptr;
  double res = strtod(expr, &end_ptr);
  // Advance the pointer and return the result
  expr = end_ptr;
  return res;
}

// Parse multiplication and division
double ParseFactors(char*& expr) {
  double num1 = ParseAtom(expr);
  for(;;) {
    // Save the operation
    char op = *expr;
    if(op != '/' && op != '*')
      return num1;
    expr++;
    double num2 = ParseAtom(expr);
    // Perform the saved operation
    if(op == '/')
      num1 /= num2;
    else
      num1 *= num2;
  }
}

// Parse addition and subtraction
double ParseSummands(char*& expr) {
  double num1 = ParseFactors(expr);
  for(;;) {
    char op = *expr;
    if(op != '-' && op != '+')
      return num1;
    expr++;
    double num2 = ParseFactors(expr);
    if(op == '-')
      num1 -= num2;
    else
      num1 += num2;
  }
}

double EvaluateTheExpression(char* expr) {
  return ParseSummands(expr);
};
*/
