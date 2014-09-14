#ifndef BST
#define BST

#include "fraction.h"

typedef struct bst_node bst;
struct bst_node {
    fraction *f;
    bst *l;
    bst *r;
};


// create a new bst node
bst *bst_init(fraction *f);

// insert a fraction into the bst
void bst_push(bst *b, fraction *f);

// print the fractions in the bst to the FILE
void bst_print(bst *b, FILE *out);

#endif