#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

typedef struct { 
    bool neg;
    int whole;
    int numer;
    int denom;
    float value;
} fraction;

static const size_t f_ptr = sizeof(fraction*);
static const size_t f_size = sizeof(fraction);
static fraction *d;

typedef struct {
    size_t num_fs;
    size_t size;
    fraction **fs;
} heap;

heap * build_heap(size_t size);
bool push(heap *h, fraction *f);
fraction * pop(heap *h);

fraction * reduce(fraction *f);
bool comp(fraction *lhs, fraction *rhs);
int gcd(int u, int v);

fraction * parse_line(char c);
void print_fs(heap *h);

int main(void) {
    d = malloc(f_size);
    d->neg = false;
    d->whole = 0;
    d->numer = 0;
    d->denom = 1;
    d->value = 0.00f;

    heap *h = build_heap(100);
    char c = getc(stdin);
    while (c != EOF) {
        fraction *f;
        if ((f = parse_line(c)) == NULL) {
            return -1;
        }
        push(h, f);
        char c = getc(stdin);
    }
    print_fs(h);

    return 0;
}

heap * build_heap(size_t size) {
    heap *h = malloc(sizeof(heap));
    h->num_fs = 0;
    h->size = size;
    h->fs = malloc(size * f_ptr);

    return h;
}

bool push(heap *h, fraction *f) {
    if (h->num_fs == h->size - 1) {
        size_t new_size = h->size << 4;
        fraction **new_fs;
        if ((new_fs = malloc(new_size * f_ptr)) == NULL) {
            return false;
        }
        memmove(new_fs, h->fs, h->size * f_ptr);
        free(h->fs);
        h->fs = new_fs;
        h->size = new_size;
    }

    h->fs[++h->num_fs] = f;

    size_t curr = h->num_fs;
    size_t half = h->num_fs >> 1;
    while (half > 0 && comp(h->fs[half], h->fs[curr])) {
        fraction *tmp = h->fs[half];
        h->fs[half] = h->fs[curr];
        h->fs[curr] = tmp;

        curr = half;
        half = half >> 1;
    }

    return true;
}

fraction * pop(heap *h) {
    if (h->num_fs == 0) {
        return NULL;
    }

    fraction *f = h->fs[1];

    size_t curr = 1;
    size_t l = 2;
    size_t r = 3;
    while (curr < h->num_fs) {
        if (r < h->num_fs) {
            if (comp(h->fs[l], h->fs[r])) {
                h->fs[curr] =  h->fs[r];
                curr = r;
            }
            else {
                h->fs[curr] =  h->fs[l];
                curr = l;
            }
        }
        else if (l < h->num_fs) {
            h->fs[curr] = h->fs[l];
            curr = l;
        }
        else {
            break;
        }

        l = curr << 1;
        r = l + 1;
    }
    
    h->num_fs--;
    return f;
}

fraction * parse_line(char c) {
    fraction * f = malloc(f_size);
    memcpy(f, d, f_size);

    if (c == '-') {
        f->neg = true;
        c = getc(stdin);
        while (c >= '0' && c <= '9') {
            f->whole = f->whole * 10 + (c - '0');
            c = getc(stdin);
        }

        c = getc(stdin);
        while (c >= '0' && c <= '9') {
            f->numer = f->numer * 10 + (c - '0');
            c = getc(stdin);
        }

        c = getc(stdin);
        while (c >= '0' && c <= '9') {
            f->denom = f->denom * 10 + (c - '0');
            c = getc(stdin);
        }
    }
    else {
        while (c >= '0' && c <= '9') {
            f->whole = f->whole * 10 + (c - '0');
            c = getc(stdin);
        }

        if (c == '-') {
            f->neg = true;
            c = getc(stdin);
        }

        c = getc(stdin);
        while (c >= '0' && c <= '9') {
            f->numer = f->numer * 10 + (c - '0');
            c = getc(stdin);
        }

        c = getc(stdin);
        while (c >= '0' && c <= '9') {
            f->denom = f->denom * 10 + (c - '0');
            c = getc(stdin);
        }
    }

    return f;
}

void print_fs(heap *h) {
    fraction *f;
    while ((f = pop(h)) != NULL) {
        if (f->neg) {
            if (f->whole == 0) {
                printf("0 -%d/%d", f->numer, f->denom);
            }
            else {
                printf("-%d %d/%d", f->whole, f->numer, f->denom);
            }
        }
        else {
            printf("%d %d/%d", f->whole, f->numer, f->denom);
        }
    }
}

bool comp(fraction *lhs, fraction *rhs) {
    return lhs->value > rhs->value;
}

fraction * reduce(fraction *f) {
    f->whole += f->numer % f->denom;
    int g = gcd(f->numer/f->denom, f->denom);
    f->numer /= g;
    f->denom /= g;

    f->value = (f->neg ? -1 : 1) * (f->whole + ((float) f->numer) / f->denom);

    return f;
}

int gcd(int u, int v) {
    int shift;

    /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
    if (u == 0) return v;
    if (v == 0) return u;

    /* Let shift := lg K, where K is the greatest power of 2
     * dividing both u and v. */
    for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
    }

    while ((u & 1) == 0)
        u >>= 1;

    /* From here on, u is always odd. */
    do {
        /* remove all factors of 2 in v -- they are not common */
        /* note: v is not zero, so while will terminate */
        while ((v & 1) == 0)  /* Loop X */
            v >>= 1;

        /* Now u and v are both odd. Swap if necessary so u <= v,
         * then set v = v - u (which is even). For bignums, the
         * swapping is just pointer movement, and the subtraction
         * can be done in-place. */
        if (u > v) {
            int t = v; v = u; u = t;  // Swap u and v.
        }
        v = v - u;  // Here v >= u.
    } while (v != 0);

    /* restore common factors of 2 */
    return u << shift;
}
