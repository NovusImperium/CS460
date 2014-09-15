#ifndef THREAD_SORT
#define THREAD_SORT

#include <pthread.h>
#include "fraction.h"
#include "farray.h"
#include "heap.h"

typedef struct {
    size_t lo;
    size_t hi;
    fraction **fs;
} msg;

typedef struct {
    heap *l;
    heap *r;
} heap_msg;

typedef struct {
    size_t num_ts;
    pthread_t *ts;
} thread_pool;

// sort function to handle sorting and merging subarrays, returns sorted fraction array
extern void *th_sort(void *th_msg);

// insertion sort for arrays of size less than 100
extern void *ins_sort(void *th_msg);

// heap sort for arrays of size less than 1000, return heap
extern void *heap_sort(void *tm_msg);

// generate msg for the number of thread jobs to perform
extern msg *mk_jobs(msg *m, size_t js);

// merge the thread jobs so that there are half as many and no two jobs overlap
extern msg *merge_jobs(msg *ms, size_t js);

#endif // THREAD_SORT