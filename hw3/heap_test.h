#ifndef HEAP
#define HEAP

#include <sys/types.h>
#include "defs.h"

typedef struct {
    size_t max_is;
    size_t num_is;
    int **is;
} heap;

// size of a pointer to a int
static const size_t i_ptr = sizeof(int *);

// initialize the heap into the given pointer
extern heap *h_init();

// insert an int into the heap, return true if successful
extern bool h_push(heap *h, int *i);

// pop the top of the heap into the given pointer and remove it from the heap
extern int *h_pop(heap *h);

// compare the two int* to determine if the lhs holds a smaller value than rhs
extern bool i_lt(int *lhs, int *rhs);

#endif // HEAP
