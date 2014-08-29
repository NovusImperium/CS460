// CS460 -- HW2
// Amandeep Gill
// read, parse, sort, and display a list of fraction in proper reduced form
// written in idiomatic c++

#include <iostream>
#include <map>
#include <cmath>
#include <algorithm>

/* class declaration for Fraction */
class Fraction {
    private:
        int whole, numer, denom;
        bool neg;
        double value;

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
