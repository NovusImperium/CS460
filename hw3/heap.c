#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "heap.h"

// initialize the heap into the given pointer
heap *h_init(int (*comp)(const void *, const void *)) {
    heap *h = malloc(sizeof(heap));

    h->m = 100;
    h->n = 0;
    h->comp = comp;
    h->q = malloc(h->m * sizeof(void*));

    return h;
}

heap *h_copy(heap *h, size_t s, int (*comp)(void const *, void const *)) {
    heap *new_h = malloc(sizeof(heap));

    new_h->m = s;
    new_h->q = malloc(s * sizeof(void*));

    if (h == null) {
        new_h->n = 0;
        new_h->comp = comp;
    } else {
        new_h->n = h->n > s ? s : h->n;
        new_h->comp = h->comp;
        memcpy(new_h->q, h->q, new_h->n * sizeof(void*));
    }

    return new_h;
}

void h_free(heap *h) {
    free(h->q);
    free(h);
}

// insert a fraction into the heap, return true if successful
bool h_push(heap *h, void *i) {
    h->n++;
    if (h->n == h->m) {
        size_t new_size = h->m << 4;
        void **new_fs;
        if ((new_fs = malloc(new_size * sizeof(void*))) == null) {
            return false;
        }
        memmove(new_fs, h->q, h->m * sizeof(void*));
        free(h->q);
        h->q = new_fs;
        h->m = new_size;
    }

    h->q[h->n] = i;

    size_t curr = h->n;
    size_t half = h->n >> 1;
    while (half > 0 && h->comp(h->q[curr], h->q[half])) {
        void *tmp = h->q[half];
        h->q[half] = h->q[curr];
        h->q[curr] = tmp;

        curr = half;
        half = half >> 1;
    }

    return true;
}

void * h_peek(heap *h) {
    return h->n == 0 ? null : h->q[1];
}

// pop the top of the heap into the given pointer and remove it from the heap
void * h_pop(heap *h) {
    if (h->n <= 1) {
        size_t i = h->n;
        h->n = 0;
        return h->q[i];
    }

    void *f = h->q[1];
    h->q[1] = h->q[h->n];
    h->q[h->n] = null;

    size_t curr = 1;
    while (true) {
        size_t l = curr << 1;
        size_t r = l + 1;
        if (r < h->n && h->comp(h->q[r], h->q[l]) && h->comp(h->q[r], h->q[curr])) {
            void *t = h->q[curr];
            h->q[curr] = h->q[r];
            h->q[r] = t;
            curr = r;
        } else if (l < h->n && h->comp(h->q[l], h->q[curr])) {
            void *t = h->q[curr];
            h->q[curr] = h->q[l];
            h->q[l] = t;
            curr = l;
        } else {
            break;
        }
    }

    h->n--;
    return f;
}
