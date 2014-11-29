#include "eval.h"
#include <stdlib.h>

using namespace std;


extern FILE * sym;
extern token_t currentType;
static array * operators;
static array * operands;

bool NewDeclaration(table *t, char *L){
  
  optional opt = get_sym(t, L);
  if(opt.e){
    printf("Error at line %d position %d Redeclaration of %s\n", L, get_linenum(), get_position());
    fprintf(dbg, "Error at line %d position %d Redeclaration of %s\n", L, get_linenum(), get_position());
    stop(t, sym);
    return false;
  }
  else{
    value val;
    if(val.flag = currentType == INTTYPE){
      val.ival = 0;
      insert_sym(t, L, val);
    }
    else{
      val.dval = 0.0;
      insert_sym(t, L, val);
    }
  }
}  
void NumLitFound(char *N){
  optional opt = get_sym(t, N);
  if(opt.e){
    arr_push(operands, N);
  }
  else{
    value val;
    val.flag = true;
    char *str = N;
    char c;
    while ((c = *str++)) {
      if (c == '.') {
	val.flag = false;
	val.dval = aotf(N);
      }
    }
    if (val.flag) {
      val.ival = atoi(N);
    }
  }

}

void VariableFound(char*N){
  optional opt = get_sym(t, N);
  if(opt.e){
    arr_push(operands, N);
  }
  else{
    printf("Error at line %d position %d Use of undeclared variable %s\n", L, get_linenum(), get_position());
    fprintf(dbg, "Error at line %d position %d Use of undeclared variable %s\n", L, get_linenum(), get_position());
  }
}

void InitSymantic(void){
  
  optional opt = init_sym();
  if(!opt.e){
    fprintf(dbg, "Initialization of symbol table failed.\n");
    exit(1);
  }
  else{
    t = opt.val;
  }
  opt = arr_init(32);
  if(!opt.e){
    fprintf(dbg, "Initializiation of operator array failed.\n");
    exit(1);
  }
  else{
    operators = opt.val;
  }
  opt = arr_init(32);
  if(!opt.e){
    fprintf(dbg, "Initilization of operand array failed.\n");
    exit(1);
  }
  else{
    operands = opt.val;
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
