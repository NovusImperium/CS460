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
        fprintf(dbg_file, "Error at line %d position %d Redeclaration of %s\n", get_linenum(), get_position(), L);
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
        fprintf(dbg_file, "Error at line %d position %d Use of undeclared variable %s\n", get_linenum(), get_position(), var);
        stop(tab, sym_file);
    }
}

void OperatorFound(OpCode_type op){

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
