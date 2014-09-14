#include <stdlib.h>
#include "bst.h"

bst *bst_init(fraction *f) {
    bst *b = calloc(1, sizeof(bst));
    b->f = f;
    return b;
}

void bst_push(bst *b, fraction *f) {
    if (b->f == null) {
        b->f = f;
        return;
    }
    while (true) {
        if (f_lt(f, b->f)) {
            if (b->l == null) {
                b->l = bst_init(f);
                return;
            }
            else {
                b = b->l;
            }
        }
        else {
            if (b->r == null) {
                b->r = bst_init(f);
                return;
            }
            else {
                b = b->r;
            }
        }
    }
}

void bst_print(bst *b, FILE *out) {
    if (b->l != null) {
        bst_print(b->l, out);
    }
    f_print(b->f, out);
    if (b->r != null) {
        bst_print(b->r, out);
    }
}
