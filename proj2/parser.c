#include "parser.h"
#include "array.h"
#include "defs.h"

static array *sym_stack;
static array *tok_stack;

bool init_parser() {
    // initialize the symbol stack
    optional opt = arr_init(32);
    if (!opt.e) {
        return false;
    }
    sym_stack = opt.val;
    // prime the symbol stack with the starting symbol
    arr_push(sym_stack, (void*)program);

    // initialize the token stack
    opt = arr_init(32);
    if (!opt.e) {
        return false;
    }

    tok_stack = opt.val;

    return true;
}

bool parse_token(token_t t) {
    optional opt = arr_pop(sym_stack);
    if (!opt.e) {
        return false;
    }

    switch ((symbol_t)opt.val) {
        case program:
            break;
        case more_stmts:
            break;
        case stmt:
            break;
        case decl:
            break;
        case ntype:
            break;
        case decl_tail:
            break;
        case term:
            break;
        case pre:
            break;
        case unry_op:
            break;
        case var:
            break;
        case post:
            break;
        case stmt_tail:
            break;
        case binop:
            break;
        case pop_token:
            break;
    }

    return true;
}
