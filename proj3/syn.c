#include "syn.h"
#include <string.h>


char *currentlex;
token_t currentType;
token_t tok;
int errors = 0;
FILE *dbg_file; //db is the debug file
FILE *lst_file; //lst is the list of errors
FILE *sym_file; //symbols from symbol table
char *errmsg;
table *tab;

static OpCode_type preOpTable[47] = {
        OPERR,  //ERROR,
        OPERR,  //IDENT,
        OPERR,  //INTTYPE,
        OPERR,  //DBLTYPE,
        OPERR,  //NUMLIT,
        UPLUS,  //PLUS,
        UMINUS, //MINUS,
        OPERR,  //MULT,
        OPERR,  //DIV,
        OPERR,  //MOD,
        OPERR,  //EXP,
        NEGATION, //NOT,
        OPERR,  //AND,
        OPERR,  //OR,
        OPERR,  //XOR,
        OPERR,  //ASSIGN,
        OPERR,  //LT,
        OPERR,  //GT,
        OPERR,  //SHIFTL,
        OPERR,  //SHIFTR,
        PREPP,  //PLUSPLUS,
        OPERR,  //PLUSEQ,
        PREMM,  //MINUSMINUS,
        OPERR,  //MINUSEQ,
        OPERR,  //MULTEQ,
        OPERR,  //DIVEQ,
        OPERR,  //MODEQ,
        OPERR,  //EXPEQ,
        OPERR,  //NOTEQ,
        OPERR,  //LOGAND,
        OPERR,  //ANDEQ,
        OPERR,  //LOGOR,
        OPERR,  //OREQ,
        OPERR,  //XOREQ,
        OPERR,  //EQUALTO,
        OPERR,  //SHIFTLEQ,
        OPERR,  //LTE,
        OPERR,  //SHIFTREQ,
        OPERR,  //GTE,
        OPERR,  //TILDE,
        OPERR,  //RPAREN,
        OPERR,  //LPAREN,
        OPERR,  //SEMI,
        OPERR,  //QUEST,
        OPERR,  //COLON,
        OPERR,  //COMMA,
        OPERR,  //EOFT
};

static OpCode_type postOpTable[] = {
        OPERR,  //ERROR,
        OPERR,  //IDENT,
        OPERR,  //INTTYPE,
        OPERR,  //DBLTYPE,
        OPERR,  //NUMLIT,
        OPERR,  //PLUS,
        OPERR,  //MINUS,
        OPERR,  //MULT,
        OPERR,  //DIV,
        OPERR,  //MOD,
        OPERR,  //EXP,
        OPERR,  //NOT,
        OPERR,  //AND,
        OPERR,  //OR,
        OPERR,  //XOR,
        OPERR,  //ASSIGN,
        OPERR,  //LT,
        OPERR,  //GT,
        OPERR,  //SHIFTL,
        OPERR,  //SHIFTR,
        POSTPP, //PLUSPLUS,
        OPERR,  //PLUSEQ,
        POSTMM, //MINUSMINUS,
        OPERR,  //MINUSEQ,
        OPERR,  //MULTEQ,
        OPERR,  //DIVEQ,
        OPERR,  //MODEQ,
        OPERR,  //EXPEQ,
        OPERR,  //NOTEQ,
        OPERR,  //LOGAND,
        OPERR,  //ANDEQ,
        OPERR,  //LOGOR,
        OPERR,  //OREQ,
        OPERR,  //XOREQ,
        OPERR,  //EQUALTO,
        OPERR,  //SHIFTLEQ,
        OPERR,  //LTE,
        OPERR,  //SHIFTREQ,
        OPERR,  //GTE,
        OPERR,  //TILDE,
        OPERR,  //RPAREN,
        OPERR,  //LPAREN,
        OPERR,  //SEMI,
        OPERR,  //QUEST,
        OPERR,  //COLON,
        OPERR,  //COMMA,
        OPERR,  //EOFT
};

static OpCode_type binOpTable[] = {
        OPERR,  //ERROR,
        OPERR,  //IDENT,
        OPERR,  //INTTYPE,
        OPERR,  //DBLTYPE,
        OPERR,  //NUMLIT,
        BPLUS,  //PLUS,
        BMINUS, //MINUS,
        MULTIPLY,  //MULT,
        DIVIDE,  //DIV,
        REMAINDER, //MOD,
        POWER,  //EXP,
        OPERR, //NOT,
        BITAND,  //AND,
        BITOR,  //OR,
        BITXOR,  //XOR,
        ASSIGNMENT,  //ASSIGN,
        LESSTHAN,  //LT,
        GREATERTHAN,  //GT,
        SHIFTLEFT,  //SHIFTL,
        SHIFTRIGHT,  //SHIFTR,
        OPERR,  //PLUSPLUS,
        PLUSEQUAL,  //PLUSEQ,
        OPERR,  //MINUSMINUS,
        MINUSEQUAL,  //MINUSEQ,
        MULTEQUAL,  //MULTEQ,
        DIVEQUAL,  //DIVEQ,
        REMEQUAL,  //MODEQ,
        POWEREQUAL,  //EXPEQ,
        NOTEQUAL,  //NOTEQ,
        LOGICALAND,  //LOGAND,
        BITANDEQUAL,  //ANDEQ,
        LOGICALOR,  //LOGOR,
        BITOREQUAL,  //OREQ,
        BITXOREQUAL,  //XOREQ,
        EQUAL,  //EQUALTO,
        SHIFTLEFTEQUAL,  //SHIFTLEQ,
        LESSTHANEQ,  //LTE,
        SHIFTRIGHTEQUAL,  //SHIFTREQ,
        GREATERTHANEQ,  //GTE,
        OPERR,  //TILDE,
        OPERR,  //RPAREN,
        OPERR,  //LPAREN,
        OPERR,  //SEMI,
        TERNQUESTION,  //QUEST,
        TERNCOLON,  //COLON,
        OPERR,  //COMMA,
        OPERR,  //EOFT
};

void start(char *filename) {
    char *filex = ".";
    char extension[256];
    long posi = strcspn(filename, filex);
    char extention[3];
    char in_file[256];
    char dbgfile[256];
    char lstfile[256];
    char symfile[256];

    strcpy(in_file, filename);
    strncpy(dbgfile, filename, (size_t) posi);
    strcpy(lstfile, dbgfile);
    strcpy(symfile, dbgfile);


    long i;

    for (i = posi; i < strlen(in_file); i++) {
        strcat(extention, &in_file[i + 1]);
    }

    if (strncmp(extention, "in", 1) == 0) {

        char *debugfile = strcat(dbgfile, ".dbg");
        char *listfile = strcat(lstfile, ".lst");
        char *symbolfile = strcat(symfile, ".sym");

        dbg_file = fopen(debugfile, "w");
        lst_file = fopen(listfile, "w");
        sym_file = fopen(symbolfile, "w");

        init_lex(filename);
        InitSemantic();
        tok = get_token();
        currentlex = get_lexeme();
        while (tok == ERROR) {
            accept(tok);
        }
        fprintf(dbg_file, "In start current token = %s, character or string = %s\n", token_names[tok], currentlex);
        program();
    }
    else {
        printf("Infile must have .in extension\n");
        exit(1);
    }
}

void stop(table *t, FILE *o) {
    end_lex();
    write_syms(t, o);
    fprintf(lst_file, "%d Syntactic errors found.\n", errors);
    fclose(dbg_file);
    fclose(lst_file);
    fclose(sym_file);
    exit(1);
}

void error(char *msg) {
    errors++;
    fprintf(lst_file, "Syntax Error in line %d position %d: Unexpected character or string found: %s\n", get_linenum(), get_position(), msg);
    while (tok != SEMI)
        tok = get_token();
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
        currentType = tok;
        fprintf(dbg_file, "In program entering decl tok = %s lexeme = %s\n", token_names[tok], currentlex);
        decl();
    }
    else {
        fprintf(dbg_file, "In program entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
    }
    //Check for semi following stmt or decl
    if (tok == SEMI) {
        Evaluate();
        accept(tok);
    } else {
        error(currentlex);
    }
    fprintf(dbg_file, "In program entering more_stmts tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_stmts();
    if (tok == EOFT)
        stop(tab, sym_file);
    else {
        error(currentlex);
    }
}

void stmt(void) {
    fprintf(dbg_file, "In stmt entering term tok = %s lexeme = %s\n", token_names[tok], currentlex);
    term();
    fprintf(dbg_file, "In stmt entering stmt_tail tok = %s lexeme = %s\n", token_names[tok], currentlex);
    stmt_tail();
}

void decl(void) {
    fprintf(dbg_file, "In decl entering ntype tok = %s lexeme = %s\n", token_names[tok], currentlex);
    ntype();

    //Check for IDENT following ntype
    if (tok == IDENT) {
        fprintf(dbg_file, "In decl returning IDENT tok = %s lexeme = %s\n", token_names[tok], currentlex);
        //ADD THE VARIABLE TO THE SYMBOL TABLE HERE....
        NewDeclaration(tab, currentlex);
        accept(tok);
    }
    else {
        error(currentlex);
    }
    fprintf(dbg_file, "In decl entering decl_tail tok = %s lexeme = %s\n", token_names[tok], currentlex);
    decl_tail();

    fprintf(dbg_file, "In decl entering more_decls tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_decls();

}

void more_decls(void) {
    if (tok != COMMA) {
        return;
    }
    else if (tok == COMMA) {
        accept(tok);
        if (tok == IDENT) {
            NewDeclaration(tab, currentlex);
            accept(tok);
            fprintf(dbg_file, "In more_decls entering decl_tail tok = %s lexeme = %s\n", token_names[tok], currentlex);
            decl_tail();
        }
        else {
            error(currentlex);
        }
    }
    fprintf(dbg_file, "In more_decls entering more_decls tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_decls();
}

void ntype(void) {
    //Check for terminal symbol in ntype
    if (tok == INTTYPE || tok == DBLTYPE) {
        fprintf(dbg_file, "In ntype returning tok = %s lexeme = %s\n", token_names[tok], currentlex);
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
        fprintf(dbg_file, "In more_stmts entering decl tok = %s lexeme = %s\n", token_names[tok], currentlex);
        decl();
    }
    else {
        fprintf(dbg_file, "In more_stmts entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
    }
    //Check for semi following stmt or decl
    if (tok == SEMI) {
        Evaluate();
        accept(tok);
    }
    else {
        error(currentlex);
    }
    if (tok == EOFT) {
        return;
    }
    fprintf(dbg_file, "In more_stmts entering more_stmts tok = %s lexeme = %s\n", token_names[tok], currentlex);
    more_stmts();
}

void decl_tail(void) {
    //Check for ASSIGN terminal symbol in decl_tail
    if (tok == ASSIGN) {
        OperatorFound(ASSIGNMENT);
        fprintf(dbg_file, "In decl_tail accepting ASSIGN tok = %s lexeme = %s\n", token_names[tok], currentlex);
        accept(tok);
    }
    else {}
    fprintf(dbg_file, "In decl_tail entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
    stmt();
}

void term(void) {
    //Check for LPAREN before stmt in term
    if (tok == LPAREN) {
        NewScopeFound();
        accept(tok);
        fprintf(dbg_file, "In term entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
        //Check for RPAREN terminal symbol after return from stmt in term
        if (tok == RPAREN) {
            EndScopeFound();
            accept(tok);
        }
        else {
            error(currentlex);
        }
    }
    else {
        fprintf(dbg_file, "In term entering pre tok = %s lexeme = %s\n", token_names[tok], currentlex);
        pre();
        fprintf(dbg_file, "In term entering var tok = %s lexeme = %s\n", token_names[tok], currentlex);
        var();
        fprintf(dbg_file, "In term entering post tok = %s lexeme = %s\n", token_names[tok], currentlex);
        post();
    }
}

void pre(void) {
    //Check for terminal Symbols in pre
    if (tok == MINUSMINUS || tok == MINUS || tok == NOT || tok == PLUS || tok == PLUSPLUS ||
            tok == TILDE) {
        OperatorFound(preOpTable[tok]);
        fprintf(dbg_file, "In pre entering pre tok = %s lexeme = %s\n", token_names[tok], currentlex);
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
        OperatorFound(binOpTable[tok]);
        fprintf(dbg_file, "In stmt_tail entering binop tok = %s lexeme = %s\n", token_names[tok], currentlex);
        binop();
        fprintf(dbg_file, "In stmt_tail entering stmt tok = %s lexeme = %s\n", token_names[tok], currentlex);
        stmt();
    }
    else if (tok == QUEST) {
        fprintf(dbg_file, "In stme_tail entering stme tok == %s lexeme = %s\n", token_names[tok], currentlex);
        accept(tok);
        stmt();
        if (tok == COLON) {
            fprintf(dbg_file, "In stmt_tail entering stmt tok == %s lexeme = %s\n,", token_names[tok], currentlex);
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
        if (tok == IDENT) {
            VariableFound(currentlex);
        } else {
            NumLitFound(currentlex);
        }
        fprintf(dbg_file, "In var accepting token tok = %s lex = %s\n", token_names[tok], currentlex);
        accept(tok);
    } else {
        fprintf(dbg_file, "In var sending error tok = %s lex = %s\n", token_names[tok], currentlex);
        error(currentlex);
    }
}

void post(void) {
    //Check for terminal symbol for post
    if (tok == PLUSPLUS || tok == MINUSMINUS) {
        OperatorFound(postOpTable[tok]);
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
        OperatorFound(binOpTable[tok]);
        accept(tok);
    }
    else {
        error(currentlex);
    }
}
