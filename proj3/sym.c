#include "sym.h"
#include "hashmap.h"
#include "set.h"

#include <string.h>
#include <malloc.h>

struct sym {
    char id[32];
    value val;
};

static set *s;
static FILE *out;

// hash function for sym*
static inline unsigned hash(void *);

// equality comparison function for sym*
static inline bool cmp(void *, void *);

static inline void *sort(void *a);

static inline void *print(void *a);

inline optional init_sym() {
    return hashmap_init(1024, hash, cmp);
}

inline optional get_sym(table *t, char *id) {
    return hashmap_get(t, id);
}

inline bool insert_sym(table *t, char *id, value val) {
    sym *s = malloc(sizeof(sym));
    memset(s,0,sizeof(sym));
    strcpy(s->id, id);
    s->val = val;
    return hashmap_insert(t, s, id);
}

inline value get_value(sym *s) {
    return s->val;
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

static inline unsigned hash(void *a) {
    register char *str = ((sym*)a)->id;
    register unsigned h = 0;

    register int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) ^ c;
    }

    return h & 0x3ff;
}

static inline bool cmp(void *a, void *b) {
    char *c1 = (char*)a, *c2 = (char*)b;
    sym *s1 = (sym*)a, *s2 = (sym*)b;
    return strcmp(c1, c2) == 0;
}

static inline void *sort(void *a) {
    set_push(s, a);
    return a;
}

static inline void *print(void *a) {
    sym *sm = (sym *) a;
    char *val;
    if (sm->val.flag) {
        asprintf(&val, "%ll", sm->val.ival);
    } else {
        asprintf(&val, "%f", sm->val.dval);
    }
    fprintf(out, "%s = %s\n", sm->id, val);

    free(val);
    free(a);
    return null;
}
