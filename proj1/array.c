#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "array.h"

struct array {
    const int8_t t;
    size_t m;
    size_t n;
    void **as;
};

array *arr_init(size_t m) {
    array *arr = malloc(sizeof(array));

    *((int8_t *) arr) = 2;
    arr->m = m;
    arr->n = 0;
    arr->as = malloc(m * sizeof(void *));

    return arr;
}

array *arr_copy(array *arr, size_t m) {
    array *new_arr = malloc(sizeof(array));

    new_arr->as = malloc(m * sizeof(void *));
    new_arr->m = m;

    if (arr == null) {
        new_arr->n = 0;
        memset(new_arr->as, 0, m * sizeof(void *));
    } else {
        new_arr->n = arr->n < m ? arr->n : m;
        memcpy(new_arr->as, arr->as, new_arr->n * sizeof(void *));
    }

    return new_arr;
}

void arr_free(array *arr) {
    free(arr->as);
    free(arr);
}

void arr_reset(array *arr) {
    memset(arr->as, 0, arr->n * sizeof(void *));
    arr->n = 0;
}

void *arr_peek(array *arr) {
    return &arr->as[0];
}

void *arr_pop(array *arr) {
    void *a = arr->as[0];
    memmove(arr->as, arr->as + 1, (arr->n - 1) * sizeof(void *));
    return a;
}

bool arr_push(array *arr, void *t) {
    if (arr->n == arr->m) {
        size_t new_size = arr->m << 1;
        void **new_as;
        if ((new_as = malloc(new_size * sizeof(void *))) == null) {
            return false;
        }
        memcpy(new_as, arr->as, arr->m * sizeof(void *));
        free(arr->as);
        arr->as = new_as;
        arr->m = new_size;
    }

    memcpy(&arr->as[arr->n++], t, sizeof(void *));
    free(t);
    return true;
}

bool arr_concat(array *dest, array *src) {
    if (dest->n + src->n >= dest->m) {
        size_t new_size = dest->m + src->m;
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
    arr_free(src);
    return true;
}

void arr_foreach(array *arr, void *(*func)(void *)) {
    int i;
    for (i = 0; i < arr->n; i++) {
        arr->as[i] = (func)(arr->as[i]);
    }
}

size_t arr_reduce(array *arr, bool (*func)(void *)) {
    size_t i, c = 0;
    for (i = 0; i < arr->n && (func)(arr->as[i]); i++, c++) {
    }

    if (c < arr->n) {
        for (i++; i < arr->n; i++) {
            if ((func)(arr->as[i])) {
                arr->as[c++] = arr->as[i];
            }
        }

        arr->n = c;
    }

    return arr->n;
}
