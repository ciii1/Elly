/* This file will generate code */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "include/defs.h"
#include "include/types.h"
#include "include/compiler.h"
#include "include/lexer.h"
#include "include/dstr.h"
#include "ht/ht.h"

#define OUTPUT_SIZE 16777216 /* 8^8 */

ht* var_ht;

/* return the binding power (precedence) of 'val' */
short expr_bp_of(tag_t tag2) {
	switch (tag2) {
		case MUL_OPR_T:
		case DIV_OPR_T:
		case MOD_OPR_T:
			return 6;
			break;
		case ADD_OPR_T:
		case SUB_OPR_T:
			return 5;
			break;
		case CGREATER_OPR_T:
		case CLESS_OPR_T:
		case CGREATER_EQU_OPR_T:
		case CLESS_EQU_OPR_T:
		case CEQU_OPR_T:
			return 4;
			break;
		case LAND_OPR_T:
		case LOR_OPR_T:
			return 3;
			break;
		case AND_OPR_T:
		case OR_OPR_T:
		case XOR_OPR_T:
			return 2;
			break;
		case ASSIGNMENT_OPR_T:
		case INC_OPR_T:
		case DEC_OPR_T:
		case MUL_ASSIGN_OPR_T:
		case DIV_ASSIGN_OPR_T:
		case MOD_ASSIGN_OPR_T:
		case AND_ASSIGN_OPR_T:
		case OR_ASSIGN_OPR_T:
		case XOR_ASSIGN_OPR_T:
			return 1;
			break;
		default:
			return -1;
	}
}

/* return the associativity of 'val' */
bool expr_is_r_assoc(tag_t tag2) {
	switch (tag2) {
		case MUL_OPR_T:
		case DIV_OPR_T:
		case MOD_OPR_T:
		case ADD_OPR_T:
		case SUB_OPR_T:
		case AND_OPR_T:
		case OR_OPR_T:
		case XOR_OPR_T:
		case CGREATER_OPR_T:
		case CLESS_OPR_T:
		case CGREATER_EQU_OPR_T:
		case CLESS_EQU_OPR_T:
		case CEQU_OPR_T:
		case LAND_OPR_T:
		case LOR_OPR_T:
			return false;
			break;
		case ASSIGNMENT_OPR_T:
		case INC_OPR_T:
		case DEC_OPR_T:
		case MUL_ASSIGN_OPR_T:
		case DIV_ASSIGN_OPR_T:
		case MOD_ASSIGN_OPR_T:
		case AND_ASSIGN_OPR_T:
		case OR_ASSIGN_OPR_T:
		case XOR_ASSIGN_OPR_T:
			return true;
			break;
	}
}

/* return NULL if the token is not an operator (+, -, *, /, |, &, ^, >, <, %, =,  ||, &&, >>, <<, ==, -=, +=, -=, *=, /=, |= &=, <= , >=, >>=, <<=, ^=) 
 * Otherwise, returns the elly instruction function of the 'operator' */
char* expr_get_operator(tag_t tag2) {
	switch (tag2) {
		case ADD_OPR_T:
			return "__add__";
			break;
		case SUB_OPR_T:
			return "__sub__";
			break;
		case MUL_OPR_T:
			return "__mul__";
			break;
		case DIV_OPR_T:
			return "__div__";
			break;
		case MOD_OPR_T:
			return "__mod__";
			break;
		case AND_OPR_T:
			return "__and__";
			break;
		case OR_OPR_T:
			return "__ror__";
			break;
		case XOR_OPR_T:
			return "__xor__";
			break;
		case CGREATER_OPR_T:
			return "__cgr__";
			break;
		case CLESS_OPR_T:
			return "__cls__";
			break;
		case ASSIGNMENT_OPR_T:
			return "__mov__";
			break;
		case INC_OPR_T:
			return "__inc__";
			break;
		case DEC_OPR_T:
			return "__dec__";
			break;
		case MUL_ASSIGN_OPR_T:
			return "__mulm_";
			break;
		case DIV_ASSIGN_OPR_T:
			return "__divm_";
			break;
		case MOD_ASSIGN_OPR_T:
			return "__modm_";
			break;
		case AND_ASSIGN_OPR_T:
			return "__andm_";
			break;
		case OR_ASSIGN_OPR_T:
			return "__rorm_";
			break;
		case XOR_ASSIGN_OPR_T:
			return "__xorm_";
			break;
		case CGREATER_EQU_OPR_T:
			return "__cge__";
			break;
		case CLESS_EQU_OPR_T:
			return "__cle__";
			break;
		case CEQU_OPR_T:
			return "__equ__";
			break;
		case LAND_OPR_T:
			return "__land_";
			break;
		case LOR_OPR_T:
			return "__lor__";
			break;
		case SHR_OPR_T:
			return "__shr__";
			break;
		case SHL_OPR_T:
			return "__shl__";
			break;
		case SHR_ASSIGN_OPR_T:
			return "__shrm_";
			break;
		case SHL_ASSIGN_OPR_T:
			return "__shlm__";
			break;
		default:
			return NULL;
	}
}

/* return false if the token is not a value (wether it's a constant or not).
 * Otherwise, append the generated output to w_area and return the first character of the generated type 
 * Note: The function wouldn't lookup to check it's existence for us */
char expr_gen_value(dstr_t* w_area) {
	token_t token = lex_peek_token();
	switch (token.tag) {
		case VARIABLE_T:
			dstr_append(w_area, token.value);
			lex_next_token();
			return 'v';
			break;
	 	case FLOAT_T:
			dstr_append(w_area, token.value);
			lex_next_token();
			return 'f';
		case INT_T:
			dstr_append(w_area, token.value);
			lex_next_token();
			return 'i';
		case STRING_T: 
			dstr_append(w_area, "\"");
			dstr_append(w_area, token.value);
			dstr_append(w_area, "\"");
			lex_next_token();
			return 's';
		default:
			return false;
	}
}

/* return 0 if the expression is invalid.
 * Otherwise, return the first char of the expression type (i.e variable = 'v', int/float = 'n')
 * example: a+b*c --> __e_add__(a, __e_mul__(b, c)). 
 * we use pratt-parsing-like algorithm to handle precedence*/
char expr_gen(dstr_t* w_area, int rbp) {
	dstr_t buff;
	dstr_init(&buff);

	/* TODO: make this dynamic */
	tag_t l_parens[100]; /* contains tag2s of operators*/
	int lp_counter = 0;

	char l_type;
	token_t l_tkn;
	var_data_t* l_var_data;

	char r_type[100]; /* contains types of rvalues */
	int rt_counter = 0;
	tag_t o_tag2; /* operator tag2 */

	l_tkn = lex_peek_token();

	if(l_tkn.tag2 == LEFT_PAREN_T) {
		lex_next_token();
		if(!expr_gen(&buff, 0)) {
			return false;
		}
		if(lex_next_token().tag2 != RIGHT_PAREN_T) {
			return false;
		}
	} else if (!(l_type = expr_gen_value(&buff))) {
	       	return false;
	}

	/* check if the variable exist on ht and store the result to l_var_data, else, error 
	 * (the expr_gen_value() wouldn't check if a name exists or not*/
	if (l_type == 'v') {
		if ((l_var_data = (var_data_t*)ht_get(var_ht, l_tkn.value)) == NULL) {
			char msg[17+MAX_TOKEN_VALUE];
			sprintf(msg, "undeclared name %s", l_tkn.value);
			exit_error(msg);
		}
	}

	while (expr_bp_of(lex_peek_token().tag2) > rbp) {
		/* insert ',' and rvalue */
		if(lex_peek_token().tag2 == LEFT_PAREN_T) {
			lex_next_token();
			if(!expr_gen(&buff, 0)) {
				return false;
			}
			if(lex_next_token().tag2 != RIGHT_PAREN_T) {
				return false;
			}
		}

		o_tag2 = lex_next_token().tag2;

		/* check if l_type is const and the operator is one of the assignment operators */
		if (l_type == 'v' && l_var_data->is_const) {
			switch(o_tag2) {
				case ASSIGNMENT_OPR_T:
				case INC_OPR_T:
				case DEC_OPR_T:
				case MUL_ASSIGN_OPR_T:
				case DIV_ASSIGN_OPR_T:
				case MOD_ASSIGN_OPR_T:
				case AND_ASSIGN_OPR_T:
				case OR_ASSIGN_OPR_T:
				case XOR_ASSIGN_OPR_T:
				case SHR_ASSIGN_OPR_T:
				case SHL_ASSIGN_OPR_T:
					exit_error("trying to assign to a constant");
					break;
			}
		}

		dstr_append(&buff, ", ");
		if (!expr_is_r_assoc(o_tag2)) {
			if(!(r_type[rt_counter] = expr_gen(&buff, expr_bp_of(o_tag2)))) {
				return false;	
			}
		} else {
			if(!(r_type[rt_counter] = expr_gen(&buff, expr_bp_of(o_tag2)-1))) {
				return false;	
			}
		}
		rt_counter++;
	
		/* the left assoc */
		dstr_append_char(&buff, ')');
		l_parens[lp_counter] = o_tag2;
		lp_counter++;
		
		printf("", o_tag2); /* Without this, somehow it produces weird error */
	}
	dstr_append_char(&buff, '\0');

	/* insert l_parens in reverse order */
	for (int i = lp_counter-1; i >= 0; i--) {
		dstr_append(w_area, expr_get_operator(l_parens[i]));
		dstr_append_char(w_area, '(');

		/*append the type of lvalue */
		dstr_append_char(w_area, '\'');
		dstr_append_char(w_area, l_type);
		dstr_append(w_area, "\', ");

		/*append the type of rvalues */
		dstr_append_char(w_area, '\'');
		dstr_append_char(w_area, r_type[i]);
		dstr_append(w_area, "\', ");
	}

	/* insert the buffer */
	dstr_append(w_area, buff.str);

	dstr_free(&buff);

	return l_type;
}

/* return false if not matches the grammar of variable declaration.
 * Otherwise, returns true */
bool vardecl_gen(dstr_t* w_area) {
	if (lex_peek_token().tag2 != DECL_T) {
		return false;
	}

	char* name;
	bool is_const;

	/* parse the code */
	if (lex_next_token().tag == CONST_T) {
		is_const = true;
	}

	token_t name_tkn = lex_peek_token();
	if (name_tkn.tag != VARIABLE_T) {
		return false;
	}
	name = name_tkn.value;
	/* don't jump to the next token because we're going to parse this with expr_gen() */

	/* check if the item exist on var_ht */
	if (ht_get(var_ht, name) != NULL) {
		char msg[17+MAX_TOKEN_VALUE];
		sprintf(msg, "redeclaration of global variable %s", lex_peek_token().value);
		exit_error(msg);
	}
	
	/* if it doesn't exist, insert to the var_ht */
	var_data_t* var_val;
	var_val = malloc(sizeof(var_data_t));
	var_val->is_const = false;

	ht_set(var_ht, name, var_val);

	/* generate the code based from the parsed data */
	dstr_append(w_area, TOSTRING(E_VARIABLE_STRUCT) " ");
	dstr_append(w_area, name);
	
	if(lex_jump_peek_token().tag2 != ASSIGNMENT_OPR_T) {
		var_val->is_const = is_const;
		lex_next_token();
		return true;
	}

	dstr_append(w_area, ";\n");
	
	/* generate the expression */
	if(!expr_gen(w_area, 0)){
		return false;
	}

	var_val->is_const = is_const;
	return true;
}


/* main entry, return a char pointer to the generated C code
 * (!) NOTE: return value must be freed after use */
char* generate_code() {
	dstr_t w_area;
	dstr_init(&w_area);

	if((var_ht = ht_create()) == NULL) {
		printf("FATAL ERROR: failed to initialize ht");
		exit(1);
	}

	do {
		/* try to match all the grammar */
		if (!vardecl_gen(&w_area) &&
		    !expr_gen(&w_area, 0)) { 
			char msg[18+MAX_TOKEN_VALUE];
			sprintf(msg, "unexpected token: %s", lex_peek_token().value);
			exit_error(msg);
		}
		
		if (lex_peek_token().tag == SEMICOLON_T) {
			lex_next_token();
		}
		dstr_append(&w_area, ";\n");
	} while(lex_peek_token().tag != EOF_T);

	ht_destroy(var_ht);

	return w_area.str;
}
