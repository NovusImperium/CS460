#ifndef FARRAY
#define FARRAY

#include "defs.h"
#include "fraction.h"

typedef struct {
    size_t max_fs;
    size_t num_fs;
    size_t curr;
    fraction **fs;
} farr;

// initialize the fraction array into the given pointer
extern farr *fa_init();

// create and return a new copy of the fraction array with size 's', init new fraction array if 'fa' is null
extern farr *fa_copy(farr *fa, size_t s);

// free the memory used by the fraction array
extern void fa_free(farr *fa);

// insert a fraction into the fraction array, return true if successful
extern bool fa_push(farr *fa, fraction *f);

// peek at the head of the fraction array
extern fraction *fa_peek(farr *fa);

// pop the head of the fraction array
extern fraction *fa_pop(farr *fa);

// print all the fractions in the fraction array
extern void fa_print(farr *fa, FILE *out);

// sort the fraction array
extern farr * fa_sort(farr *fa);

#endif // FARRAY