#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "heap.h"

// initialize the heap into the given pointer
heap *h_init() {
    heap *h = malloc(sizeof(heap));

    h->max_fs = 100;
    h->num_fs = 0;
    h->fs = malloc(h->max_fs * f_ptr);

    return h;
}

// insert a fraction into the heap, return true if successful
bool h_push(heap *h, fraction *f) {
    h->num_fs++;
    if (h->num_fs == h->max_fs) {
        size_t new_size = h->max_fs << 4;
        fraction **new_fs;
        if ((new_fs = malloc(new_size * f_ptr)) == null) {
            return false;
        }
        memmove(new_fs, h->fs, h->max_fs * f_ptr);
        free(h->fs);
        h->fs = new_fs;
        h->max_fs = new_size;
    }

    h->fs[h->num_fs] = f;

    size_t curr = h->num_fs;
    size_t half = h->num_fs >> 1;
    while (half > 0 && f_lt(h->fs[curr], h->fs[half])) {
        fraction *tmp = h->fs[half];
        h->fs[half] = h->fs[curr];
        h->fs[curr] = tmp;

        curr = half;
        half = half >> 1;
    }

    return true;
}

// pop the top of the heap into the given pointer and remove it from the heap
fraction *h_pop(heap *h) {
    if (h->num_fs <= 1) {
        size_t i = h->num_fs;
        h->num_fs = 0;
        return h->fs[i];
    }

    fraction *f = h->fs[1];
    h->fs[1] = h->fs[h->num_fs];
    h->fs[h->num_fs] = null;

    size_t curr = 1;
    while (true) {
        size_t l = curr << 1;
        size_t r = l + 1;
        if (r < h->num_fs && f_lt(h->fs[r], h->fs[l]) && f_lt(h->fs[r], h->fs[curr])) {
            fraction *t = h->fs[curr];
            h->fs[curr] = h->fs[r];
            h->fs[r] = t;
            curr = r;
        }
        else if (l < h->num_fs && f_lt(h->fs[l], h->fs[curr])) {
            fraction *t = h->fs[curr];
            h->fs[curr] = h->fs[l];
            h->fs[l] = t;
            curr = l;
        }
        else {
            break;
        }
    }

    h->num_fs--;
    return f;
}
