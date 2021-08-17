#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include "include/compiler.h"
#include "include/utils.h"

char PEEK() {
	char ch;
	ch = fgetc(SOURCE);
	ungetc(ch, SOURCE);
	return ch;
}

char NEXT() {
	char ch;
	ch = fgetc(SOURCE);
	COLUMN++;
	if (ch == '\n') {
		LINE++;
		COLUMN = 1;
	}
	return PEEK();
}

Token lex_next_token() {
	char str[32];
	int str_counter = 0;

	signed char ch = PEEK();
	/* check if the current character is EOF */
	if (ch == EOF) { 
		return t_init_token(str, EOF_T);
	}

	/* do the actual lexing stuff here */
	char tag;
	if (isalpha(ch) || ch == '_' || ch == '-') { /* check if it's a variable */
		while (isalnum(ch) || ch == '_' || ch == '-') {	
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
		}		
		tag = VARIABLE_T;
	} else if (isdigit(ch)) { /*if it's a number*/
		bool is_float = false;
		while (isdigit(ch)) {	
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
			if(ch == '.' && is_float == false) {/* check if its a float */
				str[str_counter] = ch;
				str_counter++;
				ch = NEXT();
				is_float = true;
			}
		}	
		if(is_float) {
			tag = FLOAT_T;	
		} else {
			tag = INT_T;
		}
	} else { /*else, error*/
		print_error("Invalid token");
		exit(1);
	} 
	
	str[str_counter] = '\0';	

	/* skip whitespaces */
	while (ch == ' ' || ch == '\t' || ch == '\n') {	
		ch = NEXT();
	}

	return t_init_token(str, tag);
}	

char lex_peek_token() {
	return PEEK();
}
