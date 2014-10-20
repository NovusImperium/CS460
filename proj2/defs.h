#ifndef bool
#define bool int
#define true 1
#define false 0
#endif  // bool

#ifndef null
#define null (void*)0
#endif

#ifndef OPTIONAL
typedef struct {
    bool e;
    union {
        void *val;
        int err;
    };
} optional;
#endif  // OPTIONAL

#ifndef CONTAINER_ERR_TYPES
#define CONTAINER_ERR_TYPES

typedef enum {
    no_cerr = 0,
    malloc_fail = -10,
    init_lock_fail,
    get_lock_fail,
    container_empty,
    out_of_bounds,
    invalid_container_type,
} container_err_t;

#endif  // CONTAINER_ERR_TYPES
