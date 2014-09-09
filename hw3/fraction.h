#ifndef FRACTION
#define FRACTION

#include <stdio.h>
#include "defs.h"

typedef struct {
    int n;
    int d;
} fraction;

// size of a pointer to a fraction
static const size_t f_ptr = sizeof(fraction *);
// size of a fraction struct
static const size_t f_size = sizeof(fraction);

// reduce a fraction into lowest terms
extern void f_reduce(fraction *f);

// add the right-hand fraction into the left-hand fraction
extern void f_add(fraction *lhs, fraction *rhs);

// compare the two fractions and return true if lhs is less-than rhs
extern bool f_lt(const fraction *lhs, const fraction *rhs);

// initialize and read a fraction from the input file
extern bool f_read(fraction *f, FILE *in);

// print the fraction to the output file
extern void f_print(fraction *f, FILE *out);

#endif // FRACTION
