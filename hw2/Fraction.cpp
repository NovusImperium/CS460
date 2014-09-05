#include "Fraction.h"
#include "gcd.h"
#include <sstream>

Fraction::Fraction() {
    numer = 0;
    denom = 1;
}

Fraction::Fraction(const Fraction &f) {
    this->numer = f.numer;
    this->denom = f.denom;
}

bool Fraction::operator<(const Fraction &f) {
    return numer * f.denom < f.numer * denom;
}

Fraction & Fraction::operator+=(const Fraction &f) {
    int d = denom * f.denom;
    numer = numer * d + f.numer * d;
    denom = d;
    reduce();

    return *this;
}

std::istream & operator>>(std::istream &input, Fraction &f) {
    std::string line;
    if (getline(input, line)) {
        std::stringstream ss(line);
        int whole, numer, denom;
        char div;
        bool neg = false;
        if ((ss >> whole) && (ss >> numer) && (ss >> div) && (ss >> denom)) {
            if (whole < 0) {
                neg = true;
                whole = std::abs(whole);
            }
            else if (numer < 0) {
                neg = true;
                numer = std::abs(numer);
            }

            f.numer = (neg ? -1 : 1) * (whole * denom + numer);
            f.denom = denom;
            f.reduce();
        }
    }
    return input;
}

std::ostream & operator<<(std::ostream &output, Fraction &f) {
    bool neg = f.numer < 0;
    int whole = std::abs(f.numer / f.denom);
    int numer = std::abs(f.numer % f.denom);

    if (neg) {
        if (whole == 0) {
            output << whole << " -" << numer << "/" << f.denom; 
        }
        else {
            output << "-" << whole << " " << numer << "/" << f.denom;
        }
    }
    else {
        output << whole << " " << numer << "/" << f.denom;
    }

    return output;
}

void Fraction::reduce() {
    if (numer < 0) {
        int g = gcd(-1 * numer, denom);
        numer /= g;
        denom /= g;
    }
    else {
        int g = gcd(numer, denom);
        numer /= g;
        denom /= g;
    }
}
