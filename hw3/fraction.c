#include <stdlib.h>
#include "defs.h"
#include "fraction.h"
#include "gcd.h"

// reduce a fraction into lowest terms
void f_reduce(fraction *f) {
    int d = gcd((f->n < 0) ? -1 * f->n : f->n, f->d);
    f->n /= d;
    f->d /= d;
}

// add the right-hand fraction into the left-hand fraction
void f_add(fraction *lhs, fraction *rhs) {
    lhs->n = lhs->n * rhs->d + rhs->n * lhs->n;
    lhs->d = lhs->d * rhs->d;
    f_reduce(lhs);
}

// compare the two fractions and return true if lhs is less-than rhs
bool f_lt(const fraction *lhs, const fraction *rhs) {
    return lhs->n * rhs->d < rhs->n * lhs->d;
}

// initialize and read a fraction from the input file
bool f_read(fraction *f, FILE *in) {
    char c = (char) getc(in);
    bool neg = false;
    int w = 0;
    int n = 0;
    int d = 1;
    if (c == '-') {
        neg = true;
        c = (char) getc(in);
        while (c >= '0' && c <= '9') {
            w = w * 10 + (c - '0');
            c = (char) getc(in);
        }

        c = (char) getc(in);
        while (c >= '0' && c <= '9') {
            n = n * 10 + (c - '0');
            c = (char) getc(in);
        }

        c = (char) getc(in);
        while (c >= '0' && c <= '9') {
            d = d * 10 + (c - '0');
            c = (char) getc(in);
        }
    }
    else if (c >= '0' && c <= '9') {
        while (c >= '0' && c <= '9') {
            w = w * 10 + (c - '0');
            c = (char) getc(in);
        }

        c = (char) getc(in);
        if (c == '-') {
            neg = true;
            c = (char) getc(in);
        }

        c = (char) getc(in);
        while (c >= '0' && c <= '9') {
            n = n * 10 + (c - '0');
            c = (char) getc(in);
        }

        c = (char) getc(in);
        while (c >= '0' && c <= '9') {
            d = d * 10 + (c - '0');
            c = (char) getc(in);
        }
    }
    else if (c == EOF) {
        f->d = 0;
        return false;
    }
    else {
        return false;
    }

    f->n = (neg ? -1 : 1) * (n + w * d);
    f->d = d;
    f_reduce(f);
    return true;
}

// print the fraction to the output file
void f_print(fraction *f, FILE *out) {
    bool neg = (f->n < 0);
    int w = (neg ? -1 : 1) * (f->n / f->d);
    int n = (neg ? -1 : 1) * (f->n % f->d);
    int d = f->d;

    if (neg) {
        if (w == 0) {
            fprintf(out, "0 -%d/%d\n", n, d);
        }
        else {
            fprintf(out, "-%d %d/%d\n", w, n, d);
        }
    }
    else {
        fprintf(out, "%d %d/%d\n", w, n, d);
    }
}