#define LINE_LENGTH 256
#include <stdio.h>
#include "include/compiler.h"

void print_error(char *msg) {
	printf("%s(%i, %i): \x1B[31merror\x1B[0m: %s\n", FILENAME, LINE, COLUMN, msg); 

	/* read the line content and display it*/
	char line_content[LINE_LENGTH];

	fseek(SOURCE, CHAR - COLUMN, SEEK_SET); /* set the seek to the start of the line */

	if (fgets(line_content, LINE_LENGTH, SOURCE) == NULL) {
		return;
	}

	printf("\t%i | %s", LINE, line_content);

	fseek(SOURCE, CHAR, SEEK_SET);  /*set the seek back to our previous position */

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
