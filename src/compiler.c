#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/defs.h"
#include "include/types.h"
#include "include/lexer.h"
#include "include/generator.h"
#include "ht/ht.h"


FILE *SOURCE;
char *FILENAME;
int CHAR = 1;
int LINE = 1;
int COLUMN = 1;

/* The entry point */
int compile(char *filename) {
	if ((SOURCE = fopen(filename, "r")) != NULL) {
		printf("compiling '%s'\n", filename);
		FILENAME = filename;
	} else {
		printf("can't open '%s'\n", filename);
		return 1;
	}
	
	char* output = generate_code();
	printf("%s\n", output);
	free(output);

	fclose(SOURCE);

	return 0;
}
