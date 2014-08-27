#include <iostream>

static unsigned long x=123456789, y=362436069, z=521288629;

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
static const int max = 9999;
static const int mod = min + max - 1;
static const int mid = (int) ((min + max) / 2);
static const int num = 1 << 16;

int rand(void) {
    return (xorshf96() % mod) + 1;
}

int main(void) {

    for (int i = 0; i < num; ++i) {
        bool neg = rand() < mid;
        bool zrw = rand() < mid;
        bool zrn = rand() < mid;

        if (neg) {
            if (zrw) {
                std::cout << "0 -" << rand() << "/" << rand() << std::endl;
            }
            else if (zrn) {
                std::cout << "-" << rand() << " 0/" << rand() << std::endl;
            }
            else {
                std::cout << "-" << rand() << " " << rand() << "/" << rand() << std::endl;
            }
        }
        else {
            if (zrw) {
                std::cout << "0 " << rand() << "/" << rand() << std::endl;
            }
            else if (zrn) {
                std::cout << rand() << " 0/" << rand() << std::endl;
            }
            else {
                std::cout << rand() << " " << rand() << "/" << rand() << std::endl;
            }
        }
    }

    return 0;
}

