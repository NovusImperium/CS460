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

// copy the old heap into a new heap of size 's', init a new heap if 'h' is null
extern heap *h_copy(heap *h, size_t s);

// insert a fraction into the heap, return true if successful
extern bool h_push(heap *h, fraction *f);

// peek the top of the heap
extern fraction *h_peek(heap *h);

// pop the top of the heap into the given pointer and remove it from the heap
extern fraction *h_pop(heap *h);

// free the memory used directly by the heap
extern void h_free(heap *h);

#endif // HEAP
