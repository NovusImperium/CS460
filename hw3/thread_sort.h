#ifndef THREAD_SORT
#define THREAD_SORT

#include <pthread.h>
#include "fraction.h"
#include "farray.h"

typedef struct {
    size_t lo;
    size_t hi;
    fraction **fs;
} msg;

// sort function to handle sorting and merging subarrays, returns sorted fraction array
extern void *th_sort(void *th_msg);

// insertion sort for arrays of size less than 100
extern void *ins_sort(void *th_msg);

// heap sort for arrays of size less than 1000, return heap
extern void *heap_sort(void *tm_msg);

#endif // THREAD_SORT