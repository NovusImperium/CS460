#include <stdlib.h>
#include <string.h>
#include "farray.h"
#include "thread_sort.h"

farray *fa_init() {
    farray *fa = malloc(sizeof(farray));

    fa->max_fs = 100;
    fa->num_fs = 0;
    fa->fs = malloc(fa->max_fs * f_ptr);

    return fa;
}

bool fa_push(farray *fa, fraction *f) {
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

void fa_print(farray *fa, FILE *out) {
    int i;
    for (i = 0; i < fa->num_fs; ++i) {
        f_print(fa->fs[i], out);
    }
}

void fa_sort(farray *fa) {
    msg *m = malloc(sizeof(msg));
    m->lo = 0;
    m->hi = fa->num_fs - 1;
    m->fs = fa->fs;

    th_sort(m);
}
