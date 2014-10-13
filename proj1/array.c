#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "array.h"
#include "container_types.h"

struct array {
    unsigned m;
    unsigned n;
    token **as;
};

optional arr_init(unsigned m) {
    optional opt;
    opt.e = true;

    array *arr;
    if ((arr = malloc(sizeof(array))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    if ((arr->as = malloc(m * sizeof(token *))) == null) {
        free(arr);
        opt.e = malloc_fail;
        return opt;
    }

    arr->m = m;
    arr->n = 0;

    opt.val = arr;
    return opt;
}

optional arr_copy(array *arr, unsigned m) {
    optional opt;

    array *new_arr;
    if ((new_arr = malloc(sizeof(array))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    if ((new_arr->as = malloc(m * sizeof(token *))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    new_arr->m = m;

    if (arr == null) {
        new_arr->n = 0;
        memset(new_arr->as, 0, m * sizeof(token *));
    } else {
        new_arr->n = arr->n < m ? arr->n : m;
        memcpy(new_arr->as, arr->as, new_arr->n * sizeof(token *));
    }

    opt.e = true;
    opt.val = arr;
    return opt;
}

void arr_free(array *arr) {
    free(arr->as);
    free(arr);
}

optional arr_peek(array *arr) {
    optional opt;
    if (arr->n > 0) {
        opt.e = true;
        opt.val = arr->as[0];
    } else {
        opt.e = false;
        opt.err = out_of_bounds;
    }

    return opt;
}

optional arr_pop(array *arr) {
    optional opt;
    if (arr->n > 1) {
        opt.e = true;
        opt.val = arr->as[0];
        memmove(arr->as, &arr->as[1], (arr->n - 1) * sizeof(token *));
        arr->n--;
    } else if (arr->n == 1) {
        opt.e = true;
        opt.val = arr->as[0];
        memset(arr->as, 0, sizeof(token*));
        arr->n--;
    } else {
        opt.e = false;
        opt.err = out_of_bounds;
    }

    return opt;
}

bool arr_push(array *arr, token * a) {
    if (arr->n == arr->m) {
        unsigned new_size = arr->m * 2;
        token * *new_as;
        if ((new_as = malloc(new_size * sizeof(token *))) == null) {
            return false;
        }
        memcpy(new_as, arr->as, arr->m * sizeof(token *));
        free(arr->as);
        arr->as = new_as;
        arr->m = new_size;
    }

    arr->as[arr->n++] = a;

    return true;
}

bool arr_concat(array *dest, array *src) {
    if (dest->n + src->n >= dest->m) {
        unsigned new_size = dest->m + src->m;
        token * *new_as;
        if ((new_as = malloc(new_size * sizeof(token *))) == null) {
            return false;
        }
        memcpy(new_as, dest->as, dest->n * sizeof(token *));
        free(dest->as);
        dest->as = new_as;
    }

    memcpy(&dest->as[dest->n], src->as, src->n * sizeof(token *));
    dest->n += src->n;
    return true;
}

int arr_foreach(array *arr, token * (*func)(token *)) {
    int i;
    for (i = 0; i < arr->n; i++) {
        arr->as[i] = (func)(arr->as[i]);
    }

    return 0;
}

int arr_reduce(array *arr, optional (*func)(token *)) {
    int i, c = 0;
    for (i = 0; i < arr->n; i++) {
        optional opt = func(arr->as[i]);
        if (opt.e) {
            arr->as[c++] = opt.val;
        }
    }

    if (c < arr->n) {
        arr->n = (unsigned) c;
        memset(&arr->as[arr->n], 0, (arr->m - arr->n) * sizeof(token *));
    }

    return c;
}

unsigned arr_size(array *arr) {
    return arr->n;
}

optional arr_get(array *arr, unsigned i) {
    optional opt;
    if (i < arr->n) {
        opt.e = true;
        opt.val = &arr->as[i];
    } else {
        opt.e = false;
        opt.err = out_of_bounds;
    }
    return opt;
}

bool arr_insert(array *arr, token *a, unsigned i) {
    return false;
}
