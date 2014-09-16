#include <stdlib.h>
#include <pthread.h>

#include "threadpool.h"

// worker thread for the threadpool
static void *tp_thread(void *tp);

int tp_free(tpool *pool);

tpool *tp_init(size_t num_ts) {
    tpool *pool;
    if((pool = (tpool *)malloc(sizeof(tpool))) == null) {
        goto err;
    }

    // init the pool
    pool->num_ts = 0;
    pool->qhead = null;
    pool->qtail = null;
    pool->count = 0;
    pool->shutdown = false;
    pool->started = 0;

    pool->ts = (pthread_t *)malloc(sizeof(pthread_t) * num_ts);

    // setup mutex and conditional notification
    if((pthread_mutex_init(&(pool->lock), null) != 0) ||
       (pthread_cond_init(&(pool->notify), null) != 0) ||
       (pool->ts == null)) {
        goto err;
    }

    // spin up worker threads
    size_t i;
    for(i = 0; i < num_ts; i++) {
        if(pthread_create(&(pool->ts[i]), null,
                tp_thread, (void *) pool) != 0) {
            tp_dest(pool, 0);
            return null;
        }
        pool->num_ts++;
        pool->started++;
    }

    return pool;

 err:
    if(pool) {
        tp_free(pool);
    }
    return null;
}

int th_add(tpool *pool, void *(*func)(void *), void *arg) {
    int err = 0;

    if(pool == null || func == null) {
        return tp_invalid;
    }

    // wait for lock on the queue, quit if lock fails
    if (pthread_mutex_lock(&(pool->lock)) != 0) {
        return tp_lockfail;
    }

    do {
        // check for shutdown
        if(pool->shutdown) {
            err = tp_shutdown;
            break;
        }

        tp_task *task = malloc(sizeof(tp_task));
        task->func = func;
        task->arg = arg;
        task->next = null;

        if (pool->qtail != null) {
            pool->qtail->next = task;
            pool->qtail = task;
        } else {
            pool->qhead = pool->qtail = task;
        }

        pool->count += 1;

        // send notification
        if(pthread_cond_signal(&(pool->notify)) != 0) {
            err = tp_lockfail;
            break;
        }
    } while(0);

    if(pthread_mutex_unlock(&pool->lock) != 0) {
        err = tp_lockfail;
    }

    return err;
}

int tp_dest(tpool *pool, int flags)
{
    int i, err = 0;

    if(pool == null) {
        return tp_invalid;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return tp_lockfail;
    }

    do {
        // check that we're not shutting down already
        if(pool->shutdown) {
            err = tp_shutdown;
            break;
        }

        pool->shutdown = (flags & tpexit_graceful) ?
                tpsdown_soft : tpsdown_now;

        // wakeup worker threads
        if((pthread_cond_broadcast(&(pool->notify)) != 0) ||
           (pthread_mutex_unlock(&(pool->lock)) != 0)) {
            err = tp_lockfail;
            break;
        }

        // recall worker threads
        for(i = 0; i < pool->num_ts; i++) {
            if(pthread_join(pool->ts[i], null) != 0) {
                err = tp_threadfail;
            }
        }
    } while(0);

    // don't free if we hit an error
    if(!err) {
        tp_free(pool);
    }
    return err;
}

int tp_free(tpool *pool)
{
    if(pool == null || pool->started > 0) {
        return -1;
    }

    // release threadpool if it has been created
    if(pool->ts) {
        free(pool->ts);

        // free the remaining queue
        while (pool->qhead != null) {
            tp_task *next = pool->qhead->next;
            free(pool->qhead);
            pool->qhead = next;
        }

        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->notify));
    }
    free(pool);    
    return 0;
}


static void *tp_thread(void *tp)
{
    tpool *pool = (tpool *) tp;
    tp_task task;

    while(true) {
        // wait for the queue to be free and lock wil getting task
        pthread_mutex_lock(&(pool->lock));

        // wait on condition variable, check for spurious wakeups
        // we own the lock when returning from pthread_cond_wait
        while((pool->count == 0) && (!pool->shutdown)) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        if((pool->shutdown == tpsdown_now) ||
           ((pool->shutdown == tpsdown_soft) &&
            (pool->count == 0))) {
            break;
        }

        // get the task from the queue
        task.func = pool->qhead->func;
        task.arg = pool->qhead->arg;

        tp_task *tmp = pool->qhead;
        pool->qhead = pool->qhead->next;
        free(tmp);
        pool->count -= 1;

        // check for empty queue
        if (pool->count == 0) {
            pool->qhead = pool->qtail = null;
        }

        // release the lock on the queue
        pthread_mutex_unlock(&(pool->lock));

        // run the task
        (*(task.func))(task.arg);
    }

    pool->started--;

    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(null);
    return(null);
}
