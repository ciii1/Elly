#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/types.h"
#include "include/defs.h"
#include "include/lexer.h"
#include "include/generator.h"


FILE *SOURCE;
char *FILENAME;
int CHAR = 1;
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

	printf("Tokens: \n");	
	Token token = lex_next_token();
	int i = 0;
	while (token.tag != EOF_T) {
		printf("Token%i\n  Value:%s\n  Tag:%i\n", i, token.value, token.tag);
		token = lex_next_token();	
		i += 1;
	}

	fseek(SOURCE, 0, SEEK_SET);

	printf("Code: \n");
	char* output = generate_code();
	printf(output);
	free(output);

	fclose(SOURCE);

	return 0;
}
