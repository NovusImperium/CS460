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

// sort function to handle sorting and merging subarrays
extern void *th_sort(void *th_msg);

// insertion sort for arrays of size less than 100
extern void *ins_sort(void *th_msg);

#endif // THREAD_SORT