#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"
#include "dfa.h"

extern FILE *lst;

static char *fn;
static char *file;
static size_t len;
static char lexeme[32];
static int line;
static int lpos;
static int curr;
static int errs;

char *token_names[] = {
  "ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT", "PLUS", "MINUS", "MULT",
  "DIV", "MOD", "EXP", "NOT", "AND", "OR", "XOR", "ASSIGN", "LT", "GT", "SHIFTL",
  "SHIFTR", "PLUSPLUS", "PLUSEQ", "MINUSMINUS", "MINUSEQ", "MULTEQ",
  "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ",
  "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDE", "RPAREN",
  "LPAREN", "SEMI", "QUEST", "COLON", "COMMA", "EOFT"
};

void init_lex(char *filename) {
  fn = filename;
  file = NULL;
  len = 0;
  line = 1;
  lpos = 0;
  curr = 0;
  errs = 0;

  FILE *in = fopen(filename, "r");
  if (in == NULL || getdelim(&file, &len, EOF, in) < 0) {
    fprintf(lst, "Error reading file: %s\n", filename);
    exit(1);
  }

  fclose(in);

  int i;
  for (i = 0; file[i] != '\n' && file[i] != '\0'; i++);

  if (file[i] == '\n') {
    file[i] = '\0';
    fprintf(lst, "%4d: %s\n", line, file);
    file[i] = '\n';
  } else {
    fprintf(lst, "%4d: %s\n", line, file);
  }
}

int get_linenum()
{
  return line;
}

int get_position()
{
  return lpos;
}

char read_char() {
  int i;
  for (i = 0; file[curr + i] == '\n'; i++);
  curr += i;

  if (i > 0 && file[curr] != '\0') {
    line++;
    lpos = 0;
    for (i = curr; file[i] != '\n' && file[i] != '\0'; i++);

    if (file[i] == '\n') {
      file[i] = '\0';
      fprintf(lst, "%4d: %s\n", line, &file[curr]);
      file[i] = '\n';
    } else {
      fprintf(lst, "%4d: %s\n", line, &file[curr]);
    }
  }

  lpos++;
  return file[curr++];
}

token_t get_token() {
  memset(lexeme, 0, 32);
  char c;
  for (c = read_char(); isspace(c); c = read_char());

  if (c == '\0') {
    return EOFT;
  }

  token_t t;
  int len = dfa_start(&file[curr - 1], &t);

  int l = 0;
  if (len > 31) {
    l = 31;
    t = ERROR;
  } else {
    l = len + 1;
  }
  memcpy(lexeme, &file[curr - 1], (size_t) l);

  if (t == ERROR) {
    char err[64];
    snprintf(err, 64, "Error at %d,%d: invalid character: %s", line, lpos, lexeme);
    error_message(err);
    errs++;
  } else if (t == IDENT) {
    if (strcmp("int", lexeme) == 0) {
      t = INTTYPE;
    } else if (strcmp("double", lexeme) == 0) {
      t = DBLTYPE;
    }
  }

  curr += len;
  lpos += len;
  return t;
}

char *get_lexeme() {
  return lexeme;
}

void error_message(char *msg) {
  strcat(msg, "\n");
  fputs(msg, lst);
}

void end_lex() {
  fprintf(lst, "found %d Lexical errors in %s\n", errs, fn);
}
