#include <stdio.h>
#include <stdint.h>
#include "parser.h"
#include "array.h"

static array *symbols;
static array *tokens;
static array *productions;

char *symbol_names[] = {
        "program", "more_stmts", "stmt", "decl", "ntype", "decl_tail", "term", "pre", "uoppre", "var", "post",
        "stmt_tail", "binop", "pop_token", "eof_sym"
};

// to be used to determine the token type
// if 1 << token & op_t then token is one of the operator types
static const uint64_t op_t = ((uint64_t) 1) << op_plus | ((uint64_t) 1) << op_minus |
        ((uint64_t) 1) << op_mul | ((uint64_t) 1) << op_div | ((uint64_t) 1) << op_mod | ((uint64_t) 1) << op_pow |
        ((uint64_t) 1) << op_not | ((uint64_t) 1) << op_and | ((uint64_t) 1) << op_or | ((uint64_t) 1) << op_xor |
        ((uint64_t) 1) << op_lsh | ((uint64_t) 1) << op_rsh | ((uint64_t) 1) << op_inc | ((uint64_t) 1) << op_dec |
        ((uint64_t) 1) << op_tilde;

// if 1 << token & log_t then token is one of the logical types
static const uint64_t log_t = ((uint64_t) 1) << log_neq | ((uint64_t) 1) << log_and |
        ((uint64_t) 1) << log_or | ((uint64_t) 1) << log_lt | ((uint64_t) 1) << log_gt |
        ((uint64_t) 1) << log_lte | ((uint64_t) 1) << log_gte | ((uint64_t) 1) << log_eq;

// if 1 << token & assn_t then token is one of the assignment types
static const uint64_t assn_t = ((uint64_t) 1) << assn_get | ((uint64_t) 1) << assn_plus |
        ((uint64_t) 1) << assn_minus | ((uint64_t) 1) << assn_mul | ((uint64_t) 1) << assn_div | ((uint64_t) 1) << assn_mod |
        ((uint64_t) 1) << assn_pow | ((uint64_t) 1) << assn_and | ((uint64_t) 1) << assn_or | ((uint64_t) 1) << assn_xor |
        ((uint64_t) 1) << assn_lsh | ((uint64_t) 1) << assn_rsh;

// if 1 << token & uoppre_t then token is one of the unary operator types
static const uint64_t uoppre_t = ((uint64_t) 1) << op_plus | ((uint64_t) 1) << op_minus |
        ((uint64_t) 1) << op_mul | ((uint64_t) 1) << op_not | ((uint64_t) 1) << op_and |
        ((uint64_t) 1) << op_or | ((uint64_t) 1) << op_inc | ((uint64_t) 1) << op_dec |
        ((uint64_t) 1) << op_tilde;

// if 1 << token & binop_t then token is a binary operator
static uint64_t binop_t;

bool init_parser() {
    // initialize the binary operator type checker
    binop_t = ((uint64_t) 1) << comma | assn_t | log_t | op_t;

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

    // initialize the list of productions for debugging purposes
    opt = arr_init(32);
    if (!opt.e) {
        return false;
    }
    productions = opt.val;

    return true;
}

parser_return_t parse_token(token_t t) {
    // push the current token stack onto the productions tracker
    char *t_str = null;
    asprintf(&t_str, "\ncurrent token: %s(%s)\ntokens: ", token_names[t], get_lexeme());
    arr_push(productions, t_str);
    void *print_tok(void *tok) {
        char *str = null;
        asprintf(&str, "%s, ", token_names[(token_t) tok]);
        arr_push(productions, str);
        return tok;
    }
    arr_foreach(tokens, print_tok);
    // push the current symbol stack onto the productions tracker
    char *s_str = null;
    asprintf(&s_str, "\nsymbols: ");
    arr_push(productions, s_str);
    void *print_sym(void *sym) {
        char *str = null;
        asprintf(&str, "%s, ", symbol_names[(symbol_t) sym]);
        arr_push(productions, str);
        return sym;
    }
    arr_foreach(symbols, print_sym);

    optional opt = arr_pop(symbols);
    if (!opt.e) {
        return parser_err;
    }

    char *prod = null;
    switch ((symbol_t) opt.val) {
        case program: {
            // <program> -> <stmt> semi <more_stmts> <eof_sym>
            //            | <decl> semi <more_stmts> <eof_sym>
            if (t == rsv_type_int || t == rsv_type_double) {
                asprintf(&prod, "\n<program> -> <decl> _semi <more_stmts> <eof_sym>\n");
                arr_push(productions, prod);
                arr_push(symbols, (void *) eof_sym);
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) decl);
                return keep_token;
            } else {
                asprintf(&prod, "\n<program> -> <stmt> _semi <more_stmts> <eof_sym>\n");
                arr_push(productions, prod);
                arr_push(symbols, (void *) eof_sym);
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) stmt);
                return keep_token;
            }
        }
        case more_stmts: {
            // <more_stmts> -> <stmt> semi <more_stmts>
            //               | <decl> semi <more_stmts>
            //               | {}
            if (t == rsv_type_double || t == rsv_type_int) {
                asprintf(&prod, "\n<more_stmts> -> <decl> _semi <more_stmts>\n");
                arr_push(productions, prod);
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) decl);
            } else if (((opt = arr_peek(tokens)).e && t == (token_t) opt.val) || t == eof_tok) {
                asprintf(&prod, "\n<more_stmts> -> {}\n");
                arr_push(productions, prod);
            } else {
                asprintf(&prod, "\n<more_stmts> -> <stmt> _semi <more_stmts>\n");
                arr_push(productions, prod);
                arr_push(symbols, (void *) more_stmts);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) semi);
                arr_push(symbols, (void *) stmt);
            }
            return keep_token;
        }
        case stmt: {
            // <stmt> -> <term> <more_stmts>
            asprintf(&prod, "\n<stmt> -> <term> <stmt_tail>\n");
            arr_push(productions, prod);
            arr_push(symbols, (void *) stmt_tail);
            arr_push(symbols, (void *) term);
            return keep_token;
        }
        case decl: {
            // <decl> -> <ntype> <var> <decl_tail>
            asprintf(&prod, "\n<decl> -> <ntype> <var> <decl_tail>\n");
            arr_push(productions, prod);
            arr_push(symbols, (void *) decl_tail);
            arr_push(symbols, (void *) var);
            arr_push(symbols, (void *) ntype);
            return keep_token;
        }
        case ntype: {
            // <ntype> -> int | double
            if (t == rsv_type_int || t == rsv_type_double) {
                asprintf(&prod, "\n<ntype> -> %s\n", token_names[t]);
                arr_push(productions, prod);
                return adv_token;
            } else {
                return parser_err;
            }
        }
        case decl_tail: {
            // <decl_tail> -> assign <stmt> <decl_tail>
            //              | comma <var> <decl_tail>
            //              | {}
            if (t == assn_get) {
                asprintf(&prod, "\n<decl_tail> -> %s <stmt> <decl_tail>\n", token_names[t]);
                arr_push(productions, prod);
                arr_push(symbols, (void *) decl_tail);
                arr_push(symbols, (void *) stmt);
            } else if (t == comma) {
                asprintf(&prod, "\n<decl_tail> -> %s <comma> <decl_tail>\n", token_names[t]);
                arr_push(productions, prod);
                arr_push(symbols, (void *) decl_tail);
                arr_push(symbols, (void *) var);
            } else {
                asprintf(&prod, "\n<decl_tail> -> {}\n");
                arr_push(productions, prod);
                return keep_token;
            }
            return adv_token;
        }
        case term: {
            // <term> -> <pre> <var> <post>
            //         | lparen <stmt> rparen
            if (t == lparen) {
                asprintf(&prod, "\n<term> -> %s <stmt> _rparen\n", token_names[t]);
                arr_push(productions, prod);
                arr_push(tokens, (void *) rparen);
                arr_push(symbols, (void *) pop_token);
                arr_push(symbols, (void *) stmt);
                return adv_token;
            } else {
                asprintf(&prod, "\n<term> -> <pre> <var> <post>\n");
                arr_push(productions, prod);
                arr_push(symbols, (void *) post);
                arr_push(symbols, (void *) var);
                arr_push(symbols, (void *) pre);
                return keep_token;
            }
        }
        case pre: {
            // <pre> -> <uoppre> <pre>
            //        | {}
            if (uoppre_t & (((uint64_t) 1) << t)) {
                asprintf(&prod, "\n<pre> -> %s <pre>\n", token_names[t]);
                arr_push(productions, prod);
                arr_push(symbols, (void *) pre);
                return adv_token;
            } else {
                asprintf(&prod, "\n<pre> -> {}\n");
                arr_push(productions, prod);
                return keep_token;
            }
        }
        case uoppre: {
            // <uoppre> -> plus | minus | mult | not | and | tilde | inc | dec
            // note: rule has been incorporated into <pre>
            return keep_token;
        }
        case var: {
            // <var> -> id | num
            if (t == id || t == num_lit) {
                asprintf(&prod, "\n<var> -> %s\n", token_names[t]);
                arr_push(productions, prod);
                return adv_token;
            } else {
                return parser_err;
            }
        }
        case post: {
            // <post> -> inc | dec | {}
            if (t == op_inc || t == op_dec) {
                asprintf(&prod, "\n<post> -> %s\n", token_names[t]);
                arr_push(productions, prod);
                return adv_token;
            } else {
                asprintf(&prod, "\n<post> -> {}\n");
                arr_push(productions, prod);
                return keep_token;
            }
        }
        case stmt_tail: {
            // <stmt_tail> -> <binop> <stmt>
            //              | question_mark <stmt> colon <stmt>
            //              | {}
            if (t == qst_mark) {
                asprintf(&prod, "\n<stmt_tail> -> %s <stmt> _colon <stmt>\n", token_names[t]);
                arr_push(productions, prod);
                arr_push(symbols, (void *) stmt);
                arr_push(symbols, (void *) pop_token);
                arr_push(tokens, (void *) colon);
                arr_push(symbols, (void *) stmt);
            } else if (binop_t & (((uint64_t) 1) << t)) {
                asprintf(&prod, "\n<stmt_tail> -> %s <stmt>\n", token_names[t]);
                arr_push(productions, prod);
                arr_push(symbols, (void *) stmt);
            } else {
                asprintf(&prod, "\n<stmt_tail> -> {}\n");
                arr_push(productions, prod);
                return keep_token;
            }
            return adv_token;
        }
        case binop: {
            // <binop> -> plus | minus | mult | div | mod | and | or | xor | assign | lt | gt | lte | gte | shiftl
            //          | shiftr | plus_eq | minus_eq | mul_eq | pow_eq | div_eq | mod_eq | neq | eq | log_and | log_or
            //          | and_eq | or_eq | xor_eq | shiftl_eq | shiftr_eq | comma | pow
            // note: rule has been incorporated into <stmt_tail>
            return keep_token;
        }
        case pop_token: {
            // check that the current token matches the expected token
            opt = arr_pop(tokens);
            if (opt.e && (token_t) opt.val == t) {
                asprintf(&prod, "\n<pop_token> -> %s\n", token_names[t]);
                arr_push(productions, prod);
                return adv_token;
            } else {
                return parser_err;
            }
        }
        case eof_sym: {
            // <eof_sym> -> eof_tok
            if (t == eof_tok) {
                asprintf(&prod, "\n<eof_sym> -> %s\n", token_names[t]);
                arr_push(productions, prod);
                return end_token;
            } else {
                return parser_err;
            }
        }
        default: {
            // how'd we get here?
            return parser_err;
        }
    }
}

void shutdown_parser() {
    arr_free(symbols);
    arr_free(tokens);
}

void dump_parser() {
    void *print_str(void *s) {
        printf("%s", (char *)s);
        return s;
    }
    arr_foreach(productions, print_str);
}
