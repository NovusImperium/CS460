#include "defs.h"
#include "array.h"
#include <stdio.h>

typedef struct table table;
typedef struct sym sym;

// creates and initializes the symbol table
// returns an optional such that
//  if opt.e == true => opt.val is a valid symbol
extern inline optional init_sym();

// get a symbol entry from the symbol table
// returns an optional such that:
//  opt.e == true iff the symbol has a valid entry in the table
extern inline optional get_sym(table *t, char *id);

// update a symbol that already exists in the symbol table
//  val.flag == true => val.ival
//  val.flag == false => val.dval
// returns true iff the symbol already exists and the value types match
extern inline bool update_sym(sym *s, value val);

// create and insert a new symbol into the symbol table
// returns an optional value containing the created symbol if the operation was successful
extern inline optional create_sym(table *t, char *id, value val);

// create and return the a symbol for the next temp symbol
extern inline optional create_temp(table *t, value val);

// get the value of the symbol entry
//  val.flag == true => val.ival
//  val.flag == false => val.dval
// returns an optional such that:
extern inline value get_value(sym *s);

// get the character string that represents the id of the symbol entry
extern inline char *get_id(sym *s);

// writes the symbol entries in alphabetical order to the given file pointer
extern inline void write_syms(table *t, FILE *out);
