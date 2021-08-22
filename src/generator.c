#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "include/types.h"
#include "include/defs.h"
#include "include/compiler.h"
#include "include/lexer.h"
#include "include/utils.h"

/* return a char pointer to the generated C code
 * (!) NOTE: return value must be freed after use */
char* generate_code() {
	char* output_space = malloc(16777216);
	int counter = 0;
	Token tok;
	while ((tok = lex_next_token()).tag != EOF_T) {
		int i = 0;
		while(tok.value[i] != '\0' && tok.tag != EOF_T) {
			output_space[counter] = tok.value[i];
			counter++;
			i++;
		}
	}
	output_space[counter] = '\0';
	return output_space;
}
