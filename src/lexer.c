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
	CHAR++;
	if (ch == '\n') {
		LINE++;
		COLUMN = 1;
	}
	return PEEK();
}

Token lex_next_token() {
	signed char ch = PEEK();
	char str[32];
	int str_counter = 0;
	char tag;
	if (ch == EOF) {
		tag = EOF_T;
	} else if (is_identifier(ch)) { /* check if it's an identifier */
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
	} else if (ch == '\'' || ch == '\"') { /* if it's a string */
		char str_symbol = ch; /* symbol that were used to make the string */
		ch = NEXT();
		while (ch != str_symbol) {
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
			if(ch == '\n') {
				print_error("Unexpected end of line while scanning string literal");
				exit(1);
			}
		}
		ch = NEXT();
		tag = STRING_T;
	} else if(is_operator(ch)) { /* if it's one of the operators */
		str[str_counter] = ch;
		str_counter++;

		char head_operator = ch;
		ch = NEXT();
		if (is_operator(ch)) {
			if (ch == '=' ||  /* allow = after all operator */
					ch == '&' && head_operator == '&' || /* allow & after & (&&)*/
					ch == '|' && head_operator == '|' ){ /* allow | after | (||)*/
						str[str_counter] = ch;
						str_counter++;
						ch = NEXT();	
			}
		}
		tag = OPERATOR_T;
	} else { /*else, error*/
		print_error("Invalid token");
		exit(1);
	}

	/* add an escape character after the string */	
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
