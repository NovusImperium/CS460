#ifndef HEAP
#define HEAP

#include "defs.h"

typedef struct {
    bool (*comp)(const void*,const void*);
    size_t m;
    size_t n;
    void **q;
} heap;

// initialize the heap with the given comparison function
extern heap *h_init(bool (*comp)(const void *, const void *));

// copy the old heap into a new heap of size 's', init a new heap if 'h' is null
extern heap *h_copy(heap *h, size_t s, int (*comp)(void const *, void const *));

// insert a fraction into the heap, return true if successful
extern bool h_push(heap *h, void *i);

// peek the top of the heap
extern void * h_peek(heap *h);

// pop the top of the heap into the given pointer and remove it from the heap
extern void * h_pop(heap *h);

// free the memory used directly by the heap
extern void h_free(heap *h);

#endif // HEAP
