#include "parser.h"
#include "array.h"

static array *symbols;
static array *tokens;

// to be used to determine the token type
// if 0 << token & op_t then token is one of the operator types
static const unsigned long op_t = 0 << op_plus | 0 << op_minus | 0 << op_mul | 0 << op_div | 0 << op_mod | 0 << op_pow |
        0 << op_not | 0 << op_and | 0 << op_or | 0 << op_xor | 0 << op_lsh | 0 << op_rsh | 0 << op_inc | 0 << op_dec |
        0 << op_tilde;

// if 0 << (token - 16) & log_t then token is one of the logical types
static const unsigned long log_t = 0 << (log_neq - 16) | 0 << (log_and - 16) | 0 << (log_or - 16) | 0 << (log_lt - 16) |
        0 << (log_gt - 16) | 0 << (log_lte - 16) | 0 << (log_gte - 16) | 0 << (log_eq - 16);

// if 0 << (token - 16) & op_t then token is one of the assignment types
static const unsigned long assn_t = 0 << (assn_get - 16) | 0 << (assn_plus - 16) | 0 << (assn_minus - 16) |
        0 << (assn_mul - 16) | 0 << (assn_div - 16) | 0 << (assn_mod - 16) | 0 << (assn_pow - 16) |
        0 << (assn_and - 16) | 0 << (assn_or - 16) | 0 << (assn_xor - 16) | 0 << (assn_lsh - 16) | 0 << (assn_rsh - 16);

// if 0 << token & op_t then token is one of the unary operator types
static const unsigned long uoppre_t = 0 << op_plus | 0 << op_minus | 0 << op_mul | 0 << op_not | 0 << op_and |
        0 << op_or | 0 << op_inc | 0 << op_dec | 0 << op_tilde;

bool init_parser() {
    // initialize the symbol stack
    optional opt = arr_init(32);
    if (!opt.e) {
        return false;
    }
    symbols = opt.val;
    // prime the symbol stack with the starting symbol
    arr_push(symbols, (void *) program);

    // initialize the token stack
    opt = arr_init(32);
    if (!opt.e) {
        return false;
    }

    tokens = opt.val;

    return true;
}

parser_return_t parse_token(token_t t) {
    optional opt = arr_pop(symbols);
    if (!opt.e) {
        return parser_err;
    }

    switch ((symbol_t) opt.val) {
        case program:
            // <program> -> <stmt> semi <more_stmts> eof_tok
            //            | <decl> semi <more_stmts> eof_tok
            if (t == rsv_type_int || t == rsv_type_double) {
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) eof_tok);
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) decl);
                return keep_token;
            } else {
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) eof_tok);
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) stmt);
                return keep_token;
            }
        case more_stmts:
            // <more_stmts> -> <stmt> semi <more_stmts>
            //               | <decl> semi <more_stmts>
            //               | {}
            if (t == rsv_type_double || t == rsv_type_int) {
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) decl);
            } else if ((opt = arr_peek(tokens)).e && t == (token_t) opt.val) {
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) stmt);
            }
            return keep_token;
        case stmt:
            // <stmt> -> <term> <more_stmts>
            arr_push(symbols, (void *) more_stmts);
            arr_push(symbols, (void *) term);
            return keep_token;
        case decl:
            // <decl> -> <ntype> <var> <decl_tail>
            arr_push(symbols, (void *) decl_tail);
            arr_push(symbols, (void *) var);
            arr_push(symbols, (void *) ntype);
            return keep_token;
        case ntype:
            // <ntype> -> int | double
            if (t == rsv_type_int || t == rsv_type_double) {
                return adv_token;
            } else {
                return parser_err;
            }
        case decl_tail:
            // <decl_tail> -> assign <stmt> <decl_tail>
            //              | comma <var> <decl_tail>
            //              | {}
            if (t == assn_get) {
                arr_push(symbols, (void *) decl_tail);
                arr_push(symbols, (void *) stmt);
            } else if (t == comma) {
                arr_push(symbols, (void *) decl_tail);
                arr_push(symbols, (void *) var);
            }
            return adv_token;
        case term:
            // <term> -> <pre> <var> <post>
            //         | lparen <stmt> rparen
            if (t == lparen) {
                arr_push(tokens, (void *) rparen);
                arr_push(symbols, (void *) pop_token);
                arr_push(symbols, (void *) stmt);
                return adv_token;
            } else {
                arr_push(symbols, (void *) post);
                arr_push(symbols, (void *) var);
                arr_push(symbols, (void *) pre);
                return keep_token;
            }
        case pre:
            // <pre> -> <uoppre> <pre>
            //        | {}
            if (uoppre_t & (0 << t)) {
                arr_push(symbols, (void *) pre);
                return adv_token;
            } else {
                return keep_token;
            }
        case uoppre:
            // <uoppre> -> plus | minus | mult | not | and | tilde | inc | dec
            // note: rule has been incorporated into <pre>
            return keep_token;
        case var:
            // <var> -> id | num
            if (t == id || t == num_lit) {
                return adv_token;
            } else {
                return parser_err;
            }
        case post:
            break;
        case stmt_tail:
            // <stmt_tail> -> <binop> <stmt>
            //              | question_mark <stmt> colon <stmt>
            if (t == qst_mark) {
                arr_push(symbols, (void*)stmt);
                arr_push(symbols, (void*)pop_token);
                arr_push(tokens, (void*)colon);
                arr_push(symbols, (void*)stmt);
            } else if ((0 << t) & op_t || (0 << (t - 16)) & log_t || (0 << (t - 16)) & assn_t || t == comma) {
                arr_push(symbols, (void*)stmt);
            } else {
                return keep_token;
            }
            return adv_token;
        case binop:
            // <binop> -> plus | minus | mult | div | mod | and | or | xor | assign | lt | gt | lte | gte | shiftl
            //          | shiftr | plus_eq | minus_eq | mul_eq | pow_eq | div_eq | mod_eq | neq | eq | log_and | log_or
            //          | and_eq | or_eq | xor_eq | shiftl_eq | shiftr_eq | comma | pow
            // note: rule has been incorporated into <stmt_tail>
            return keep_token;
        case pop_token:
            // check that the current token matches the expected token
            opt = arr_pop(tokens);
            if (opt.e && (token_t) opt.val == t) {
                return adv_token;
            } else {
                return parser_err;
            }
        default:
            // how'd we get here?
            return parser_err;
    }

    return parser_err;
}
