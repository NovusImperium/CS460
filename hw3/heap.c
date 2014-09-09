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
        if ((new_fs = malloc(new_size * f_ptr)) == NULL) {
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
    while (half > 0 && f_lt(h->fs[half], h->fs[curr])) {
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
    if (h->num_fs == 0) {
        return null;
    }

    fraction *f = h->fs[1];

    size_t curr = 1;
    size_t l = 2;
    size_t r = 3;
    while (curr < h->num_fs) {
        if (r < h->num_fs) {
            if (f_lt(h->fs[l], h->fs[r])) {
                h->fs[curr] = h->fs[r];
                curr = r;
            }
            else {
                h->fs[curr] = h->fs[l];
                curr = l;
            }
        }
        else if (l < h->num_fs) {
            h->fs[curr] = h->fs[l];
            curr = l;
        }
        else {
            break;
        }

        l = curr << 1;
        r = l + 1;
    }

    h->num_fs--;
    return f;
}
