#include <stdio.h>
#include "include/compiler.h"

void print_error(char *msg) {
	printf("%s(%i, %i): \x1B[31merror\x1B[0m: %s\n", FILENAME, LINE, COLUMN, msg); 
}
