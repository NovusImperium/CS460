#ifndef CONTAINER_TYPES
#define CONTAINER_TYPES

typedef enum {
    array_t = 0,
    threadarray_t,
    bst_t,
    threadbst_t,
    bsa_t,
    threadbsa_t,
    heap_t,
    threadheap_t,
    list_t,
    threadlist_t,
    map_t,
    threadmap_t,
    num_containers,
} container_t;

typedef enum {
    no_cerr = 0,
    malloc_fail = -1,
    init_lock_fail,
    get_lock_fail,
    container_empty,
    out_of_bounds,
    invalid_container_type,
} container_err_t;

#endif  // CONTAINER_TYPES