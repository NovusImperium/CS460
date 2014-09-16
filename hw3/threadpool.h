#ifndef THREADPOOL
#define THREADPOOL

#include <pthread.h>
#include "defs.h"

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
    void *(*func)(void *);
    void *arg;
    void *next;
} tp_task;

// struct to store the threadpool in
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *ts;
    tp_task *qhead;
    tp_task *qtail;
    size_t num_ts;
    size_t count;
    size_t started;
    bool shutdown;
} tpool;

// initialize the threadpool with num_ts threads and a queue of initial size q_size; return null on failure
extern tpool *tp_init(size_t num_ts);

// add a function to the task pool,
// returns a struct containing a pointer to the return value and a semaphore that gets signalled when the ret is ready
// errors are set in the retval struct, err = 0 indicates no error
extern int th_add(tpool *pool, void *(*func)(void *), void *arg);

// destroy the given threadpool in the manner determined by the flag (sflags_tp)
extern int tp_dest(tpool *pool, int flags);

#endif
