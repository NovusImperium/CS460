// CS460 -- HW1-2
// Amandeep Gill
// read, parse, sort, and display a list of fraction in proper reduced form

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>

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

/* Misc functions for dealing with math and fractions */
// find and return the greatest common divisor of ints a and b
unsigned int gcd(unsigned int u, unsigned int v);
// reduce the number to a proper fraction
void reduce(fraction &f);
// convert the string into an int
unsigned int consume(std::string &s);

/* finite state machine to parse the string and build the fraction simultaneously */
// initial state 
unsigned int starting(char c, std::string &s, fraction &f);
// whole number state 
unsigned int whole_num(char c, std::string &s, fraction &f);
// numerator transition state 
unsigned int numer_trans(char c, std::string &s, fraction &f);
// fraction only transition state 
unsigned int frac_only(char c, std::string &s, fraction &f);
// numerator state 
unsigned int numer(char c, std::string &s, fraction &f);
// denom transition state
unsigned int denom_trans(char c, std::string &s, fraction &f);
// denom state 
unsigned int denom(char c, std::string &s, fraction &f);
// final state 
void final_succ(std::vector<fraction> &fs, fraction &f);
// ENUM for state labels
enum fsm_states { 
                  STARTING, 
                  WHOLE_NUM, 
                  NUMER_TRANS, 
                  FRAC_ONLY, 
                  NUMER, 
                  DENOM_TRANS, 
                  DENOM, 
                  ERROR, 
                  FINAL_SUCC 
                };

/* Functions for handling reading and writing to the console */
// Read in a string from stdin, return -1 if EOF reached, else return length of the string
int readline(std::string &s);
// Print all fractions to the console
void print_fractions(std::vector<fraction> &fs);

int main(void) {
    std::vector<fraction> fs;
    std::string line;
    int lnum = 0;

    while (readline(line) > -1) {
        int state = 0;
        unsigned int  i = 0;
        std::string stack = "";
        fraction f;

        while (i <= line.size()) {
            char c = (i < line.size()) ? line[i] : 0;
            std::cout << "i = " << i << " : state = " << state 
                      << " : c = '" << c << "'" << std::endl;
            switch (state) {
                case STARTING:
                    state = starting(c, stack, f);
                    break;
                case WHOLE_NUM:
                    state = whole_num(c, stack, f);
                    break;
                case NUMER_TRANS:
                    state = numer_trans(c, stack, f);
                    break;
                case FRAC_ONLY:
                    state = frac_only(c, stack, f);
                    break;
                case NUMER:
                    state = numer(c, stack, f);
                    break;
                case DENOM:
                    state = denom(c, stack, f);
                    break;
                case DENOM_TRANS:
                    state = denom_trans(c, stack, f);
                    break;
                case ERROR:
                default:
                    goto exit_with_error;
            }

            if (state == FINAL_SUCC) {
                std::cout << "reached success state" << std::endl;
                final_succ(fs, f);
            }

            i++;
        }

        if (state != 6) {
            std::cout << "left switch loop without error or success" << std::endl;
            std::cout << "i = " << i << " : state = " << stack << std::endl;
            goto exit_with_error;
        }
        lnum++;
    }

    print_fractions(fs);

    return 0;

exit_with_error:
    std::cout << "Error on line " << lnum << std::endl;
    std::cout << "\t" << line << std::endl;
    return -1;
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

unsigned int starting(char c, std::string &s, fraction &f) {
    // check if c is unsigned int, epsilon move to whole number state 
    if (c >= '1' && c <= '9') {
       return whole_num(c, s, f);
    }
    switch (c) {
        // continue to consume leading spaces, loopback
        case ' ':
            return STARTING;
        // set the negative flag, move to negative whole number state 
        case '-':
            f.neg = true;
            return WHOLE_NUM;
        // consume leading zero, move to fraction only state transition state 
        case '0':
            return FRAC_ONLY;
        // invalid character, move to error state 
        default:
            return ERROR;
    }
}

unsigned int whole_num(char c, std::string &s, fraction &f) {
    // check if c is int, push to stack and loopback
    if (c >= '0' && c <= '9') {
        s.push_back(c);
       return WHOLE_NUM;
    }
    switch (c) {
        // consume space, 
        case ' ':
            // stack is non-empty: consume stack and move to numerator state 
            if (s.size() > 0) {
                f.whole = consume(s);
                return NUMER_TRANS;
            }
            // stack is empty: move to error state 
            return ERROR;
        // invalid character, move to error state 
        default:
            return ERROR;
    }
}

unsigned int numer_trans(char c, std::string &s, fraction &f) {
    // consume int, push to stack and move to numerator state 
    if (c >= '1' && c <= '9') {
        s.push_back(c);
        return NUMER;
    }
    switch (c) {
        // consume 0, move to denom state 
        case '0':
            return DENOM_TRANS;
        // invalid character, move to error state 
        default:
            return ERROR;
    }
}

unsigned int frac_only(char c, std::string &s, fraction &f) {
    // check if c is int,
    if (c >= '0' && c <= '9') {
        // stack non-empty: epsilon move to numerator transition state 
        if (s.size() > 0) {
            s.clear();
            return numer_trans(c, s, f);
        }
        // stack empty: move to error state 
        else {
            return ERROR;
        }
    }
    switch (c) {
        // consume space, push to stack and loopback
        case ' ':
            s.push_back(c);
            return FRAC_ONLY;
        // mark number as negative, move to numerator transition state 
        case '-':
            s.clear();
            f.neg = true;
            return NUMER_TRANS;
        // invalid character, move to error state 
        default:
            return ERROR;
    }
}

unsigned int numer(char c, std::string &s, fraction &f) {
    // check if c is int, push to stack and loopback
    if (c >= '0' && c <= '9') {
        s.push_back(c);
       return NUMER;
    }
    switch (c) {
        // consume div sign, 
        case '/':
            // stack is non-empty: consume stack and move to denom state 
            if (s.size() > 0) {
                f.num = consume(s);
                return DENOM_TRANS;
            }
            // stack is empty: move to error state 
            return ERROR;
        // invalid character, move to error state 
        default:
            return ERROR;
    }
}

unsigned int denom_trans(char c, std::string &s, fraction &f) {
    // check that the character is a div sign: go to denom state
    if (c == '/') {
        return DENOM_TRANS;
    }
    // check if c is an int: epsilon move to denom state
    else if (c >= '1' && c <= '9') {
        return denom(c, s, f);
    }
    // go to error state if not
    else {
        return ERROR;
    }
}

unsigned int denom(char c, std::string &s, fraction &f) {
    // check if c is int, push c to stack and loopback
    if (c >= '1' && c <= '9') {
        s.push_back(c);
        return DENOM_TRANS;
    }
    switch (c) {
        // special case for 0
        case '0': 
            // push c to stack and loopback
            if (s.size() > 0) {
                s.push_back(c);
                return DENOM_TRANS;
            }
            // move to error state 
            else {
                return ERROR;
            }
        // null terminator,
        case 0:
            // consume stack and move to final state 
            if (s.size() > 0) {
                f.den = consume(s);
                return FINAL_SUCC;
            }
            // move to error state 
            else {
                return ERROR;
            }
        // invalid character, move to error state 
        default:
            return ERROR;
    }
}

// reduce the number to a proper fraction and insert into bst, insertion code borrowed from SO
// http://stackoverflow.com/questions/18774858/using-binary-search-with-vectors
void final_succ(std::vector<fraction> &fs, fraction &f) {
    reduce(f);
    size_t mid, left = 0 ;
    size_t right = fs.size();
    while (left < right) {
        mid = left + (right - left)/2;
        if (f > fs[mid]){
            left = mid+1;
        }
        else if (f < fs[mid]) {
            right = mid;
        }
        else {
            break;
        }                                                                                                               
    }

    fs.insert(fs.begin() + mid, f);
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

int readline(std::string &s) {
    if (std::getline(std::cin, s)) {
        return s.size();
    }
    else {
        return ERROR;
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

unsigned int consume(std::string &s) {
    unsigned int i = atoi(s.c_str());
    s.clear();
    return i;
}
