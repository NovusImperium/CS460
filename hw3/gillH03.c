#include <stdlib.h>
#include "fraction.h"
#include "heap.h"

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "r");
    heap *h = h_init(NULL);
    fraction *sum = malloc(f_size);
    sum->n = 0;
    sum->d = 1;

    bool cont = true;
    while (cont) {
        fraction *f = malloc(f_size);
        f->n = 0;
        f->d = 1;
        if (f_read(f, fd)) {
            if (f->d != 0) {
                f_add(sum, f);
                h_push(h, f);
            }
            else {
                cont = false;
            }
        }
        else {
            return -1;
        }
    }

    printf("The sum of the fractions is: ");
    f_print(sum, stdout);

    fraction *f;
    while ((f = h_pop(h)) != null) {
        f_print(f, stdout);
    }

    return 0;
}
