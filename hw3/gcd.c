#include "gcd.h"

// code borrowed from:
// http://www.mathblog.dk/gcd-faceoff/#comment-86852
int gcd(int x, int y) {
     while (x && y) {
        if (x >= y) {
            x %= y;
        }
        else {
            y %= x;
        }
        if (y == 1 || x == 1) {
            return 1;
        }
    }

    return x + y;
}

