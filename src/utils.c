#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include "include/compiler.h"

/* the following function creates a new token from the given arguments */
Token t_init_token(char *value, Tag tag) {
	Token token; 
	int i = 0;
	while (*(value+i) != '\0' || i < 32) {
		token.value[i] = *(value+i);
		i++;
	}
	token.tag = tag;
	return token;
}	

