#include "hashmap.h"
#include "array.h"

#include <malloc.h>

struct hashmap {
    unsigned s;
    unsigned n;

    unsigned (*hash)(void *);

    bool (*cmp)(void *, void *);

    array **map;
};

inline optional hashmap_init(unsigned s, unsigned (*hash)(void *), bool (*cmp)(void *, void *)) {
    optional opt;
    hashmap *h;
    if (s == 0 || (h = malloc(sizeof(hashmap))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    if ((h->map = malloc(s * sizeof(array *))) == null) {
        free(h);
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    unsigned i;
    for (i = 0; i < s; i++) {
        if ((opt = arr_init(1)).e) {
            h->map[i] = opt.val;
        } else {
            for (i; i != 0; i--) {
                arr_free(h->map[i - 1]);
            }
            free(h->map);
            free(h);
            return opt;
        }
    }

    h->hash = hash;
    h->cmp = cmp;
    h->n = 0;

    opt.e = true;
    opt.val = h;
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

inline unsigned hashmap_size(hashmap *h) {
    return h->n;
}

inline optional hashmap_get(hashmap *h, void *i) {
    unsigned hsh = h->hash(i);

    unsigned idx;
    array *a = h->map[hsh];
    unsigned s = arr_size(a);
    for (idx = 0; idx < s && !h->cmp(arr_get(a, idx).val, i); idx++) {}

    return arr_get(a, idx);
}

inline bool hashmap_insert(hashmap *h, void *t, void *i) {
    unsigned hsh = h->hash(i);

    unsigned idx;
    array *a = h->map[hsh];
    unsigned s = arr_size(a);
    for (idx = 0; idx < s && !h->cmp(arr_get(a, idx).val, i); idx++) {}

    if (idx < s) {
        free(arr_get(a, idx).val);
        arr_set(a, t, idx);
        return true;
    } else {
        arr_push(a, t);
        h->n++;
        return false;
    }
}

inline void hashmap_foreach(hashmap *h, void *(*func)(void *)) {
    unsigned i;
    for (i = 0; i < h->s; i++) {
        arr_foreach(h->map[i], func);
    }
}
