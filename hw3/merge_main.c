#include <stdlib.h>
#include "fraction.h"
#include "farray.h"

int main(int argc, char **argv) {
    FILE *in = fopen(argv[1], "r");
    FILE *out = argc > 2 ? fopen(argv[2], "w") : stdout;
    farr *fa = fa_init();
    fraction *sum = malloc(f_size);
    sum->n = 0;
    sum->d = 1;

    while (true) {
        fraction *f = malloc(f_size);
        f->n = 0;
        f->d = 1;
        if (f_read(f, in)) {
            if (f->d != 0) {
                f_add(sum, f);
                fa_push(fa, f);
            } else {
                break;
            }
        } else {
            return -1;
        }
    }

    fa = fa_sort(fa);

    printf("The sum of the fractions is: ");
    f_print(sum, out);
    fa_print(fa, out);

    return 0;
}
