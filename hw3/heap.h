#ifndef HEAP
#define HEAP

#include "defs.h"
#include "fraction.h"

typedef struct {
    size_t max_fs;
    size_t num_fs;
    fraction **fs;
} heap;

// initialize the heap into the given pointer
extern heap *h_init();

// insert a fraction into the heap, return true if successful
extern bool h_push(heap *h, fraction *f);

// pop the top of the heap into the given pointer and remove it from the heap
extern fraction * h_pop(heap *h);

#endif // HEAP
