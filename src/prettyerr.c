#define LINE_LENGTH 256
#include <stdio.h>
#include "include/compiler.h"

void print_error(char *msg) {
	printf("%s(%i, %i): \x1B[31merror\x1B[0m: %s\n", FILENAME, LINE, COLUMN, msg); 
	/* read the line and display it*/
	char line[LINE_LENGTH];
	fseek(SOURCE, CHAR - COLUMN, SEEK_SET);
	printf("\t%i | %s", LINE, fgets(line, LINE_LENGTH, SOURCE));
	/* print a 'cursor' that points to the error */	
	printf("\t ");
	int x = LINE;
	while ( x /= 10 ) /*print space with the digit length of LINE */
		printf(" "); 
	printf(" |");
	for(int i = 0; i < COLUMN; i++) { /*print space until it reaches the column that has error*/
		printf(" ");
	}
	printf("^\n\n");	
}
