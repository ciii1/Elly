/* contains functions to generate c code */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "../include/defs.h"
#include "../include/dstr.h"

void c_var_decl(dstr_t* w_area, char* name, char* value, bool is_const) {
	if (is_const) {
		dstr_append(w_area, "const " TOSTRING(VARIABLE_STRUCT) " ");
	} else {
		dstr_append(w_area, TOSTRING(VARIABLE_STRUCT) " ");
	}
	dstr_append(w_area, name);
	
	/* if there's no init */
	if(value == NULL) {
		dstr_append(w_area, ";\n");
		dstr_append(w_area, TOSTRING(NULLIFY_FUNC) "(");
		dstr_append(w_area, name);
		dstr_append(w_area, ";\n");
	} else {
		dstr_append(w_area, ";\n");
		dstr_append(w_area, value);
		dstr_append(w_area, ";");
	}
}
