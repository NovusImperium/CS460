#ifndef ARRAY
#define ARRAY

#include "defs.h"
#include "optional.h"
#include "lexer_dfa.h"

typedef struct array array;

// initialize the item array with max length of m
extern optional arr_init(unsigned m);

// create and return a new copy of the item array with size 'm', init new item array if 'arr' is null
extern optional arr_copy(array *arr, unsigned m);

// free the memory used by the item array
extern void arr_free(array *arr);

// return the number of elements in the array
extern unsigned arr_size(array *arr);

// get the element at the index 'i'
extern optional arr_get(array *arr, unsigned i);

// insert the element at the index 'i'
extern bool arr_insert(array *arr, token *t, unsigned i);

// insert a item into the item array, return true if successful
extern bool arr_push(array *arr, token *t);

// concatenate the src item array onto the end of the dest item array
extern bool arr_concat(array *dest, array *src);

// peek at the head of the item array
extern optional arr_peek(array *arr);

// pop the head of the item array
extern optional arr_pop(array *arr);

// apply the function to each item in the array
extern int arr_foreach(array *arr, token * (*func)(token *));

// remove the items marked with false by the function, returns the new size of the array
extern int arr_reduce(array *arr, optional (*func)(token *));

#endif // ARRAY