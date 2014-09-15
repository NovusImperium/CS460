#include <stdlib.h>
#include "defs.h"
#include "thread_sort.h"
#include "heap.h"

void *th_sort(void *th_msg) {
    pthread_t th_lo, th_hi;
    msg *m = (msg *) th_msg;
    size_t lo = m->lo;
    size_t hi = m->hi;
    size_t mid = (lo + hi) / 2;

    if (hi - lo > 2048) {
        msg *lo_msg = malloc(sizeof(msg));
        lo_msg->lo = lo;
        lo_msg->hi = mid;
        lo_msg->fs = m->fs;

        pthread_create(&th_lo, null, th_sort, lo_msg);

        msg *hi_msg = malloc(sizeof(msg));
        hi_msg->lo = mid + 1;
        hi_msg->hi = hi;
        hi_msg->fs = m->fs;

        pthread_create(&th_hi, null, th_sort, hi_msg);

        farr *lo_fa = malloc(sizeof(farr));
        farr *hi_fa = malloc(sizeof(farr));
        pthread_join(th_lo, (void **) &lo_fa);
        pthread_join(th_hi, (void **) &hi_fa);

        free(lo_msg);
        free(hi_msg);

        farr *fa = fa_copy(null, lo_fa->max_fs << 1);

        while (true) {
            fraction *lo_f = fa_peek(lo_fa);
            fraction *hi_f = fa_peek(hi_fa);
            if (lo_f != null && hi_f != null) {
                if (f_lt(lo_f, hi_f)) {
                    fa_push(fa, fa_pop(lo_fa));
                } else {
                    fa_push(fa, fa_pop(lo_fa));
                }
            } else if (lo_f != null) {
                fa_push(fa, fa_pop(lo_fa));
            } else if (hi_f != null) {
                fa_push(fa, fa_pop(lo_fa));
            } else {
                break;
            }
        }

        fa_free(lo_fa);
        fa_free(hi_fa);

        return fa;
    } else {
        msg *lo_msg = malloc(sizeof(msg));
        lo_msg->lo = lo;
        lo_msg->hi = mid;
        lo_msg->fs = m->fs;

        pthread_create(&th_lo, null, heap_sort, lo_msg);

        msg *hi_msg = malloc(sizeof(msg));
        hi_msg->lo = mid + 1;
        hi_msg->hi = hi;
        hi_msg->fs = m->fs;

        pthread_create(&th_hi, null, heap_sort, hi_msg);

        heap *lo_h = malloc(sizeof(heap));
        heap *hi_h = malloc(sizeof(heap));
        pthread_join(th_lo, (void **) &lo_h);
        pthread_join(th_hi, (void **) &hi_h);

        free(lo_msg);
        free(hi_msg);

        farr *fa = fa_copy(null, lo_h->max_fs << 1);

        while (true) {
            fraction *lo_f = h_peek(lo_h);
            fraction *hi_f = h_peek(hi_h);
            if (lo_f != null && hi_f != null) {
                if (f_lt(lo_f, hi_f)) {
                    fa_push(fa, h_pop(lo_h));
                } else {
                    fa_push(fa, h_pop(hi_h));
                }
            } else if (lo_f != null) {
                fa_push(fa, h_pop(lo_h));
            } else if (hi_f != null) {
                fa_push(fa, h_pop(hi_h));
            } else {
                break;
            }
        }

        h_free(lo_h);
        h_free(hi_h);

        return fa;
    }
}

void *heap_sort(void *th_msg) {
    msg *m = (msg *)th_msg;
    heap *h = h_copy(null, m->hi - m->lo + 2);

    size_t i;
    for (i = m->lo; i <= m->hi; i++) {
        h_push(h, m->fs[i]);
    }

    return h;
}

void *ins_sort(void *th_msg) {
    size_t lo = ((msg *) th_msg)->lo;
    size_t hi = ((msg *) th_msg)->hi;
    fraction **fs = ((msg *) th_msg)->fs;

    size_t curr;
    for (curr = lo; curr <= hi; curr++) {
        size_t i;
        size_t min = curr;
        for (i = curr + 1; i < hi; i++) {
            min = f_lt(fs[i], fs[min]) ? i : min;
        }

        if (min != curr) {
            fraction *f = fs[min];
            fs[min] = fs[curr];
            fs[curr] = f;
        }
    }
}
