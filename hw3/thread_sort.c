#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "thread_sort.h"
#include "farray.h"

void * heap_merge(void *arg) {
    pthread_t th_lo, th_hi;
    msg *m = arg;

    msg *lo_msg = malloc(sizeof(msg));
    lo_msg->lo = m->lo;
    lo_msg->hi = m->mid;
    lo_msg->fs = m->fs;

    pthread_create(&th_lo, null, heap_sort, lo_msg);

    msg *hi_msg = malloc(sizeof(msg));
    hi_msg->lo = m->mid + 1;
    hi_msg->hi = m->hi;
    hi_msg->fs = m->fs;

    pthread_create(&th_hi, null, heap_sort, hi_msg);

    heap *lo_h;
    heap *hi_h;
    pthread_join(th_lo, (void **) &lo_h);
    pthread_join(th_hi, (void **) &hi_h);

    free(lo_msg);
    free(hi_msg);

    farr fa;
    fa.max_fs = m->hi;
    fa.curr = 0;
    fa.num_fs = 0;
    fa.fs = &m->fs[m->lo];

    while (true) {
        fraction *lo_f = h_peek(lo_h);
        fraction *hi_f = h_peek(hi_h);
        if (lo_f != null && hi_f != null) {
            if (f_lt(lo_f, hi_f)) {
                fa_push(&fa, h_pop(lo_h));
            } else {
                fa_push(&fa, h_pop(hi_h));
            }
        } else if (lo_f != null) {
            fa_push(&fa, h_pop(lo_h));
        } else if (hi_f != null) {
            fa_push(&fa, h_pop(hi_h));
        } else {
            break;
        }
    }

    h_free(lo_h);
    h_free(hi_h);

    return null;
}

void * th_merge(void *arg) {
    msg *m = arg;

    farr fa;
    fa.max_fs = m->hi;
    fa.num_fs = 0;
    fa.curr = 0;
    fa.fs = &m->fs[m->lo];

    farr lo_fa;
    lo_fa.max_fs = m->hi;
    lo_fa.curr = 0;
    lo_fa.num_fs = m->mid - m->lo + 1;
    lo_fa.fs = malloc(lo_fa.num_fs * f_ptr);
    memcpy(lo_fa.fs, &m->fs[m->lo], lo_fa.num_fs * f_ptr);

    farr hi_fa;
    hi_fa.max_fs = m->hi;
    hi_fa.curr = 0;
    hi_fa.num_fs = m->hi - m->mid;
    hi_fa.fs = malloc(lo_fa.num_fs * f_ptr);
    memcpy(hi_fa.fs, &m->fs[m->mid + 1], lo_fa.num_fs * f_ptr);

    while (true) {
        fraction *lo_f = fa_peek(&lo_fa);
        fraction *hi_f = fa_peek(&hi_fa);
        if (lo_f != null && hi_f != null) {
            if (f_lt(lo_f, hi_f)) {
                fa_push(&fa, fa_pop(&lo_fa));
            } else {
                fa_push(&fa, fa_pop(&lo_fa));
            }
        } else if (lo_f != null) {
            fa_push(&fa, fa_pop(&lo_fa));
        } else if (hi_f != null) {
            fa_push(&fa, fa_pop(&lo_fa));
        } else {
            break;
        }
    }

    free(lo_fa.fs);
    free(hi_fa.fs);

    return null;
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
        ms[i].mid = (ms[i].lo + ms[i].hi) / 2;

        lo = hi + 1;
        hi = hi + len;
    }

    return ms;
}

msg *merge_jobs(msg *ms, size_t js) {
    size_t new_js = js >> 1;

    if (new_js == 0) {
        free(ms);
        return null;
    }

    msg *new_ms = malloc(new_js * sizeof(msg));

    size_t i;
    size_t j = 0;
    for (i = 1; i < js; i += 2) {
        new_ms[j].fs = ms[0].fs;
        new_ms[j].lo = ms[i-1].lo;
        new_ms[j].mid = ms[i-1].hi;
        new_ms[j].hi = ms[i].hi;
        j++;
    }

    free(ms);
    return new_ms;
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

    return null;
}
