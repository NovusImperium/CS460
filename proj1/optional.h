#include "defs.h"

#ifndef OPTIONAL
typedef struct {
    bool e;
    union {
        void *val;
        int err;
    };
} optional;
#endif  // OPTIONAL