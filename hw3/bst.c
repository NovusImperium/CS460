#include <stdlib.h>
#include <string.h>
#include "bst.h"

bn *bn_init(fraction *f) {
    bn *b = malloc(bn_size);
    b->f = f;

    return b;
}

bst *bst_init() {
    bst *b = malloc(bst_size);
    b->max_fs = 100;
    b->num_fs = 0;
    b->bns = malloc(b->max_fs * bn_size);

    return b;
}

bool bst_push(bst *b, fraction *f) {
    b->num_fs++;
    if (b->num_fs == b->max_fs) {
        size_t new_size = b->max_fs << 4;
        bn *new_bns;
        if ((new_bns = malloc(new_size * bn_size)) == null) {
            return false;
        }
        memmove(new_bns, b->bns, b->max_fs * bn_size);
        free(b->bns);
        b->bns = new_bns;
        b->max_fs = new_size;
    }

    b->bns[b->num_fs].f = f;
    b->bns[b->num_fs].l = 0;
    b->bns[b->num_fs].r = 0;

    size_t count = 1;
    size_t curr = 1;
    while (curr < b->num_fs) {
        if (count < b->num_fs) {
            count++;
        } else {
            return false;
        }
        if (f_lt(f, b->bns[curr].f)) {
            if (b->bns[curr].l == 0) {
                b->bns[curr].l = b->num_fs;
                return true;
            } else {
                curr = b->bns[curr].l;
            }
        } else {
            if (b->bns[curr].r == 0) {
                b->bns[curr].r = b->num_fs;
                return true;
            } else {
                curr = b->bns[curr].r;
            }
        }
    }

    return false;
}

void bst_print(bst *b, FILE *out, size_t curr) {
    if (b->bns[curr].l != 0) {
        bst_print(b, out, b->bns[curr].l);
    }
    f_print(b->bns[curr].f, out);
    if (b->bns[curr].r != 0) {
        bst_print(b, out, b->bns[curr].r);
    }
}
