#include <stdlib.h>
#include <etip.h>
#include "defs.h"
#include "thread_sort.h"
#include "heap.h"

void *th_sort(void *th_msg) {
    msg *m = (msg *)th_msg;

    if (m->hi < 1024) {
        return ins_sort(m);
    }

    size_t n;
    for (n = 1; n < 8 && m->hi >> n >= 1024; n++);

    thread_pool tp;
    tp.num_ts = (size_t)1 << n;
    tp.ts = malloc(tp.num_ts * sizeof(pthread_t));

    size_t js = tp.num_ts;
    msg *ms = mk_jobs(m, js);

    heap *hs = malloc(js * sizeof(heap));

    size_t i;
    for (i = 0; i < tp.num_ts; i++) {
        pthread_create(&tp.ts[i], null, heap_sort, &ms[i]);
    }

    for (i = 0; i < tp.num_ts; i++) {
        heap *h = &hs[i];
        pthread_join(tp.ts[i], (void **) &h);

        size_t j = ms[i].lo;
        while (h_peek(&hs[i]) != null) {
            m->fs[j] = h_pop(&hs[i]);
            j++;
        }

        h_free(&hs[i]);
    }

    do {
        farr *fa = malloc(js * sizeof(farr));
        for (i = 0; i < js; i++) {
            
        }

        ms = merge_jobs(ms, js);
        js = js >> 1;
    } while (js > 1);
}

msg *mk_jobs(msg *m, size_t js) {
    msg *ms = malloc(js * sizeof(msg));
    size_t len = m->hi / js;
    size_t lo = 0;
    size_t hi = len;

    size_t i;
    for (i = 0; i < js; i++) {
        ms[i].fs = m->fs;
        ms[i].lo = lo;
        ms[i].hi = hi > m->hi ? m->hi : hi;

        lo = hi + 1;
        hi = hi + len;
    }

    return ms;
}

msg *merge_jobs(msg *ms, size_t js) {
    size_t new_js = js >> 1;
    msg *new_ms = malloc(new_js * sizeof(msg));

    size_t i;
    size_t j = 0;
    for (i = 1; i < js; i += 2) {
        new_ms[j].fs = ms[0].fs;
        new_ms[j].lo = ms[i-1].lo;
        new_ms[j].hi = ms[i].hi;
        j++;
    }

    free(ms);
    return new_ms;
}

void *th_sort_old(void *th_msg) {
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
