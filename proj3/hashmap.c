#include <malloc.h>
#include "hashmap.h"
#include "array.h"

struct hashmap {
    size_t s;
    size_t n;
    unsigned (*hash)(void*);
    bool (*cmp)(void*,void*);
    array **map;
};

inline optional hashmap_init(size_t s, unsigned int (*hash)(void *), bool (*cmp)(void *, void *)) {
    optional opt;
    return opt;
}

inline void hashmap_free(hashmap *h) {
    int i;
    for (i = 0; i < h->s; i++) {
        arr_free(h->map[i]);
    }
    free(h->map);
    free(h);
}

inline size_t hashmap_size(hashmap *h) {
    return h->n;
}

inline optional hashmap_get(hashmap *h, void *i) {
    unsigned hsh = h->hash(i);

    unsigned idx;
    array *a = h->map[hsh];
    unsigned s = arr_size(a);
    for (idx = 0; idx < s && !h->cmp(arr_get(a, idx).val, i); idx++) { }

    return arr_get(a, idx);
}

inline bool hashmap_insert(hashmap *h, void *t, void *i) {
    return 0;
}

inline void hashmap_foreach(hashmap *h, void *(*func)(void *)) {

}
