#include <stdio.h>
#include "include/compiler.h"

void print_error(char *msg) {
	printf("Error: %s, at file '%s', line %i\n", msg, FILENAME, LINE); 
}
