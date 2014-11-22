#include <stdio.h>
#include "defs.h"
#include "array.h"

typedef struct sym_tab sym_tab;
typedef struct sym sym;

// get a symbol entry from the symbol table
// returns an optional such that:
//  opt.e == true iff the symbol has a valid entry in the table
inline optional get_sym(sym_tab *t, char* id);

// insert a symbol into the symbol table
// val.e == true => val.val == integer
// val.e == false => val.val == double
// returns true iff the symbol already exists
inline bool insert_sym(sym_tab *t, char* id, optional val);

// get the value of the symbol entry
// returns an optional such that:
//  opt.e == true => opt.val == integer
//  opt.e == false => opt.val == double
inline optional get_value(sym *s);

// get the character string that represents the id of the symbol entry
inline char *get_id(sym *s);

// writes the symbol entries in alphabetical order to the given file pointer
inline void write_syms(sym_tab *t, FILE *out);
