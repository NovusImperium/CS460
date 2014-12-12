#include "sym.h"
#include "set.h"

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

struct sym {
    char id[32];
    value val;
};

struct table {
    array *syms;
    array *lits;
    array *tmps;
};

static set *s;
static FILE *out;

// hash function for sym*
static inline unsigned hash(void *);

// equality comparison function for sym*
static inline bool cmp(void *, void *);

static inline void *sort(void *);

static inline void *print(void *);

static inline void *print_temps(void *);

static inline void *print_operators(void *);

inline optional init_sym() {
    table *t;
    optional opt;
    if ((t = malloc(sizeof(table))) == null) {
        opt.e = false;
        opt.err = malloc_fail;
        return opt;
    }

    opt = arr_init(32);
    if (!opt.e) {
        free(t);
        return opt;
    }
    t->syms = opt.val;

    opt = arr_init(32);
    if (!opt.e) {
        arr_free(t->syms);
        free(t);
        return opt;
    }
    t->lits = opt.val;

    opt = arr_init(32);
    if (!opt.e) {
        arr_free(t->syms);
        arr_free(t->lits);
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
        unsigned idx;
        optional opt;
        for (idx = 0; idx < arr_size(t->lits); idx++) {
            if (strcmp(((sym *) (opt = arr_get(t->lits, idx)).val)->id, id) == 0) {
                return opt;
            }
        }

        opt.e = false;
        opt.err = element_not_found;
        return opt;
    } else {
        unsigned idx;
        optional opt;
        for (idx = 0; idx < arr_size(t->syms); idx++) {
            if (cmp((opt = arr_get(t->syms, idx)).val, id)) {
                return opt;
            }
        }

        opt.e = false;
        opt.err = element_not_found;
        return opt;
    }
}

inline optional create_sym(table *t, char *id, value val) {
    optional opt;
    if (isdigit(*id) || *id == '.') {
        sym *s = malloc(sizeof(sym));
        memset(s, 0, sizeof(sym));
        strcpy(s->id, id);
        s->val = val;
        opt.e = true;
        opt.val = s;

        unsigned idx;
        unsigned n = arr_size(t->lits);
        for (idx = 0; idx < n; idx++) {
            if (strcmp(((sym *) arr_get(t->lits, idx).val)->id, id) == 0) {
                break;
            }
        }
        if (idx >= n) {
            arr_push(t->lits, s);
        }
    } else {
        unsigned idx;
        opt.e = true;
        for (idx = 0; idx < arr_size(t->syms); idx++) {
            if (strcmp(((sym *) (opt = arr_get(t->syms, idx)).val)->id, id) == 0) {
                opt.e = false;
                opt.err = element_not_found;
            }
        }

        if (opt.e) {
            sym *s = malloc(sizeof(sym));
            memset(s, 0, sizeof(sym));
            strcpy(s->id, id);

            s->val = val;
            opt.val = s;
            arr_push(t->syms, s);
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

inline bool update_sym(sym *s, value val) {
    if (*s->id == '$' || isdigit(*s->id) || *s->id == '.') {
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

inline void *print_sym(void *a) {
    if (a != null) {
        sym *sm = (sym *) a;
        if (sm->val.flag) {
            printf("%s = %lld\n", sm->id, sm->val.ival);
        } else {
            printf("%s = %4.2f\n", sm->id, sm->val.dval);
        }
    }

    return a;
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
        arr_foreach(t->syms, sort);
        set_foreach(s, print);
        set_free(s);
    }

    fputs("Temporaries used: \n", out);
    arr_foreach(t->tmps, print);

    opt = set_init(cmp);
    if (opt.e) {
        s = opt.val;

        fputs("Literals found: \n", out);
        arr_foreach(t->lits, sort);
        set_foreach(s, print);
        set_free(s);
    }

    opt = set_init(cmp);
    if (opt.e) {
        s = opt.val;
        int i = 0;
        fputs("Symbol Table Operators: \n", out);
        //This is where we need to output the operators
        //Example: ++      POSTPP        1           1       LEFT_TO_RIGHT        INTEGER       NONE
    }
}

inline void *print(void *a) {
    if (a != null) {
        sym *sm = (sym *) a;
        if (sm->val.flag) {
            fprintf(out, "\t%s \tINTTYPE\t%lld\n", sm->id, sm->val.ival);
        } else {
            fprintf(out, "\t%s \tDBLTYPE\t%4.2f\n", sm->id, sm->val.dval);
        }
    }
    return a;
}

inline void *print_temps(void *a) {
    if (a != null) {
        sym *sm = (sym *) a;
        if (sm->val.flag) {
            fprintf(out, "\t%s\tINTTYPE\t\t%lld\n", sm->id, sm->val.ival);
        } else {
            fprintf(out, "\t%s\tDOUBLE\t\t%4.2f\n", sm->id, sm->val.dval);
        }
    }
    return a;
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

