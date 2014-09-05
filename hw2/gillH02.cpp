// CS460 -- HW2
// Amandeep Gill
// read, parse, sort, and display a list of fraction in proper reduced form
// written in "idiomatic" c++

#include <iostream>
#include <set>
#include "Fraction.h"

typedef struct {
    Fraction *f;
} FWrap;

// overloaded comparison operators
bool operator<(const FWrap &lhs, const FWrap &rhs) { 
    return *lhs.f < *rhs.f; 
}

/* main function */
int main() {
    std::multiset<FWrap> fs;
    
    Fraction in;
    Fraction sum;
    while (std::cin >> in) {
        FWrap fw;
        fw.f = new Fraction(in);
        sum += *fw.f;
        //std::cout << *fw.f << std::endl;
        fs.insert(fw);
    }

    std::cout << "The sum of the fractions is: " << sum << std::endl;

    for (auto fw : fs) {
        std::cout << *fw.f << std::endl;
    }
}
