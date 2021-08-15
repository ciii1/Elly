#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "include/compiler.h"

char NEXT() {
	char ch;
	if((ch = fgetc(SOURCE)) != EOF)
		return ch;
	return EOF;
}

char PEEK() {
	char ch;
	if((ch = fgetc(SOURCE)) != EOF) {
		ungetc(ch, SOURCE);
		return ch;
	}
	return EOF;
}

Token lex_next_token() {
	char str[32];
	int str_counter = 0;
	
	char ch = NEXT();
	/* check if the current character is EOF */
	if (ch == EOF) { 
		return t_init_token(str, EOF_T);
	}

	/* do the actual lexing stuff here */
	char tag;
	if (isalpha(ch) || ch == '_' || ch == '-') {
		while (isalnum(ch) || ch == '_' || ch == '-') {	
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
		}		
		tag = VARIABLE_T;
	}
	str[str_counter] = '\0';
	
	/* skip whitespaces */
	while (ch != EOF) {	
		ch = PEEK();
		if (ch == ' ' || ch == '\t' || ch == '\n') {
			NEXT();
		} else {
			break;
		}
	}

	return t_init_token(str, tag);
}	

char lex_peek_token() {
	return PEEK();
}
