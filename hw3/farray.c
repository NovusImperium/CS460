#include <stdlib.h>
#include <string.h>
#include "farray.h"
#include "thread_sort.h"

farr *fa_init() {
    farr *fa = malloc(sizeof(farr));

    fa->max_fs = 100;
    fa->num_fs = 0;
    fa->curr = 0;
    fa->fs = malloc(fa->max_fs * f_ptr);

    return fa;
}

farr *fa_copy(farr *fa, size_t s) {
    farr *new_fa = malloc(sizeof(farr));

    new_fa->fs = malloc(s * f_ptr);
    new_fa->max_fs = s;

    if (fa == null) {
        new_fa->curr = 0;
        new_fa->num_fs = 0;
    } else {

    }

    return new_fa;
}

void fa_free(farr *fa) {
    free(fa->fs);
    free(fa);
}

fraction *fa_peek(farr *fa) {
    return fa->curr < fa->num_fs ? fa->fs[fa->curr] : null;
}

fraction *fa_pop(farr *fa) {
    return fa->curr < fa->num_fs ? fa->fs[fa->curr++] : null;
}

bool fa_push(farr *fa, fraction *f) {
    if (fa->num_fs == fa->max_fs) {
        size_t new_size = fa->max_fs << 4;
        fraction **new_fs;
        if ((new_fs = malloc(new_size * f_ptr)) == null) {
            return false;
        }
        memmove(new_fs, fa->fs, fa->max_fs * f_ptr);
        free(fa->fs);
        fa->fs = new_fs;
        fa->max_fs = new_size;
    }

    fa->fs[fa->num_fs++] = f;
    return true;
}

void fa_print(farr *fa, FILE *out) {
    int i;
    for (i = 0; i < fa->num_fs; ++i) {
        f_print(fa->fs[i], out);
    }
}

void fa_sort(farr *fa) {
    threadpool *pool = tp_init(1);

    size_t len = fa->num_fs - 1;
    size_t mid = len / 2;
    msg *r = malloc(sizeof(msg));
    msg *l = malloc(sizeof(msg));

    r->lo = 0;
    r->hi = mid;
    r->fslice = fa->fs;
    pthread_mutex_init(r->r_lock, null);

    pthread_mutex_lock(r->r_lock);

    l->lo = mid + 1;
    l->hi = len;
    l->fslice = &fa->fs[mid + 1];
    pthread_mutex_init(l->r_lock, null);

    tp_dest(pool, tpexit_graceful);
}

