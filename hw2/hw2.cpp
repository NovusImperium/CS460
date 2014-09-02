// CS460 -- HW2
// Amandeep Gill
// read, parse, sort, and display a list of fraction in proper reduced form
// written in "idiomatic" c++

#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <cmath>
#include <algorithm>

/* class declaration for Fraction */
class Fraction {
    private:
        int whole, numer, denom;
        bool neg;
        float value;

        // reduce the fraction into lowest, proper form
        void reduce();

    public:
        // initializes the fraction as 0+0/1 and 0.00
        Fraction();
        // copy constructor
        Fraction(const Fraction &f);
        
        // read the values for this Fraction from the input stream, return true on success
        bool readLine(std::istream &input);
        // print the Fraction to the output stream
        void printFraction(std::ostream & output);

        // return the value of the Fraction
        double getValue() const { return value; }
};

typedef struct {
    Fraction *f;
} FWrap;

// overloaded comparison operators
bool operator<(const FWrap &lhs, const FWrap &rhs) { 
    return lhs.f->getValue() < rhs.f->getValue(); 
}

// find the greatest common divisor between the two ints
int gcd(int u, int v);

/* main function */
int main() {
    std::multiset<FWrap> fs;
    
    Fraction f;
    while (f.readLine(std::cin)) {
        FWrap fw;
        fw.f = new Fraction(f);
        fs.insert(fw);
    }

    for (auto fi : fs) {
        fi.f->printFraction(std::cout);
    }
}

Fraction::Fraction() {
    whole = 0;
    numer = 0;
    denom = 1;
    neg = false;
    value = 0.0;
}

Fraction::Fraction(const Fraction &f) {
    this->whole = f.whole;
    this->numer = f.numer;
    this->denom = f.denom;
    this->neg = f.neg;
    this->value = f.value;
}

bool Fraction::readLine(std::istream &input) {
    std::string line;
    if (getline(input, line)) {
        std::stringstream ss(line);
        char div;
        if ((ss >> whole) && (ss >> numer) && (ss >> div) && (ss >> denom)) {
            if (whole < 0) {
                neg = true;
                whole = std::abs(whole);
            }
            else if (numer < 0) {
                neg = true;
                numer = std::abs(numer);
            }

            reduce();
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

void Fraction::printFraction(std::ostream &output) {
    if (neg) {
        if (whole == 0) {
            output << whole << " -" << numer << "/" << denom << std::endl;
        }
        else {
            output << "-" << whole << " " << numer << "/" << denom << std::endl;
        }
    }
    else {
        output << whole << " " << numer << "/" << denom << std::endl;
    }
}

void Fraction::reduce() {
    if (numer >= denom) {
        whole += numer / denom;
        numer = numer % denom;
    }

    if (numer > 0) {
        int d = gcd(numer, denom);
        numer /= d;
        denom /= d;
    }
    else {
        denom = 1;
    }

    if (neg) {
        neg = whole != 0 || numer != 0;
    }

    value = (neg ? -1 : 1) * (((float) whole) + numer / ((float)denom));
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
