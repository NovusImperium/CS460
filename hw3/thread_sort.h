#ifndef THREAD_SORT
#define THREAD_SORT

#include <pthread.h>
#include "threadpool.h"
#include "fraction.h"
#include "farray.h"
#include "heap.h"

typedef struct {
    size_t lo;
    size_t mid;
    size_t hi;
    farr *fa;
} msg;

// sort function to handle sorting and merging subarrays, returns sorted fraction array
//extern void *th_sort(void *th_msg);

// merge pre-sorted subarrays
extern void *th_merge(void *arg);

// heap sort and merge two subarrays
extern void *heap_merge(void *arg);

// heap sort for arrays of size less than 1000, return heap
extern void *heap_sort(void *tm_msg);

// generate msg for the number of thread jobs to perform
extern msg *mk_jobs(msg *m, int js);

// merge the thread jobs so that there are half as many and no two jobs overlap
extern msg *merge_jobs(msg *ms, int js);

#endif // THREAD_SORT