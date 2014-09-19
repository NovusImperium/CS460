#include <stdlib.h>
#include <pthread.h>

#include "threadpool.h"

// worker thread for the threadpool
static void *tp_thread(void *arg);

// sentinel thread for watching for an empty task queue
static void *tp_sentinel(void *pool);

int tp_free(threadpool *pool);

threadpool *tp_init(size_t num_ts) {
    threadpool *pool;
    if ((pool = (threadpool *)malloc(sizeof(threadpool))) == null) {
        goto err;
    }

    // init the pool
    pool->num_ts = 0;
    pool->shutdown = false;
    pool->started = 0;
    pool->tasks = h_init(tp_taskcomp);

    pool->ts = malloc(sizeof(pthread_t) * num_ts);

    // setup mutex and conditional notification
    if ((pthread_mutex_init(&pool->qlock, null) != 0) ||
        (pthread_mutex_init(&pool->empty, null) != 0) ||
        (pthread_cond_init(&pool->notify, null) != 0) ||
        (pool->ts == null)) {
        goto err;
    }

    // spin up worker threads
    size_t i;
    for (i = 0; i < num_ts; i++) {
        if (pthread_create(&pool->ts[i], null, tp_thread, (void *) pool) != 0) {
            tp_dest(pool, 0);
            return null;
        }
        pool->num_ts++;
        pool->started++;
    }

    pthread_create(&pool->sentinel, null, tp_sentinel, (void *)pool);

    return pool;

 err:
    // initialization has failed somewhere, cleanup and return null
    if (pool) {
        tp_free(pool);
    }
    return null;
}

int tp_add(threadpool *pool, void *(*func)(void *), void *arg, int priority) {
    int err = 0;

    if (pool == null || func == null) {
        return tp_invalid;
    }

    // wait for lock on the queue, quit if lock fails
    if (pthread_mutex_lock(&pool->qlock) != 0) {
        return tp_lockfail;
    }

    do {
        // check for shutdown
        if (pool->shutdown) {
            err = tp_shutdown;
            break;
        }

        tp_task *task = malloc(sizeof(tp_task));
        task->func = func;
        task->arg = arg;
        task->pty = priority;

        h_push(pool->tasks, task);

        // send notification that the queue is unlocking
        if (pthread_cond_signal(&pool->notify) != 0) {
            err = tp_lockfail;
            break;
        }
    } while (0);

    if (pthread_mutex_unlock(&pool->qlock) != 0) {
        err = tp_lockfail;
    }

    return err;
}

int tp_dest(threadpool *pool, int flags) {
    int i, err = 0;

    if (pool == null) {
        return tp_invalid;
    }

    if (pthread_mutex_lock(&pool->qlock) != 0) {
        return tp_lockfail;
    }

    do {
        // check that we're not shutting down already
        if (pool->shutdown) {
            err = tp_shutdown;
            break;
        }

        pool->shutdown = (flags & tpexit_graceful) ?
                tpsdown_soft : tpsdown_now;

        // wakeup worker threads
        if ((pthread_cond_broadcast(&pool->notify) != 0) ||
           (pthread_mutex_unlock(&pool->qlock) != 0)) {
            err = tp_lockfail;
            break;
        }

        // recall worker threads
        for (i = 0; i < pool->num_ts; i++) {
            if (pthread_join(pool->ts[i], null) != 0) {
                err = tp_threadfail;
            }
        }
    } while (0);

    // don't free if we hit an error
    if (!err) {
        tp_free(pool);
    }
    return err;
}

int tp_free(threadpool *pool) {
    if (pool == null || pool->started > 0) {
        return -1;
    }

    // release threadpool if it has been created
    if (pool->ts) {
        free(pool->ts);

        // free the task queue if it has been initialized
        if (pool->tasks != null) {
            tp_task *t;
            while ((t = h_pop(pool->tasks)) != null) {
                free(t);
            }

            h_free(pool->tasks);
        }

        pthread_mutex_lock(&pool->qlock);
        pthread_mutex_lock(&pool->empty);
        pthread_mutex_destroy(&pool->qlock);
        pthread_mutex_destroy(&pool->empty);
        pthread_cond_destroy(&pool->notify);
    }
    free(pool);    
    return 0;
}

static void *tp_sentinel(void *arg) {
    threadpool *pool = (threadpool *) arg;

    // spinning up the sentinel thread, assume the queue is non-empty
    pthread_mutex_lock(&pool->empty);
    while (true) {
        // wait for the queue to unlock
        pthread_mutex_lock(&pool->qlock);

        if (pool->tasks->n == 0) {
            // queue is empty, release the lock to signal such
            pthread_mutex_unlock(&pool->empty);

            // wait on condition variable, check for spurious wakeups
            // we own the lock when returning from pthread_cond_wait
            while (pool->tasks->n == 0 || !pool->shutdown) {
                pthread_cond_wait(&pool->notify, &pool->qlock);
            }

            // queue now guaranteed to be non-empty, lock the signal
            pthread_mutex_lock(&pool->empty);
        }

        // exit thread loop if the shutdown signal is set
        if (pool->shutdown) {
            break;
        }

        // allow another thread to access the queue
        pthread_mutex_unlock(&pool->qlock);
    }

    // cleanup and exit thread
    pthread_mutex_unlock(&pool->qlock);
    pthread_mutex_unlock(&pool->empty);
    pthread_exit(null);
}

static void *tp_thread(void *arg) {
    threadpool *pool = ((threadpool*) arg);
    tp_task task;

    while (true) {
        // wait for the queue to be free and lock while getting task
        pthread_mutex_lock(&pool->qlock);

        // wait on condition variable, check for spurious wakeups
        // we own the lock when returning from pthread_cond_wait
        while (pool->tasks->n == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notify, &pool->qlock);
        }

        if ((pool->shutdown == tpsdown_now) ||
           ((pool->shutdown == tpsdown_soft) &&
            (pool->tasks->n == 0))) {
            break;
        }

        // get the task from the queue
        task = *(tp_task*)h_pop(pool->tasks);

        // release the lock on the queue
        pthread_mutex_unlock(&pool->qlock);

        // run the task
        (*(task.func))(task.arg);
    }

    pool->started--;

    pthread_mutex_unlock(&pool->qlock);
    pthread_exit(null);
}

void tp_wait(threadpool *pool) {
    while (true) {
        pthread_mutex_lock(&pool->empty);
        pthread_mutex_lock(&pool->qlock);
        if (pool->tasks->n == 0 || pool->shutdown) {
            pthread_mutex_unlock(&pool->qlock);
            return;
        }
        pthread_mutex_unlock(&pool->qlock);
        pthread_mutex_unlock(&pool->empty);
    }
}

bool tp_taskcomp(void const *lhs, void const *rhs) {
    tp_task *l = (tp_task*)lhs;
    tp_task *r = (tp_task*)rhs;

    return l->pty > r->pty;
}