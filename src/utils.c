#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stddef.h>
#include "include/compiler.h"

/* the following function creates a new token from the given arguments */
Token t_init_token(char *value, int tag) {
	Token token; 
	int i = 0;
	while (*(value+i) != '\0' || i < 32) {
		token.value[i] = *(value+i);
		i++;
	}
	token.tag = tag;
	return token;
}	

/* the following function returns true if the argument 'ch' given is one of the first operator combination 
 * (if the operator is += then + is the first operator combination) and returns false otherwise */
bool is_operator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || 
			ch == '=' || ch == '!' || ch == '>' || ch == '<' || 
			ch == '&' || ch == '|' || ch == '^' || ch == '-') {
		return true;
	} else {
		return false;
	}
}

/* the following function returns true if 'ch' is one of the allowed first characters of identifier */
bool is_identifier(char ch) {
	if (isalpha(ch) || ch == '_') {
		return true;
	} else {
		return false;
	}
}
