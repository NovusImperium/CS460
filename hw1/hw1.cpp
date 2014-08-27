// CS460 -- HW1-2
// Amandeep Gill
// read, parse, sort, and display a list of fraction in proper reduced form

#include <sstream>
#include <iostream>
#include <cstdlib>

/* struct to hold the logical representation of the fraction */
typedef struct {
    bool neg;   // flag set to 'true' if the number is negative
    int whole;
    int num;
    int den;
    float dnum;
} fraction;
// Convert the fraction into a string for output
std::string print_fraction(const fraction &f);
// less-than operator overide for the fraction struct
bool operator<(const fraction &lhs, const fraction &rhs) { return lhs.dnum < rhs.dnum; }
// greater-than operator overide for the fraction struct
bool operator>(const fraction &lhs, const fraction &rhs) { return lhs.dnum > rhs.dnum; }
// equality operator overide for the fraction struct
bool operator==(const fraction &lhs, const fraction &rhs) { return lhs.dnum == rhs.dnum; }

struct tree_node {
    struct tree_node * r;
    struct tree_node * l;
    fraction f;
};
// typedef to avoid writing 'struct' everywhere
typedef struct tree_node tree;
// insert the fraction into the bst
void insert(tree * root, fraction &f);

/* Misc functions for dealing with math and fractions */
// find and return the greatest common divisor of ints a and b
int gcd(int u, int v);
// reduce the number to a proper fraction
void reduce(fraction &f);

/* Functions for handling console IO */
// read and parse a line from stdin, return true/false if read was possible
bool readline(fraction &f);
// Print all fractions to the console
void print_fractions(tree * root);

//* debug */ static int lnum = 0;
int main(void) {
    // default fraction
    fraction d;
    d.neg = false;
    d.whole = 0;
    d.num = 0;
    d.den = 0;

    tree * fs = new tree;
    fraction tmp = d;
    if (readline(tmp)) {
        //* debug */ ++lnum;
        fs->f = tmp;
        fs->l = NULL;
        fs->r = NULL;

        while (true) {
            fraction f = d;
            if (readline(f)) {
                //* debug */ ++lnum;
                insert(fs, f);
            }
            else {
                break;
            }
        }

        print_fractions(fs);
    }

    return 0;
}

// iterative Stein's Algorithm borrowed from Wikipedia
// http://en.wikipedia.org/wiki/Binary_GCD_algorithm#Iterative_version_in_C
int gcd(int u, int v) {
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
            int t = v; v = u; u = t;  // Swap u and v.
        }
        v = v - u;  // Here v >= u.
    } while (v != 0);

    /* restore common factors of 2 */
    return u << shift;
}

void insert(tree * root, fraction &f) {
    // step thru tree and insert
    while (root != NULL) {
        if (f < root->f) {
            if (root->l == NULL) {
                root->l = new tree;
                root->l->f = f;
                root->l->l = NULL;
                root->l->r = NULL;
                return;
            }
            else {
                root = root->l;
            }
        }
        else {
            if (root->r == NULL) {
                root->r = new tree;
                root->r->f = f;
                root->r->l = NULL;
                root->r->r = NULL;
                return;
            }
            else {
                root = root->r;
            }
        }
    }
}

void reduce(fraction &f) {
    if (f.num >= f.den) {
        f.whole += f.num / f.den;
        f.num = f.num % f.den;
    }

    if (f.num > 0) {
        int d = gcd(f.num, f.den);
        f.num /= d;
        f.den /= d;
    }
    else {
        f.den = 1;
    }

    if (f.neg) {
        f.neg = f.whole != 0 || f.num != 0;
    }

    f.dnum = (f.neg ? -1 : 1) * (((float) f.whole) + f.num / ((float)f.den));
}

bool readline(fraction &f) {
    std::string line;
    //* debug */ std::cout << (f.neg ? '-' : ' ') << f.whole << ' ' << f.num << '/' << f.den << std::endl;
    if (getline(std::cin, line)) {
        std::stringstream ss(line);
        int whole, num, den;
        char div;
        if ((ss >> whole) && (ss >> num) && (ss >> div) && (ss >> den)) {
            //* debug */ std::cout << whole << ' ' << num << '/' << den << std::endl;
            if (whole < 0) {
                f.neg = true;
                f.whole = std::abs(whole);
                f.num = num;
                f.den = den;
            }
            else if (num < 0) {
                f.neg = true;
                f.whole = whole;
                f.num = std::abs(num);
                f.den = den;
            }
            else {
                f.whole = whole;
                f.num = num;
                f.den = den;
            }
            //* debug */ std::cout << (f.neg ? '-' : ' ') << f.whole << ' ' << f.num << '/' << f.den << std::endl;
            reduce(f);
            //* debug */ std::cout << (f.neg ? '-' : ' ') << f.whole << ' ' << f.num << '/' << f.den << std::endl << std::endl;
            return true;
        }
        else {
            //* debug */ std::cout << "parse failed on line#" << lnum << std::endl;
            return false;
        }
    }
    else {
        //* debug */ std::cout << "getline failed on line#" << lnum << std::endl;
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
    //* debug */ s << " : " << f.dnum;
    return s.str();
}

void print_fractions(tree * root) {
    if (root != NULL) {
        print_fractions(root->l);
        std::cout << print_fraction(root->f) << std::endl;
        print_fractions(root->r);
    }
}
