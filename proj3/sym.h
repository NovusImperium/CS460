#include <stdio.h>
#include "defs.h"
#include "array.h"

typedef struct hashmap table;
typedef struct sym sym;

// creates and initializes the symbol table
// returns an optional such that
//  if opt.e == true => opt.val is valid
extern inline optional init_sym();

// get a symbol entry from the symbol table
// returns an optional such that:
//  opt.e == true iff the symbol has a valid entry in the table
extern inline optional get_sym(table *t, char *id);

// insert a symbol into the symbol table
// val.e == true => *val.val == integer
// val.e == false => *val.val == double
// returns true iff the symbol already exists
extern inline bool insert_sym(table *t, char *id, optional val);

// get the value of the symbol entry
// returns an optional such that:
//  opt.e == true => *opt.val == integer
//  opt.e == false => *opt.val == double
extern inline optional get_value(sym *s);

// get the character string that represents the id of the symbol entry
extern inline char *get_id(sym *s);

// writes the symbol entries in alphabetical order to the given file pointer
extern inline void write_syms(table *t, FILE *out);
