#ifndef BST
#define BST

#include "defs.h"
#include "fraction.h"

typedef struct {
    fraction *f;
    size_t l;
    size_t r;
} bn;

typedef struct {
    size_t max_fs;
    size_t num_fs;
    bn *bns;
} bst;

static const size_t bn_ptr = sizeof(bn *);
static const size_t bn_size = sizeof(bn);
static const size_t bst_size = sizeof(bst);

// create a new bst node
bn *bn_init(fraction *f);

// initialize the bst array
bst *bst_init();

// insert a fraction into the bst
bool bst_push(bst *b, fraction *f);

// print the fractions in the bst to the FILE
void bst_print(bst *b, FILE *out, size_t curr);

#endif