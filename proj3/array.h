#ifndef ARRAY
#define ARRAY

#include "defs.h"

typedef struct array array;

// initialize the item array with max length of m
extern inline optional arr_init(unsigned s);

// create and return a new copy of the item array with size 'm', init new item array if 'arr' is null
extern inline optional arr_copy(array *arr, unsigned s);

// free the memory used by the item array
extern inline void arr_free(array *arr);

// return the number of elements in the array
extern inline unsigned arr_size(array *arr);

// get the element at the index 'i'
extern inline optional arr_get(array *arr, unsigned i);

// set the element at the index 'i'
// returns false if the index does not already contain an element
extern inline bool arr_set(array *arr, void *a, unsigned i);

// insert the element at the index 'i'
extern inline bool arr_insert(array *arr, void *t, unsigned i);

// insert a item into the item array, return true if successful
extern inline bool arr_push(array *arr, void *t);

// concatenate the src item array onto the end of the dest item array
extern inline bool arr_concat(array *dest, array *src);

// peek at the tail of the item array
extern inline optional arr_peek(array *arr);

// pop the tail of the item array
extern inline optional arr_pop(array *arr);

// apply the function to each item in the array
extern inline void arr_foreach(array *arr, void *(*func)(void *));

// remove the items marked with false by the function, returns the new size of the array
extern inline int arr_reduce(array *arr, optional (*func)(void *));

#endif // ARRAY
