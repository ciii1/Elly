#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "include/compiler.h"

Token t_init_token(char *value, int tag) {
	Token token; 
	for(int i = 0; i < 32; i++) {
		token.value[i] = *(value+i);
	}
	token.tag = tag;
	return token;
}	
