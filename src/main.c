#include <stdio.h>
#include <stdlib.h>
#include "include/main.h"


FILE *SOURCE;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("usage: elly <file>\n");	
		return 1;
	}

	if ((SOURCE = fopen(argv[1],"r")) != NULL) {
		printf("compiling '%s'\n", argv[1]);
	} else {
		printf("can't open '%s'\n", argv[1]);
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
