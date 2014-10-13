#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

int main (int argc, char * argv[])
{
	lexical_t t;
	if (argc < 2)
	{
		printf ("format: proj1 <filename>\n");
		exit (1);
	}
	init_lex (argv[1]);
	while ((t = get_token()) != eof_tok) {
        printf("\t%-12s%s\n", token_names[t], get_lexeme());
    }
	end_lex();
	return 0;
}
