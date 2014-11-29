#ifndef bool
#define bool int
#define true 1
#define false 0
#endif  // bool

#ifndef null
#define null (void*)0
#endif

#ifndef OPTIONAL
#define OPTIONAL
typedef struct {
    bool e;
    union {
        void *val;
        int err;
    };
} optional;
#endif  // OPTIONAL

#ifndef VALUE_TYPE
#define VALUE_TYPE
typedef struct {
    bool flag;
    union {
        long long ival;
        double dval;
    };
} value;
#endif  // VALUE_TYPE

#ifndef CONTAINER_ERR_TYPES
#define CONTAINER_ERR_TYPES
typedef enum {
    no_cerr = 0,
    malloc_fail = -10,
    container_empty,
    out_of_bounds,
    invalid_container_type,
} container_err_t;
#endif  // CONTAINER_ERR_TYPES
