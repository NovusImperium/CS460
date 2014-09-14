#include <stdlib.h>
#include "fraction.h"
#include "farray.h"

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "r");
    farray *fa = fa_init();
    fraction *sum = malloc(f_size);
    sum->n = 0;
    sum->d = 1;

    while (true) {
        fraction *f = malloc(f_size);
        f->n = 0;
        f->d = 1;
        if (f_read(f, fd)) {
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

    fa_sort(fa);

    printf("The sum of the fractions is: ");
    f_print(sum, stdout);
    fa_print(fa, stdout);

    return 0;
}
