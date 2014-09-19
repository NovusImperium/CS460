#ifndef THREADPOOL
#define THREADPOOL

#include <pthread.h>
#include "defs.h"
#include "heap.h"

typedef enum {
    tp_invalid = -1,
    tp_lockfail = -2,
    tp_shutdown = -3,
    tp_threadfail = -4
} err_tp;

typedef enum {
    tpexit_graceful = 1
} dflags_tp;

typedef enum {
    tpsdown_now = 1,
    tpsdown_soft = 2
} sflags_tp;

// task to store in the queue
typedef struct {
    int pty;
    void *(*func)(void *);
    void *arg;
    void *next;
} tp_task;

/*
typedef struct {
    pthread_mutex_t mtx;
    bool done;
    pthread_t th;
} tp_pthread;
*/

// struct to store the threadpool in
typedef struct {
    pthread_mutex_t qlock;
    pthread_mutex_t empty;
    pthread_cond_t notify;
    pthread_t sentinel;
    pthread_t *ts;
    heap *tasks;
    size_t num_ts;
    size_t started;
    bool shutdown;
} threadpool;

/*
typedef struct {
    tp_pthread *th;
    threadpool *pool;
} tp_arg;
*/

// initialize the threadpool with num_ts threads and a queue of initial size q_size; return null on failure
extern threadpool *tp_init(size_t num_ts);

// add a function to the task pool; returns 0 on success and an err_tp on failure
// note: any return value for func is ignored
extern int tp_add(threadpool *pool, void *(*func)(void *), void *arg, int priority);

// destroy the given threadpool in the manner determined by the flag (sflags_tp)
extern int tp_dest(threadpool *pool, int flags);

// wait for the queue to empty and return
extern void tp_wait(threadpool *pool);

// comparison function for task priority
extern bool tp_taskcomp(void const *lhs, void const *rhs);

#endif
