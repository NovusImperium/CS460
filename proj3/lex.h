#ifndef LEX_H
#define LEX_H

typedef enum {
    ERROR = 0, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT, DIV, MOD,
    EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL, SHIFTR, PLUSPLUS, PLUSEQ,
    MINUSMINUS, MINUSEQ, MULTEQ, DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ,
    LOGOR, OREQ, XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDE, RPAREN,
    LPAREN, SEMI, QUEST, COLON, COMMA, LBRACKET, RBRACKET, LBRACE, RBRACE, EOFT
} token_t;

extern char *token_names[];

void init_lex(char *filename);

token_t get_token();

char *get_lexeme();

void error_message(char *msg);

void end_lex();

int get_linenum();

int get_position();

#endif  // LEX_H
