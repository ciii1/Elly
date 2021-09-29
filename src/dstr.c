#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include "include/defs.h"
#include "include/types.h"
#include "include/compiler.h"

/* the following function creates a new token from the given arguments */
token_t t_init_token(char *value, tag_t tag, tag_t tag2) {
	token_t token; 
	int i = 0;
	while (*(value+i) != '\0' || i < MAX_TOKEN_VALUE) {
		token.value[i] = *(value+i);
		i++;
	}
	token.tag = tag;
	token.tag2 = tag2;
	return token;
}	

/* like strcpy but doesn't copy the \0 (stracpy = str_append_cpy).
 * Returns char* pointing to the last character copied. */
char* stracpy(char* source, char* dest) {
	int i = 0;
	while (*(source+i) != '\0') {
		*(dest+i) = *(source+i);
		i++;
	}
	return dest+(i-1);
}

void dstr_init(dstr_t* dstr) {
	dstr->str = malloc(INIT_DSTR_SPACE);
	dstr->end = dstr->str;
}

void dstr_erase(dstr_t* dstr) {
	while (dstr->end != dstr->str) {
		*dstr->end = 0;
		dstr->end--;
	}
	dstr->end = dstr->str;
}

void dstr_free(dstr_t* dstr) {
	if (dstr->end != dstr->str)
		dstr_erase(dstr);
	free(dstr->str);
}

void dstr_append(dstr_t* dest, char* str) {
	dest->end = stracpy(str, dest->end)+1;
}

void dstr_append_char(dstr_t* dest, char ch) {
	*(dest->end) = ch;
	dest->end++;
}
