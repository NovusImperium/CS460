#include <stdlib.h>
#include "fraction.h"
#include "bst.h"

int main(int argc, char **argv) {
    FILE *fd = fopen(argv[1], "r");
    bst *b = bst_init();
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
                bst_push(b, f);
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

    bst_print(b, stdout, 1);

    return 0;
}
