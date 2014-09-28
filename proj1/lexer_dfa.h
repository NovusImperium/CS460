#include "array.h"
#include "lexer_dfa.c"

typedef enum {
    keyword,
    ident,
    num_int,
    num_float,
    arith_add,
    arith_sub,
    arith_mul,
    arith_div,
    arith_mod,
    arith_inc,
    arith_dec,
    assn_get,
    assn_add,
    assn_sub,
    assn_mul,
    assn_div,
    assn_mod,
    assn_lsh,
    assn_rsh,
    assn_and,
    assn_xor,
    assn_inor,
    logic_and,
    logic_not,
    logic_or,
    logic_ne,
    logic_eq,
    logic_gt,
    logic_ge,
    logic_lt,
    logic_le,
    bit_and,
    bit_inor,
    bit_xor,
    bit_lsh,
    bit_rsh,
    bit_ones,
    open_paren,
    close_paren,
    semi_colon,
    tern_cond,
    tern_else,
    series_op,
    invalid_lex
} lexical_t;

typedef enum {
    no_err = false,
    invalid_char,
    invalid_id_length
} err_t;

// struct to hold the lexical type and the string representation (if need be)
// if a lexical error is present, also holds the error type and the column location in the line
typedef struct {
    lexical_t lex;
    char str[32];
    err_t err;
    int c;
} token;

// entry point to the lexer dfa,
// the input line must terminate in a null-char
// returns an array of tokens that represent the valid lexemes found on the input line
extern array *dfa_start(char *line);

// generate an ID or keyword token starting from the beginning of the input string
// returns the length of the ID or keyword, up to and beyond the 31 character length limit
extern int dfa_word(char *str, token *t);

// generate an int or float token starting from the beginning of the input string
// returns the length of the int or float, up to and beyond the 31 character length limit
extern int dfa_num(char *str, token *t);

// generate an invalid_char token
// returns 0
extern int dfa_invchar(char *str, token *t);

// generate logic_not or logic_ne token
// returns 0 if logic_not, 1 if logic_ne
extern int dfa_not(char *str, token *t);

// generate arith_mod or assn_mod token
// returns 0 if arith_mod, 1 if assn_mod
extern int dfa_mod(char *str, token *t);

// generate bit_and, assn_and, or logic_and token
// returns 0 if bit_and, 1 if assn_and or logic_and
extern int dfa_and(char *str, token *t);

// generate an open_paren token
// returns 0
extern int dfa_oparen(char *str, token *t);

// generate a close_paren token
// returns 0
extern int dfa_cparen(char *str, token *t);

// generate arith_mul or assn_mul token
// returns 0
extern int dfa_mul(char *str, token *t);

// generate arith_add, arith_inc, or assn_add token
// returns 0 if arith_add, 1 if arith_inc or assn_add
extern int dfa_add(char *str, token *t);

// generate series_op token
// returns 0
extern int dfa_series(char *str, token *t);

// generate arith_sub, arith_dec, or assn_sub token
// returns 0 if arith_sub, 1 if arith_dec or assn_sub
extern int dfa_sub(char *str, token *t);

// generate arith_div or assn_div token

// store an array of dfa state functions to call from the starting state
// the index of each state function corresponds to the ascii value of the character to transition from
//      33 '!'  => index 0
//      126 '~' => index 94
typedef int (*dfa_func)(char *, token *);

static const dfa_func dfa_trans[94] = {
        dfa_not,    // 33	!
        dfa_invchar,// 34	"
        dfa_invchar,// 35	#
        dfa_invchar,// 36	$
        dfa_mod,    // 37	%
        dfa_and,    // 38	&
        dfa_invchar,// 39	'
        dfa_oparen, // 40	(
        dfa_cparen, // 41	)
        dfa_mul,    // 42	*
        dfa_add,    // 43	+
        dfa_series, // 44	,
        dfa_sub,    // 45	-
        dfa_num,    // 46	.
        // 47	/
        dfa_num,    // 48	0
        dfa_num,    // 49	1
        dfa_num,    // 50	2
        dfa_num,    // 51	3
        dfa_num,    // 52	4
        dfa_num,    // 53	5
        dfa_num,    // 54	6
        dfa_num,    // 55	7
        dfa_num,    // 56	8
        dfa_num,    // 57	9
        // 58	:
        // 59	;
        // 60	<
        // 61	=
        // 62	>
        // 63	?
        dfa_invchar,// 64	@
        dfa_word,   // 65	A
        dfa_word,   // 66	B
        dfa_word,   // 67	C
        dfa_word,   // 68	D
        dfa_word,   // 69	E
        dfa_word,   // 70	F
        dfa_word,   // 71	G
        dfa_word,   // 72	H
        dfa_word,   // 73	I
        dfa_word,   // 74	J
        dfa_word,   // 75	K
        dfa_word,   // 76	L
        dfa_word,   // 77	M
        dfa_word,   // 78	N
        dfa_word,   // 79	O
        dfa_word,   // 80	P
        dfa_word,   // 81	Q
        dfa_word,   // 82	R
        dfa_word,   // 83	S
        dfa_word,   // 84	T
        dfa_word,   // 85	U
        dfa_word,   // 86	V
        dfa_word,   // 87	W
        dfa_word,   // 88	X
        dfa_word,   // 89	Y
        dfa_word,   // 90	Z
        dfa_invchar,// 91	[
        dfa_invchar,// 92	\ (backslash)
        dfa_invchar,// 93	]
        // 94	^
        dfa_word,   // 95	_
        dfa_invchar,// 96	`
        dfa_word,   // 97	a
        dfa_word,   // 98	b
        dfa_word,   // 99	c
        dfa_word,   // 100	d
        dfa_word,   // 101	e
        dfa_word,   // 102	f
        dfa_word,   // 103	g
        dfa_word,   // 104	h
        dfa_word,   // 105	i
        dfa_word,   // 106	j
        dfa_word,   // 107	k
        dfa_word,   // 108	l
        dfa_word,   // 109	m
        dfa_word,   // 110	n
        dfa_word,   // 111	o
        dfa_word,   // 112	p
        dfa_word,   // 113	q
        dfa_word,   // 114	r
        dfa_word,   // 115	s
        dfa_word,   // 116	t
        dfa_word,   // 117	u
        dfa_word,   // 118	v
        dfa_word,   // 119	w
        dfa_word,   // 120	x
        dfa_word,   // 121	y
        dfa_word,   // 122	z
        dfa_invchar,// 123	{
        // 124	|
        dfa_invchar,// 125	}
        // 126	~
};
