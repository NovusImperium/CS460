#include "defs.h"

#ifndef SET_H
#define SET_H

typedef struct set set;

// initialize the item set with max length of m
extern inline optional set_init(int (*cmp)(void *, void *));

// free the memory used by the item set
extern inline void set_free(set *s);

// return the number of elements in the set
extern inline unsigned set_size(set *s);

// insert a item into the item set, return true if successful
extern inline bool set_push(set *s, void *t);

// peek at the tail of the item set
extern inline optional set_peek(set *s);

// pop the tail of the item set
extern inline optional set_pop(set *s);

// apply the function to each item in the set
extern inline void set_foreach(set *s, void *(*func)(void *));

#endif  // SET_H