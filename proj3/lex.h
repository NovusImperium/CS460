#ifndef LEX_H
#define LEX_H

typedef enum {
    ERROR = 0, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT, DIV, MOD,
    EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL, SHIFTR, PLUSPLUS, PLUSEQ,
    MINUSMINUS, MINUSEQ, MULTEQ, DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ,
    LOGOR, OREQ, XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDE, RPAREN,
    LPAREN, SEMI, QUEST, COLON, COMMA, EOFT
} token_t;

extern char *token_names[];

extern inline void init_lex(char *filename);

extern inline token_t get_token();

extern inline char *get_lexeme();

extern inline void error_message(char *msg);

extern inline void end_lex();

extern inline int get_linenum();

extern inline int get_position();

#endif  // LEX_H
