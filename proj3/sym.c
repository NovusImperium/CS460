#include "sym.h"

struct sym {
    char *id;
    bool flag;
    union {
        long long ival;
        double dval;
    };
};

optional get_sym(sym_tab *t, char *id) {
    optional opt;
    return opt;
}

bool insert_sym(sym_tab *t, char *id, optional val) {
    return 0;
}

optional get_value(sym *s) {
    optional opt;
    return opt;
}

char *get_id(sym *s) {
    return null;
}

void write_syms(sym_tab *t, FILE *out) {

}
