#include <libio.h>

enum token_type {ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT,
		DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
		SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
		DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
		XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDA, RPAREN,
		LPAREN, SEMI, QUEST, COLON, COMMA, EOFT};

extern char * token_names [];

void init_lex (char * filename);

enum token_type get_token ();

char * get_lexeme ();

void error_message (char * msg);

void end_lex ();

