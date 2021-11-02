#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include "include/defs.h"
#include "include/types.h"
#include "include/compiler.h"


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

static inline void dstr_double_size(dstr_t* dest) {
	char* old_str = dest->str;
	int new_size = (dest->alloc_end + INIT_DSTR_SPACE) - dest->str;
	if ((dest->str = realloc(dest->str, new_size)) == NULL) {
		printf("FATAL: failed to allocate memory for dstr\n");
		exit(1);
	}
	dest->alloc_end = dest->str + new_size;
	dest->end = dest->str + (dest->end - old_str);
}

void dstr_append(dstr_t* dest, char* str) {
	/* realloc if the capacity is not enough */
	if (dest->end + strlen(str) >= dest->alloc_end) {
		dstr_double_size(dest);
	}
	dest->end = stracpy(str, dest->end)+1;
}

void dstr_append_char(dstr_t* dest, char ch) {
	/* realloc if the capacity is not enough */
	if (dest->end + 1 >= dest->alloc_end) {
		dstr_double_size(dest);
	}
	*(dest->end) = ch;
	dest->end++;
}
