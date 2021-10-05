#define LINE_LENGTH 256
#include <stdio.h>
#include <stdlib.h>
#include "include/compiler.h"

void exit_error(char *msg) {
	printf("%s(%i, %i): \x1B[31merror\x1B[0m: %s\n", FILENAME, LINE, COLUMN, msg); 

	/* read the line content and display it*/
	char line_content[LINE_LENGTH];

	fseek(SOURCE, CHAR - COLUMN, SEEK_SET); /* set the seek to the start of the line */
	
	/* read characters till the character that contains the error */
	if (fgets(line_content, COLUMN, SOURCE) == NULL) { 
		return;
	}

	/*print space with the digit length of LINE */
	printf("\t ");
	int x = LINE;
	while ( x /= 10 )
		printf(" "); 
	printf(" |\n");

	printf("\t%i | %s", LINE, line_content);
	
	/*read the character that contains the error and print with red color */
	char err_char = fgetc(SOURCE);
	if (err_char == '\n') {
		printf("\x1B[31m↵\x1B[0m\n\n");
	} else {
		printf("\x1B[31m%c\x1B[0m", err_char);
		fgets(line_content, LINE_LENGTH, SOURCE); /*read to the end of the line*/
		printf("%s\n", line_content);
	}
	
	fseek(SOURCE, CHAR, SEEK_SET);  /*set the seek back to our previous position+1*/
	
	exit(1); /*TODO: don't exit when encounter an error */
}
