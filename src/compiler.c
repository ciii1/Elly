#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"


FILE *SOURCE;
char *FILENAME;
int LINE = 1;
int COLUMN = 1;

int compile(char *filename) {
	if ((SOURCE = fopen(filename,"r")) != NULL) {
		printf("compiling '%s'\n", filename);
		FILENAME = filename;
	} else {
		printf("can't open '%s'\n", filename);
		return 1;
	}
	
	Token token = lex_next_token();	
	int i = 0;
	while (token.tag != EOF_T) {
		printf("Token%i\n  Value:%s\n  Tag:%i\n", i, token.value, token.tag);
		token = lex_next_token();	
		i += 1;
	}
	fclose(SOURCE);

	return 0;
}
