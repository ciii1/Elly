#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stddef.h>
#include "include/defs.h"
#include "include/dstr.h"


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
	dstr->alloc_end = dstr->str + INIT_DSTR_SPACE;
}

void dstr_expand(dstr_t* dest, int size) {
	char* old_str = dest->str;
	int new_size = dest->end - dest->str + size;
	if ((dest->str = realloc(dest->str, new_size)) == NULL) {
		printf("FATAL: failed to allocate memory for dstr\n");
		exit(1);
	}
	dest->alloc_end = dest->str + new_size;
	dest->end = dest->str + (dest->end - old_str);
}

void dstr_append(dstr_t* dest, char* str) {
	/* realloc if the capacity is not enough */
	int str_len = strlen(str);
	if (dest->end + str_len >= dest->alloc_end) {
		dstr_expand(dest, str_len+1);
	}
	dest->end = stracpy(str, dest->end)+1;
}

void dstr_append_char(dstr_t* dest, char ch) {
	/* realloc if the capacity is not enough */
	if (dest->end + 1 >= dest->alloc_end) {
		dstr_expand(dest, INIT_DSTR_SPACE);
	}
	*(dest->end) = ch;
	dest->end++;
}
