#include <stddef.h>
#include "defs.h"

#ifndef HASHMAP
#define HASHMAP

typedef struct hashmap hashmap;

// inits the hashmap with a max hash size of s, with the hash and equality comparison functions given
extern inline optional hashmap_init(size_t s, unsigned (*hash)(void*), bool (*cmp)(void*,void*));

extern inline void hashmap_free(hashmap *h);

// return the number of elements in the hashmap
extern inline size_t hashmap_size(hashmap *h);

// get the element at the index 'i'
extern inline optional hashmap_get(hashmap *h, void *i);

// insert the element at the index 'i'
extern inline bool hashmap_insert(hashmap *h, void *t, void *i);

// apply the function to each item in the hashmap
extern inline void hashmap_foreach(hashmap *h, void *(*func)(void *));

#endif  // HASHMAP