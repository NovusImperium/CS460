#include "parser.h"
#include "array.h"

static array *stack;



bool init_parser() {
    optional opt = arr_init(32);
    if (!opt.e) {
        return false;
    }

    stack = opt.val;
    arr_push(stack, program);

    return true;
}

bool parse_token(token_t t) {
    return 0;
}
