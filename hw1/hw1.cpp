#include <iostream>
#include <string>

typedef struct {
    bool neg;   // flag set to 'true' if the number is negative
    unsigned int whole;
    unsigned int num;
    unsigned int den;
} fraction;

// find and return the greatest common divisor of ints a and b
unsigned int gcd(unsigned int u, unsigned int v);
// reduce the number to a proper fraction
void reduce(fraction &f);
// convert the string into an int
void consume(std::string &s);

/* finite state machine to parse the string and build the fraction simultaneously */
// init state (0)
unsigned int s0(char c, std::string &s, fraction &f) {
    // check if c is unsigned int, epsilon move to whole number state (1)
    if (c >= '1' && c <= '9') {
       return s1(c, s, f);
    }
    switch (c) {
        // continue to consume leading spaces, loopback
        case ' ':
            return 0;
        // set the negative flag, move to negative whole number state (1)
        case '-':
            f.neg = true;
            return 1;
        // consume leading zero, move to fraction only state transition state (3)
        case '0':
            return 3;
        // invalid character, move to error state (-1)
        default:
            return -1;
    }
}
// whole number state (1)
unsigned int s1(char c, std::string &s, fraction &f) {
    // check if c is int, push to stack and loopback
    if (c >= '0' && c <= '9') {
        s.push_back(c);
       return 1;
    }
    switch (c) {
        // consume space, 
        case ' ':
            // stack is non-empty: consume stack and move to numerator state (2)
            if (s.size > 0) {
                f.whole = consume(s);
                return 2;
            }
            // stack is empty: move to error state (-1)
            return -1;
        // invalid character, move to error state (-1)
        default:
            return -1;
    }
}
// numerator transition state (2)
unsigned int s2(char c, std::string &s, fraction &f) {
    // consume int, push to stack and move to numerator state (4)
    if (c >= '1' && c <= '9') {
        s.push_back(c);
        return 4;
    }
    switch (c) {
        // consume 0, move to denom transition state (5)
        case '0':
            return 5;
        // invalid character, move to error state (-1)
        default:
            return -1;
    }
}
// fraction only transition state (3)
unsigned int s3(char c, std::string &s, fraction &f) {
    // check if c is int,
    if (c >= '0' && c <= '9') {
        // stack non-empty: epsilon move to numerator transition state (2)
        if (s.size() > 0) {
            s.clear();
            return s2(c, s, f);
        }
        // stack empty: move to error state (-1)
        else {
            return -1;
        }
    }
    switch (c) {
        // consume space, push to stack and loopback
        case ' ':
            s.push_back(c);
            return 3;
        // mark number as negative, move to numerator transition state (2)
        case '-':
            s.clear();
            f.neg = true;
            return 2;
        // invalid character, move to error state (-1)
        default:
            return -1;
    }
}
// numerator state (4)
unsigned int s4(char c, std::string &s, fraction &f) {
    // check if c is int, push to stack and loopback
    if (c >= '0' && c <= '9') {
        s.push_back(c);
       return 4;
    }
    switch (c) {
        // consume div sign, 
        case '/':
            // stack is non-empty: consume stack and move to denom transition state
            if (s.size > 0) {
                f.whole = consume(s);
                return 6;
            }
            // stack is empty: move to error state (-1)
            return -1;
        // invalid character, move to error state (-1)
        default:
            return -1;
    }
}

int main(void) {
    // TODO: things
    return 0;
}

// iterative Stein's Algorithm borrowed from Wikipedia
// http://en.wikipedia.org/wiki/Binary_GCD_algorithm#Iterative_version_in_C
unsigned int gcd(unsigned int u, unsigned int v) {
    int shift;

    /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
    if (u == 0) return v;
    if (v == 0) return u;

    /* Let shift := lg K, where K is the greatest power of 2
     *         dividing both u and v. */
    for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
    }

    while ((u & 1) == 0)
        u >>= 1;

    /* From here on, u is always odd. */
    do {
        /* remove all factors of 2 in v -- they are not common */
        /*   note: v is not zero, so while will terminate */
        while ((v & 1) == 0)  /* Loop X */
            v >>= 1;

        /* Now u and v are both odd. Swap if necessary so u <= v,
         *           then set v = v - u (which is even). For bignums, the
         *                     swapping is just pointer movement, and the subtraction
         *                               can be done in-place. */
        if (u > v) {
            unsigned int t = v; v = u; u = t;}  // Swap u and v.
        v = v - u;                       // Here v >= u.
    } while (v != 0);

    /* restore common factors of 2 */
    return u << shift;
}

void reduce(fraction &f) {
    if (f.num >= f.dem) {
        f.whole += f.num / f.dem;
        f.num = f.num % f.dem;
    }

    if (f.num > 0) {
        unsigned int d = gcd(f.num, f.dem);
        f.num /= d;
        f.dem /= d;
    }
    else {
        f.dem = 1;
    }

    if (f.neg) {
        f.neg = f.whole != 0 || f.num != 0;
    }
}
