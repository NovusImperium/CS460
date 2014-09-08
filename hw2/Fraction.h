#include <iostream>

/* class declaration for Fraction */
class Fraction {
    private:
        long long numer, denom;

        // reduce the fraction into lowest form
        void reduce();

    public:
        // initializes the fraction as 0+0/1 and 0.00
        Fraction();
        // copy constructor
        Fraction(const Fraction &f);
        
        // read the values for this Fraction from the input stream, return true on success
        friend std::istream & operator>>(std::istream &input, Fraction &f);
        // print the Fraction to the output stream
        friend std::ostream & operator<<(std::ostream & output, Fraction &f);
        
        // overload the less-than comparison operator
        bool operator<(const Fraction &f);
        // overload the add-to operator
        Fraction & operator+=(const Fraction &f);
};

