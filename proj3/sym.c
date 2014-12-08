#include "sym.h"
#include "hashmap.h"
#include "set.h"
#include "defs.h"

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <zlib.h>

struct sym {
    char id[32];
    value val;
};

struct table {
    hashmap *syms;
    hashmap *lits;
    array *tmps;
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
    table *t;
    optional opt;
    if ((t = malloc(sizeof(table))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
    }

    opt = hashmap_init(1024, hash, cmp);
    if (!opt.e) {
        free(t);
        return opt;
    }
    t->syms = opt.val;

    opt = hashmap_init(1024, hash, cmp);
    if (!opt.e) {
        hashmap_free(t->syms);
        free(t);
        return opt;
    }
    t->lits = opt.val;

    opt = arr_init(1024);
    if (!opt.e) {
        hashmap_free(t->syms);
        hashmap_free(t->lits);
        free(t);
        return opt;
    }
    t->tmps = opt.val;

    opt.val = t;
    return opt;
}

inline optional get_sym(table *t, char *id) {
    if (*id == '$') {
        return arr_get(t->tmps, (unsigned) atoi(&id[1]));
    } else if (isdigit(*id) || *id == '.') {
        return hashmap_get(t->lits, id);
    } else {
        return hashmap_get(t->syms, id);
    }
}

inline optional create_sym(table *t, char *id, value val) {
    optional opt;
    if (isdigit(*id) || *id == '.') {
        if (!(opt = hashmap_get(t->lits, id)).e) {
            sym *s = malloc(sizeof(sym));
            memset(s, 0, sizeof(sym));
            strcpy(s->id, id);
            s->val = val;
            if (!hashmap_insert(t->lits, s, id)) {
                opt.e = true;
                opt.val = s;
            } else {
                opt.e = false;
                opt.err = malloc_fail;
            }
        }
    } else {
        if (hashmap_get(t->syms, id).e) {
            opt.e = false;
            opt.err = malloc_fail;
        } else {
            sym *s = malloc(sizeof(sym));
            memset(s, 0, sizeof(sym));
            strcpy(s->id, id);
            s->val = val;
            if (!hashmap_insert(t->syms, s, id)) {
                opt.e = true;
                opt.val = s;
            } else {
                opt.e = false;
                opt.err = malloc_fail;
            }
        }
    }

    return opt;
}

inline optional create_temp(table *t, value val) {
    sym *s = malloc(sizeof(sym));
    memset(s, 0, sizeof(sym));
    sprintf(s->id, "$%u", arr_size(t->tmps));
    s->val = val;

    optional opt;
    if ((opt.e = arr_push(t->tmps, s))) {
        opt.val = s;
    } else {
        opt.err = malloc_fail;
        free(s);
    }

    return opt;
}

inline bool update_sym(table *t, sym *s, value val) {
    if (*s->id == '$') {
        s->val = val;
        return true;
    } else if (isdigit(*s->id) || *s->id == '.') {
        return false;
    } else if (s->val.flag == val.flag) {
        s->val = val;
        return true;
    } else {
        return false;
    }
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

    out = o;
    optional opt = set_init(cmp);
    if (opt.e) {
        s = opt.val;

        fputs("Symbols found: \n", out);
        hashmap_foreach(t->syms, sort);
        set_foreach(s, print);
        set_free(s);
    }

    opt = set_init(cmp);
    if (opt.e) {
        s = opt.val;

        fputs("Temporaries used: \n", out);
        arr_foreach(t->tmps, sort);
        set_foreach(s, print);
        set_free(s);
    }

    opt = set_init(cmp);
    if (opt.e) {
        s = opt.val;

        fputs("Literals found: \n", out);
        hashmap_foreach(t->lits, sort);
        set_foreach(s, print);
        set_free(s);
    }
}

static inline unsigned hash(void *a) {
    register char *str = ((sym *) a)->id;
    register unsigned h = 0;

    register int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) ^ c;
    }

    return h & 0x3ff;
}

static inline bool cmp(void *a, void *b) {
    char *c1 = (char *) a, *c2 = (char *) b;
    return strcmp(c1, c2) == 0;
}

static inline void *sort(void *a) {
    set_push(s, a);
    return a;
}

static inline void *print(void *a) {
    sym *sm = (sym *) a;
    if (sm->val.flag) {
        fprintf(out, "%s = %ll\n", sm->id, sm->val.ival);
    } else {
        fprintf(out, "%s = %f\n", sm->id, sm->val.dval);
    }

    return a;
}
