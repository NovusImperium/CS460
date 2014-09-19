#include <stdio.h>
#include <stdlib.h>
#include "heap_test.h"

int main(int argc, char **argv) {
    heap *h = h_init(NULL);

    int i = 16;
    for (i; i > 0; i--) {
        int *ip = malloc(sizeof(int));
        *ip = i;
        h_push(h, ip);
    }

    i = 1;
    int *ip;
    while ((ip = h_pop(h)) != null) {
        printf("%d: %d @%x\n", i, *ip, ip);
        i++;
    }

    return 0;
}