#include "sym.h"
#include "hashmap.h"
#include "set.h"

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

static set *s;
static FILE *out;

// hash function for char*
static unsigned hash(void *);

// equality comparison function for char*
static bool cmp(void *, void *);

static void *sort(void *a);

static void *print(void *a);

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
    opt.e = s->flag;
    opt.val = (void *) &s->ival;
    return opt;
}

inline char *get_id(sym *s) {
    return s->id;
}

inline void write_syms(table *t, FILE *o) {
    int cmp(void *a, void *b) {
        return strcmp((char *) a, (char *) b);
    }
    optional opt = set_init(cmp);
    if (opt.e) {
        out = o;
        s = opt.val;
        hashmap_foreach(t, sort);
        set_foreach(s, print);
        set_free(s);
    }
}

static unsigned hash(void *a) {
    char *str = (char *) a;
    unsigned h = 0;

    int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) ^ c;
    }

    return h & 0x3ff;
}

static bool cmp(void *a, void *b) {
    return strcmp((char *) a, (char *) b) == 0;
}

static void *sort(void *a) {
    set_push(s, a);
    return a;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"

static void *print(void *a) {
    sym *sm = (sym *) a;
    char *val;
    if (sm->flag) {
        asprintf(&val, "%d", sm->ival);
    } else {
        asprintf(&val, "%f", sm->dval);
    }
    fprintf(out, "%s = %s", sm->id, val);

    free(val);
    free(a);
    return null;
}

#pragma clang diagnostic pop
