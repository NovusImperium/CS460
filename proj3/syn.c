#include "syn.h"
#include <stdio.h>
#include <string.h>


char *currentlex;
token_t tok;
int errors = 0;
FILE *db; //db is the debug file
FILE *lst; //lst is the list of errors
FILE *sym; //symbols from symbol table
char *errmsg;


//extern Filein filein;

void start(char *filename) {
    char *filex = ".";
    char extension[256];
    long posi = strcspn(filename, filex);
    char extention[3];
    char temp[256];
    char file[256];
    char file2[256];
    char file3[256];

    strcpy(temp, filename);
    strncpy(file, filename, posi);
    strcpy(file2, file);
    strcpy(file3, file);


    long i;

    for (i = posi; i < strlen(temp); i++) {
        strcat(extention, &temp[i + 1]);
    }

    if (strncmp(extention, "in", 1) == 0) {

        char *debugfile = strcat(file, ".dbg");
        char *listfile = strcat(file2, ".lst");
        char *symbolfile = strcat(file3, ".sym");

        db = fopen(debugfile, "w");
        lst = fopen(listfile, "w");
        sym = fopen(symbolfile, "w");

        init_lex(filename);
        tok = get_token();
        currentlex = get_lexeme();
        while (tok == ERROR) {
            accept(tok);
        }
        fprintf(db, "In start current token = %s, character or string = %s\n", token_names[tok], currentlex);
        program();
    }
    else {
        printf("Infile must have .in extension\n");
        exit(1);
    }
}

void stop(void) {
    end_lex();
    fprintf(lst, "%d Syntactic errors found.\n", errors);
    fclose(db);
    fclose(lst);

    exit(1);
}

void error(char *msg) {
    errors++;
    fprintf(lst, "Syntax Error in line %d position %d: Unexpected character or string found: %s\n", get_linenum(), get_position(), msg);
}

int accept(token_t t) {
    if (tok == t) {
        tok = get_token();
        currentlex = get_lexeme();
        while (tok == ERROR) {
            tok = get_token();
            currentlex = get_lexeme();
        }
        return 1;
    }
    return 0;
}

void program(void) {
    //Check for firsts of decl
    if (tok == INTTYPE || tok == DBLTYPE) {
        fprintf(db, "In program entering decl tok = %s lexeme = %s\n", token_names[tok], currentlex);
        decl();
    }
    else {
        fprintf(db, "In program entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
    }
    //Check for semi following stmt or decl
    if (tok == SEMI) {
        accept(tok);
    } else {
        error(currentlex);
    }
    fprintf(db, "In program entering more_stmts tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_stmts();
    if (tok == EOFT)
        stop();
    else {
        error(currentlex);
    }
}

void stmt(void) {
    fprintf(db, "In stmt entering term tok = %s lexeme = %s\n", token_names[tok], currentlex);
    term();
    fprintf(db, "In stmt entering stmt_tail tok = %s lexeme = %s\n", token_names[tok], currentlex);
    stmt_tail();
}

void decl(void) {
    fprintf(db, "In decl entering ntype tok = %s lexeme = %s\n", token_names[tok], currentlex);
    ntype();

    //Check for IDENT following ntype
    if (tok == IDENT) {
        fprintf(db, "In decl returning IDENT tok = %s lexeme = %s\n", token_names[tok], currentlex);
        //ADD THE VARIABLE TO THE SYMBOL TABLE HERE....
        accept(tok);
    }
    else {
        error(currentlex);
    }
    fprintf(db, "In decl entering decl_tail tok = %s lexeme = %s\n", token_names[tok], currentlex);
    decl_tail();

    fprintf(db, "In decl entering more_decls tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_decls();

}

void more_decls(void) {
    if (tok != COMMA) {
        return;
    }
    else if (tok == COMMA) {
        accept(tok);
        if (tok == IDENT) {
            accept(tok);
            fprintf(db, "In more_decls entering decl_tail tok = %s lexeme = %s\n", token_names[tok], currentlex);
            decl_tail();
        }
        else {
            error(currentlex);
        }
    }
    fprintf(db, "In more_decls entering more_decls tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_decls();
}

void ntype(void) {
    //Check for terminal symbol in ntype
    if (tok == INTTYPE || tok == DBLTYPE) {
        fprintf(db, "In ntype returning tok = %s lexeme = %s\n", token_names[tok], currentlex);
        accept(tok);
    }
    else {
        error(currentlex);
    }
}

void more_stmts(void) {
    if (tok == EOFT) {
        return;
    }
        //Check for firsts of decl
    else if (tok == INTTYPE || tok == DBLTYPE) {
        fprintf(db, "In more_stmts entering decl tok = %s lexeme = %s\n", token_names[tok], currentlex);
        decl();
    }
    else {
        fprintf(db, "In more_stmts entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
    }
    //Check for semi following stmt or decl
    if (tok == SEMI) {
        accept(tok);
    }
    else {
        error(currentlex);
    }
    if (tok == EOFT) {
        return;
    }
    fprintf(db, "In more_stmts entering more_stmts tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_stmts();
}

void decl_tail(void) {
    //Check for ASSIGN terminal symbol in decl_tail
    if (tok == ASSIGN) {
        fprintf(db, "In decl_tail accepting ASSIGN tok = %s lexeme = %s\n", token_names[tok], currentlex);
        accept(tok);
    }
    else {}
    fprintf(db, "In decl_tail entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
    stmt();
}

void term(void) {
    //Check for LPAREN before stmt in term
    if (tok == LPAREN) {
        accept(tok);
        fprintf(db, "In term entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
        //Check for RPAREN terminal symbol after return from stmt in term
        if (tok == RPAREN) {
            accept(tok);
        }
        else {
            error(currentlex);
        }
    }
    else {
        fprintf(db, "In term entering pre tok = %s lexeme = %s\n", token_names[tok], currentlex);
        pre();
        fprintf(db, "In term entering var tok = %s lexeme = %s\n", token_names[tok], currentlex);
        var();
        fprintf(db, "In term entering post tok = %s lexeme = %s\n", token_names[tok], currentlex);
        post();
    }
}

void pre(void) {
    //Check for terminal Symbols in pre
    if (tok == MINUSMINUS || tok == MINUS || tok == NOT || tok == PLUS || tok == PLUSPLUS ||
            tok == TILDE) {
        fprintf(db, "In pre entering pre tok = %s lexeme = %s\n", token_names[tok], currentlex);
        accept(tok);
        pre();
    }
    else {}
}

void stmt_tail(void) {
    if (tok == SEMI) {
    }
    else if (tok == AND || tok == ANDEQ || tok == ASSIGN || tok == DIV || tok == DIVEQ || tok == EQUALTO ||
            tok == EXP || tok == EXPEQ || tok == GT || tok == GTE || tok == LOGAND || tok == LOGOR || tok == LT ||
            tok == LTE || tok == MINUS || tok == MINUSEQ || tok == MOD || tok == MODEQ || tok == MULT || tok == MULTEQ ||
            tok == NOTEQ || tok == OR || tok == OREQ || tok == PLUS || tok == PLUSEQ || tok == SHIFTL || tok == SHIFTLEQ ||
            tok == SHIFTR || tok == SHIFTREQ || tok == XOR || tok == XOREQ) {
        fprintf(db, "In stmt_tail entering binop tok = %s lexeme = %s\n", token_names[tok], currentlex);
        binop();
        fprintf(db, "In stmt_tail entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
    }
    else if (tok == QUEST) {
        fprintf(db, "In stme_tail entering stme tok == %s lexeme = %s\n", token_names[tok], currentlex);
        accept(tok);
        stmt();
        if (tok == COLON) {
            fprintf(db, "In stmt_tail entering stmt tok == %s lexeme = %s\n,", token_names[tok], currentlex);
            accept(tok);
            stmt();
        }
    }
}

void var(void) {
    //Check for terminal symbols for var
    if (tok == IDENT || tok == NUMLIT) {
        //TEST TO SEE IF THE SYMBOL IS IN THE SYMBOL TABLE.  IF NOT ADD IT IN....
        //IF TOKEN IS NUMNUT IT IS A NUMLIT WITH A DECIMAL POINT IN IT
        fprintf(db, "In var accepting token tok = %s lex = %s\n", token_names[tok], currentlex);
        accept(tok);
    }
    else {
        fprintf(db, "In var sending error tok = %s lex = %s\n", token_names[tok], currentlex);
        error(currentlex);
    }
}

void post(void) {
    //Check for terminal symbol for post
    if (tok == PLUSPLUS || tok == MINUSMINUS) {
        accept(tok);
    }
}

void binop(void) {
    //Check for terminal symbol for binop
    if (tok == AND || tok == ANDEQ || tok == ASSIGN || tok == DIV || tok == DIVEQ || tok == EQUALTO ||
            tok == EXP || tok == EXPEQ || tok == GT || tok == GTE || tok == LOGAND || tok == LOGOR ||
            tok == LT || tok == LTE || tok == MINUS || tok == MINUSEQ || tok == MOD || tok == MODEQ ||
            tok == MULT || tok == MULTEQ || tok == NOTEQ || tok == OR || tok == OREQ || tok == PLUS ||
            tok == PLUSEQ || tok == SHIFTL || tok == SHIFTLEQ || tok == SHIFTR || tok == SHIFTREQ ||
            tok == XOR || tok == XOREQ) {
        accept(tok);
    }
    else {
        error(currentlex);
    }
}
