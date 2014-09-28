#ifndef ARRAY
#define ARRAY

#include <stdio.h>
#include "defs.h"
#include "lexer_dfa.h"

typedef struct {
    size_t max_ts;
    size_t num_ts;
    size_t curr;
    token *ts;
} array;

// initialize the token array with max length of s
extern array *arr_init(size_t s);

// create and return a new copy of the token array with size 's', init new token array if 'fa' is null
extern array *arr_copy(array *arr, size_t s);

// free the memory used by the token array
extern void arr_free(array *arr);

// reset the state of the array to init
extern void arr_reset(array *arr);

// insert a token into the token array, return true if successful
extern bool arr_push(array *arr, token *t);

// concatenate the src token array onto the end of the dest token array
extern bool arr_concat(array *dest, array *src);

// peek at the head of the token array
extern token *arr_peek(array *arr);

// pop the head of the token array
extern token *arr_pop(array *arr);

// print all the tokens in the token array
extern void arr_print(array *arr, FILE *out);

// sort the token array
extern void arr_sort(array *arr);

#endif // ARRAY