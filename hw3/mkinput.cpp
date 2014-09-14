#include <iostream>

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96(void) {          //period 2^96-1
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

static const int min = 1;
static const int max = 19;
static const int mod = min + max - 1;
static const int mid = (min + max) / 2;
static const int num = 1 << 24;

int rand(void) {
    return (int) (xorshf96() % mod);
}

int drand(void) {
    return (int) (xorshf96() % 7) + 2;
}

int main(void) {

    for (int i = 0; i < num; ++i) {
        bool neg = rand() < mid;
        int w = rand();
        int n = rand();
        int d = drand();

//        int n = rand();
//        int w = 0;
//        int d = 1;

        if (neg) {
            if (!w) {
                n = -1 * n;
            }
            else {
                w = -1 * w;
            }
        }

        std::cout << w << " " << n << "/" << d << std::endl;
    }

    return 0;
}

