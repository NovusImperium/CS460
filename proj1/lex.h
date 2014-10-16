#include "lexer_dfa.h"

extern const char *token_names[];

void init_lex(char *filename);

lexical_t get_token();

char *get_lexeme();

void error_message(char *msg);

void end_lex();
