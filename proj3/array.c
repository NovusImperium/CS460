#include "array.h"
#include <stdlib.h>
#include <string.h>


struct array {
    unsigned s;
    unsigned n;
    void **as;
};

// resize the array to the given value, can be larger or smaller than the current size
// if the new size is smaller, the items at index size or higher are lost
// does nothing if the new size is not different from the old size
static inline bool resize(array *arr, unsigned s) {
    if (s < arr->n) {
        void **as;
        if ((as = malloc(s * sizeof(void *))) == null) {
            return false;
        }

        memcpy(as, arr->as, s * sizeof(void *));
        free(arr->as);
        arr->as = as;
        arr->s = s;
        arr->n = arr->n < s ? arr->n : s;

        return true;
    } else if (s > arr->n) {
        void **as;
        if ((as = realloc(arr->as, s * sizeof(void *))) != null) {
            arr->as = as;
            arr->s = s;

            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

inline optional arr_init(unsigned s) {
    optional opt;
    array *arr;
    if ((arr = malloc(sizeof(array))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    if ((arr->as = malloc(s * sizeof(void *))) == null) {
        free(arr);
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    memset(arr->as, 0, s * sizeof(void *));

    arr->s = s;
    arr->n = 0;

    opt.e = true;
    opt.val = arr;
    return opt;
}

inline optional arr_copy(array *arr, unsigned s) {
    optional opt;

    array *new_arr;
    if ((new_arr = malloc(sizeof(array))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    if ((new_arr->as = malloc(s * sizeof(void *))) == null) {
        free(new_arr);
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    new_arr->s = s;

    if (arr == null) {
        new_arr->n = 0;
        memset(new_arr->as, 0, s * sizeof(void *));
    } else {
        new_arr->n = arr->n < s ? arr->n : s;
        memcpy(new_arr->as, arr->as, new_arr->n * sizeof(void *));
    }

    opt.e = true;
    opt.val = arr;
    return opt;
}

inline void arr_free(array *arr) {
    if (arr != null) {
        if (arr->as != null) {
            free(arr->as);
        }
        free(arr);
    }
}

inline optional arr_peek(array *arr) {
    optional opt;
    if (arr->n > 0) {
        opt.e = true;
        opt.val = arr->as[arr->n - 1];
    } else {
        opt.e = false;
        opt.err = out_of_bounds;
    }

    return opt;
}

inline optional arr_pop(array *arr) {
    optional opt;
    if (arr->n > 0) {
        arr->n--;
        opt.e = true;
        opt.val = arr->as[arr->n];
        arr->as[arr->n] = null;

        /*if (arr->n < (arr->s / 4)) {
            resize(arr, arr->s / 2);
        ]*/
    } else {
        opt.e = false;
        opt.err = out_of_bounds;
    }

    return opt;
}

inline bool arr_push(array *arr, void *a) {
    if (arr->n == arr->s && !resize(arr, arr->s * 2)) {
        return false;
    }

    arr->as[arr->n++] = a;

    return true;
}

inline bool arr_concat(array *dest, array *src) {
    if (dest->n + src->n >= dest->s) {
        unsigned new_size = dest->s + src->s;
        void **new_as;
        if ((new_as = malloc(new_size * sizeof(void *))) == null) {
            return false;
        }
        memcpy(new_as, dest->as, dest->n * sizeof(void *));
        free(dest->as);
        dest->as = new_as;
    }

    memcpy(&dest->as[dest->n], src->as, src->n * sizeof(void *));
    dest->n += src->n;
    return true;
}

inline void arr_foreach(array *arr, void *(*func)(void *)) {
    int i;
    for (i = 0; i < arr->n; i++) {
        arr->as[i] = (func)(arr->as[i]);
    }
}

inline int arr_reduce(array *arr, optional (*func)(void *)) {
    int i, c = 0;
    for (i = 0; i < arr->n; i++) {
        optional opt = func(arr->as[i]);
        if (opt.e) {
            arr->as[c++] = opt.val;
        }
    }

    if (c < arr->n) {
        arr->n = (unsigned) c;
        memset(&arr->as[arr->n], 0, (arr->s - arr->n) * sizeof(void *));
    }

    return c;
}

inline unsigned arr_size(array *arr) {
    return arr->n;
}

inline optional arr_get(array *arr, unsigned i) {
    optional opt;
    if (i < arr->n) {
        opt.e = true;
        opt.val = arr->as[i];
    } else {
        opt.e = false;
        opt.err = out_of_bounds;
    }
    return opt;
}

inline bool arr_set(array *arr, void *a, unsigned i) {
    if (i < arr->n) {
        arr->as[i] = a;
        return true;
    }
    return false;
}

inline bool arr_insert(array *arr, void *a, unsigned i) {
    if (i <= arr->n) {
        if (arr->n++ == arr->s && !resize(arr, arr->s * 2)) {
            return false;
        }
        memmove(&arr->as[i + 1], &arr->as[i], (arr->n - i) * sizeof(void *));
        arr->as[i] = a;
        return true;
    }
    return false;
}

void arr_reset(array *arr) {
    memset(arr->as, 0, arr->n * sizeof(void *));
    arr->n = 0;
}
