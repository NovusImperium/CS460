#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "heap_test.h"

// initialize the heap into the given pointer
heap *h_init() {
    heap *h = malloc(sizeof(heap));

    h->max_is = 100;
    h->num_is = 0;
    h->is = malloc(h->max_is * i_ptr);
    h->is[0] = null;

    return h;
}

// insert a int into the heap, return true if successful
bool h_push(heap *h, int *i) {
    h->num_is++;
    if (h->num_is == h->max_is) {
        size_t new_size = h->max_is << 4;
        int **new_is;
        if ((new_is = malloc(new_size * i_ptr)) == null) {
            return false;
        }
        memmove(new_is, h->is, h->max_is * i_ptr);
        free(h->is);
        h->is = new_is;
        h->max_is = new_size;
    }

    h->is[h->num_is] = i;

    size_t curr = h->num_is;
    size_t half = h->num_is >> 1;
    while (half > 0 && i_lt(h->is[curr], h->is[half])) {
        int *tmp = h->is[half];
        h->is[half] = h->is[curr];
        h->is[curr] = tmp;

        curr = half;
        half = half >> 1;
    }

    return true;
}

// pop the top of the heap into the given pointer and remove it from the heap
int *h_pop(heap *h) {
    if (h->num_is <= 1) {
        size_t i = h->num_is;
        h->num_is = 0;
        return h->is[i];
    }

    int *i = h->is[1];
    h->is[1] = h->is[h->num_is];
    h->is[h->num_is] = null;

    size_t curr = 1;
    while (true) {
        size_t l = curr << 1;
        size_t r = l + 1;
        if (r < h->num_is && i_lt(h->is[r], h->is[l]) && i_lt(h->is[r], h->is[curr])) {
            int *t = h->is[curr];
            h->is[curr] = h->is[r];
            h->is[r] = t;
            curr = r;
        }
        else if (l < h->num_is && i_lt(h->is[l], h->is[curr])) {
            int *t = h->is[curr];
            h->is[curr] = h->is[l];
            h->is[l] = t;
            curr = l;
        }
        else {
            break;
        }
    }

    h->num_is--;
    return i;
}

bool i_lt(int *lhs, int *rhs) {
    return *lhs < *rhs;
}