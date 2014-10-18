#ifndef LEX_H
#define LEX_H

typedef enum {ERROR = 0, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT,
		DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
		SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
		DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
		XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDE, RPAREN,
		LPAREN, SEMI, QUEST, COLON, COMMA, EOFT} token_type;

extern char * token_names [];

void init_lex (char * filename);

token_type get_token ();

char * get_lexeme ();

void error_message (char * msg);

void end_lex ();

#endif  // LEX_H
