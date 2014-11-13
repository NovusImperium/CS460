#ifndef SET
#define SET

#include "defs.h"

typedef struct set set;

// initialize the item set with max length of m
extern optional set_init(unsigned m);

// intersect the two sets into set a
extern bool set_intersect(set *a, set *b);

// union the two sets into set b
extern bool set_union(set *a, set *b);

// free the memory used by the item set
extern void set_free(set *s);

// return the number of elements in the set
extern unsigned set_size(set *s);

// insert the element into the set
extern int set_insert(set *s, unsigned t);

// returns true if the argument is an element of the set
extern bool set_element(set *s, unsigned t);

// apply the function to each item in the set
extern void set_foreach(set *s, void (*func)(unsigned));

#endif // SET
