#include <stdlib.h>
#include <string.h>
#include "array.h"

array *arr_init(size_t s) {
    array *arr = malloc(sizeof(array));

    arr->max_ts = s;
    arr->num_ts = 0;
    arr->curr = 0;
    arr->ts = malloc(s * sizeof(token));

    return arr;
}

array *arr_copy(array *arr, size_t s) {
    array *new_arr = malloc(sizeof(array));

    new_arr->ts = malloc(s * sizeof(token));
    new_arr->max_ts = s;

    if (arr == null) {
        new_arr->curr = 0;
        new_arr->num_ts = 0;
    } else {
        new_arr->max_ts = arr->max_ts;
        new_arr->num_ts = arr->num_ts;
        memcpy(new_arr->ts, arr->ts, new_arr->num_ts * sizeof(token));
    }

    return new_arr;
}

void arr_free(array *arr) {
    free(arr->ts);
    free(arr);
}

void arr_reset(array *arr) {
    memset(arr->ts, 0, arr->num_ts * sizeof(token));
    arr->num_ts = 0;
}

token *arr_peek(array *arr) {
    return arr->curr < arr->num_ts ? arr->ts[arr->curr] : null;
}

token *arr_pop(array *arr) {
    return arr->curr < arr->num_ts ? arr->ts[arr->curr++] : null;
}

bool arr_push(array *arr, token *t) {
    if (arr->num_ts == arr->max_ts) {
        size_t new_size = arr->max_ts << 1;
        token *new_ts;
        if ((new_ts = malloc(new_size * sizeof(token))) == null) {
            return false;
        }
        memcpy(new_ts, arr->ts, arr->max_ts * sizeof(token));
        free(arr->ts);
        arr->ts = new_ts;
        arr->max_ts = new_size;
    }

    memcpy(&arr->ts[arr->num_ts++], t, sizeof(token));
    free(t);
    return true;
}

bool arr_concat(array *dest, array *src) {
    if (dest->num_ts + src->num_ts >= dest->max_ts) {
        size_t new_size = dest->max_ts + src->max_ts;
        token *new_ts;
        if ((new_ts = malloc(new_size * sizeof(token))) == null) {
            return false;
        }
        memcpy(new_ts, dest->ts, dest->num_ts * sizeof(token));
        free(dest->ts);
        dest->ts = new_ts;
    }

    memcpy(&dest->ts[dest->num_ts], src->ts, src->num_ts * sizeof(token));
    dest->num_ts += src->num_ts;
    arr_free(src);
    return true;
}
