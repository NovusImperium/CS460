// CS460 -- HW1-2
// Amandeep Gill
// read, parse, sort, and display a list of fraction in proper reduced form

#include <sstream>
#include <iostream>
#include <vector>

/* struct to hold the logical representation of the fraction */
typedef struct {
    bool neg;   // flag set to 'true' if the number is negative
    unsigned int whole;
    unsigned int num;
    unsigned int den;
    double dnum;
} fraction;
// Convert the fraction into a string for output
std::string print_fraction(const fraction &f);
// less-than operator overide for the fraction struct
bool operator<(const fraction &lhs, const fraction &rhs) { return lhs.dnum < rhs.dnum; }
// greater-than operator overide for the fraction struct
bool operator>(const fraction &lhs, const fraction &rhs) { return lhs.dnum > rhs.dnum; }
// equality operator overide for the fraction struct
bool operator==(const fraction &lhs, const fraction &rhs) { return lhs.dnum == rhs.dnum; }

struct tree{
    tree * r;
    tree * l;
    fraction f;
};

/* Misc functions for dealing with math and fractions */
// find and return the greatest common divisor of ints a and b
unsigned int gcd(unsigned int u, unsigned int v);
// reduce the number to a proper fraction
void reduce(fraction &f);

/* Functions for handling console IO */
// read and parse a line from stdin, return true/false if read was possible
bool readline(fraction &f);
// Print all fractions to the console
void print_fractions(std::vector<fraction> &fs);

int main(void) {
    std::vector<fraction> fs;

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
     * dividing both u and v. */
    for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
    }

    while ((u & 1) == 0)
        u >>= 1;

    /* From here on, u is always odd. */
    do {
        /* remove all factors of 2 in v -- they are not common */
        /* note: v is not zero, so while will terminate */
        while ((v & 1) == 0)  /* Loop X */
            v >>= 1;

        /* Now u and v are both odd. Swap if necessary so u <= v,
         * then set v = v - u (which is even). For bignums, the
         * swapping is just pointer movement, and the subtraction
         * can be done in-place. */
        if (u > v) {
            unsigned int t = v; v = u; u = t;  // Swap u and v.
        }
        v = v - u;  // Here v >= u.
    } while (v != 0);

    /* restore common factors of 2 */
    return u << shift;
}

// insert the fraction into bst, insertion code borrowed from SO
// http://stackoverflow.com/questions/18774858/using-binary-search-with-vectors
void insert(tree * fs, fraction &f) {
    // step thru tree and insert
}

void reduce(fraction &f) {
    if (f.num >= f.den) {
        f.whole += f.num / f.den;
        f.num = f.num % f.den;
    }

    if (f.num > 0) {
        unsigned int d = gcd(f.num, f.den);
        f.num /= d;
        f.den /= d;
    }
    else {
        f.den = 1;
    }

    if (f.neg) {
        f.neg = f.whole != 0 || f.num != 0;
    }

    f.dnum = (f.neg ? -1 : 1) * (f.whole + f.num / f.den);
}

bool readline(fraction &f) {
    std::string line;
    if (getline(std::cin, line)) {
        std::stringstream ss(line);
        int whole, num, den;
        char div;
        if ((ss >> whole) && (ss >> num) && (ss >> div) && (ss >> den)) {
            if (whole < 0 || (whole == 0 && num < 0)) {
                f.neg = true;
            }
            f.whole = (unsigned int) whole;
            f.num = (unsigned int) num;
            f.den = (unsigned int) den;
            reduce(f);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

std::string print_fraction(const fraction &f) {
    std::stringstream s;

    if (f.neg) {
        if (f.whole == 0) {
            s << f.whole << " -" << f.num << "/" << f.den;
        }
        else {
            s << "-" << f.whole << " " << f.num << "/" << f.den;
        }
    }
    else {
        s << f.whole << " " << f.num << "/" << f.den;
    }

    return s.str();
}

void print_fractions(std::vector<fraction> &fs) {
    for (unsigned int i = 0; i < fs.size(); i++) {
        std::cout << print_fraction(fs[i]) << std::endl;
    }
}
