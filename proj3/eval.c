#include "eval.h"
#include <stdlib.h>

using namespace std;


extern FILE * sym;


void SaveDeclType(token_t T){


}

void NewDeclaration(char *L){


}

void NumLitFound(char *N){


}

void VariableFound(char*N){


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
