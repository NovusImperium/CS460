#include "sym.h"
#include "hashmap.h"

#include <string.h>
#include <malloc.h>

struct sym {
    char *id;
    bool flag;
    union {
        long long ival;
        double dval;
    };
};

// hash function for char*
static unsigned hash(void *);

// equality comparison function for char*
static bool cmp(void *, void *);

inline optional init_sym() {
    return hashmap_init(1024, hash, cmp);
}

inline optional get_sym(table *t, char *id) {
    return hashmap_get(t, id);
}

inline bool insert_sym(table *t, char *id, optional val) {
    sym *s = malloc(sizeof(sym));
    s->id = id;
    if ((s->flag = val.e)) {
        s->ival = *(long long *) val.val;
    } else {
        s->dval = *(double *) val.val;
    }
    return hashmap_insert(t, s, id);
}

inline optional get_value(sym *s) {
    optional opt;
    opt.val = (opt.e = s->flag) ? &s->ival : &s->dval;
    return opt;
}

inline char *get_id(sym *s) {
    return s->id;
}

inline void write_syms(table *t, FILE *out) {

}

static unsigned hash(void *a) {
    char *str = (char *) a;
    unsigned h = 0;

    int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) ^ c;
    }

    return h;
}

static bool cmp(void *a, void *b) {
    return strcmp((char *) a, (char *) b) == 0;
}
