#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stddef.h>
#include "include/defs.h"
#include "include/darr.h"


void darr_init(darr_t* darr) {
	darr->arr = malloc(INIT_DARR_SPACE);
	darr->end = darr->arr;
	darr->alloc_end = darr->arr + INIT_DARR_SPACE;
}

void darr_expand(darr_t* dest, int size) {
	void* old_arr = dest->arr;
	int new_size = dest->end - dest->arr + size;
	if ((dest->arr = realloc(dest->arr, new_size)) == NULL) {
		printf("FATAL: failed to allocate memory for darr\n");
		exit(1);
	}
	dest->alloc_end = dest->arr + new_size;
	dest->end = dest->arr + (dest->end - old_arr);
}

void darr_append(darr_t* dest, void* val) {
	/* realloc if the capacity is not enough */
	if (dest->end + sizeof(void*) >= dest->alloc_end) {
		/* double the size */
		darr_expand(dest, dest->alloc_end - dest->arr);
	}
	/* ok we need a better way, pls help */
	*((char**)(dest->end)) = ((char*)val);
	dest->end += sizeof(void*);
}

int darr_length(darr_t* darr) {
	return darr->end - darr->arr;
}
