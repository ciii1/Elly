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

Token parse_next_token() {
	
	return lex_next_token();
}
