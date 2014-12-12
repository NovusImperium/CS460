#include "defs.h"
#include "set.h"

#include <malloc.h>

typedef struct node node;
struct node {
    void *t;
    node *r;
    node *l;
};

struct set {
    unsigned n;

    int (*cmp)(void *, void *);

    node *root;
};

static inline bool node_insert(node *n, void *t, int (*cmp)(void *, void *));

static inline void node_free(node *n);

static inline void node_foreach(node *n, void *(*func)(void *));

optional set_init(int (*cmp)(void *, void *)) {
    optional opt;
    if ((opt.e = (opt.val = malloc(sizeof(set *))) != null)) {
        (*(set *) opt.val).n = 0;
        (*(set *) opt.val).cmp = cmp;
        (*(set *) opt.val).root = null;
    } else {
        opt.err = malloc_fail;
    }
    return opt;
}

void set_free(set *s) {
    if (s->n > 0) {
        node_free(s->root);
    }
    free(s);
}

unsigned set_size(set *s) {
    return s->n;
}

bool set_push(set *s, void *t) {
    if (s->n > 0) {
        return node_insert(s->root, t, s->cmp) ? s->n++ : false;
    } else if ((s->root = malloc(sizeof(node *))) == null) {
        return false;
    } else {
        s->root->t = t;
        s->root->l = null;
        s->root->r = null;
        s->n++;
        return true;
    }
}

optional set_peek(set *s) {
    optional opt;
    if ((opt.e = s->n > 0)) {
        opt.val = s->root->t;
        return opt;
    } else {
        opt.err = container_empty;
        return opt;
    }
}

optional set_pop(set *s) {
    optional opt;
    if ((opt.e = s->n > 0)) {
        node *curr = s->root;
        if (curr->l == null) {
            opt.val = s->root->t;
            node *tmp = s->root->r;
            free(s->root);
            s->root = tmp;
            s->n--;
            return opt;
        } else {
            for (curr; curr->l->l != null; curr = curr->l) {}
            opt.val = curr->l->t;
            node *tmp = curr->l->r;
            free(curr->l);
            curr->l = tmp;
            s->n--;
            return opt;
        }
    } else {
        opt.err = container_empty;
        return opt;
    }
}

void set_foreach(set *s, void *(*func)(void *)) {
    if (s->n > 0) {
        node_foreach(s->root, func);
    }
}

static inline bool node_insert(node *n, void *t, int (*cmp)(void *, void *)) {
    int c = cmp(t, n->t);
    if (c == 0) {
        return false;
    }

    node **tmp = c > 0 ? &n->r : &n->l;
    if ((*tmp) == null) {
        *tmp = malloc(sizeof(node *));
        (*tmp)->t = t;
        (*tmp)->l = null;
        (*tmp)->r = null;
        return true;
    } else {
        return node_insert(*tmp, t, cmp);
    }
}

static inline void node_free(node *n) {
    if (n != null) {
        node_free(n->l);
        node_free(n->r);
        if (n->t != null) {
            free(n->t);
        }
        free(n);
    }
}

static inline void node_foreach(node *n, void *(*func)(void *)) {
    if (n != null) {
        node_foreach(n->l, func);
        n->t = func(n->t);
        node_foreach(n->r, func);
    }
}
