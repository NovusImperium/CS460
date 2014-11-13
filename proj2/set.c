#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct set {
    unsigned sets;
    uint64_t *bitsets;
};

static const uint64_t mask = 0x3f;
static const unsigned idx = 0xffffc0;
static const unsigned vals[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63,
};

optional set_init(unsigned m) {
    optional opt;
    if (!m || (opt.val = malloc(sizeof(set))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    if ((((set *)opt.val)->bitsets = malloc(m * sizeof(uint64_t))) == null) {
        opt.e = false;
        free(opt.val);
        opt.err = malloc_fail;
        return opt;
    }

    memset(((set *)opt.val)->bitsets, 0, m * sizeof(uint64_t));
    opt.e = true;
    return opt;
}

bool set_intersect(set *a, set *b) {
    unsigned i;
    for (i = 0; i < a->sets; i++) {
        a->bitsets[i] &= b->bitsets[i];
    }
    return true;
}

bool set_union(set *a, set *b) {
    unsigned i;
    for (i = 0; i < a->sets; i++) {
        a->bitsets[i] |= b->bitsets[i];
    }
    return true;
}

void set_free(set *s) {
    free(s->bitsets);
    free(s);
}

unsigned set_size(set *s) {
    unsigned i, c = 0;
    for (i = 0; i < s->sets; i++) {
        uint64_t v = s->bitsets[i];
        for (c; v; c++) {
            v &= v - 1;
        }
    }
    return c;
}

bool set_insert(set *s, unsigned t) {
    unsigned i = t / 64;
    if (i < s->sets) {
        bool ret = (s->bitsets[i] & (1 << (t & mask))) != 0;
        s->bitsets[i] |= (1 << (t & mask));
        return ret;
    }

    return false;
}

void set_foreach(set *s, void (*func)(unsigned)) {
    unsigned i;
    for (i = 0; i < s->sets; i++) {
        unsigned n;
        for (n = 0; n < 64; n++) {
            if (s->bitsets[i] & (1 << n)) {
                func(64 * i + n);
            }
        }
    }
}

bool set_element(set *s, unsigned t) {
    unsigned i = t / 64;
    if (i < s->sets) {
        return (s->bitsets[i] & (1 << (t & mask))) != 0;
    }

    return false;
}
