#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    bool neg;
    int whole;
    int numer;
    int denom;
    float value;
} fraction;

static const size_t size_f = sizeof(fraction);

typedef struct {
    size_t num_fs;
    size_t size;
    fraction *fs[];
} heap;

heap & build_heap(size_t size);
bool insert(heap &h, fraction &f);
fraction * pop(heap &h);
bool comp(fraction *lhs, fraction *rhs);

int main(void) {
    char c;
    while ((c = getchar())!= EOF) {
        if (ferror(stdin)) {
            perror("shit blew up");
        }
        else {
            putchar(c);
        }
    }
    return 0;
}

heap & build_heap(size_t size) {
    heap h;
    h.num_fs = 0;
    h.size = size;
    h.fs = malloc(size * size_f);

    return h;
}

bool insert(heap &h, fraction *f) {
    if (h.num_fs == h.size - 1) {
        size_t new_size = size << 4;
        fraction *new_fs;
        if ((new_fs = malloc(new_size * size_f)) == NULL) {
            return false;
        }
        memmove(new_fs, h.fs, h.size * size_f);
        free(h.fs);
        h.fs = new_fs;
        h.size = new_size;
    }

    h.fs[++h.num_fs] = f;

    size_t curr = h.num_fs;
    size_t half = h.num_fs >> 1;
    while (half > 0 && comp(h.fs[half], h.fs[curr])) {
        fraction *tmp = h.fs[half];
        h.fs[half] = h.fs[curr];
        h.fs[curr] = tmp;

        curr = half;
        half = half >> 1;
    }

    return true;
}

bool comp(fraction *lhs, fraction *rhs) {
    return lhs->value > rhs->value;
}
