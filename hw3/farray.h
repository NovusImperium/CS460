#ifndef FARRAY
#define FARRAY

#include "defs.h"
#include "fraction.h"

typedef struct {
    size_t max_fs;
    size_t num_fs;
    fraction **fs;
} farray;

// initialize the heap into the given pointer
extern farray *fa_init();

// insert a fraction into the heap, return true if successful
extern bool fa_push(farray *fa, fraction *f);

// print all the fractions in the farray
extern void fa_print(farray *fa, FILE *out);

// sort the fraction array
extern void fa_sort(farray *fa);

#endif // FARRAY