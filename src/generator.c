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
#include "include/utils.h"
#include "ht/ht.h"

#define OUTPUT_SIZE 16777216 /* 8^8 */

/* TODO: make this dynamic */
ht* var_ht[VAR_HT_STACK_INIT_SIZE];
int scope = 0;

ht* func_ht;

bool match_global_grammar(dstr_t* w_area);
bool match_local_grammar(dstr_t* w_area);

/* return var_data from 'val' on every stack of var_ht if the key is exist.
 * Otherwise, return false */
var_data_t* get_var(char* val) {
	var_data_t* out;
	for (int i = 0; i <= scope; i++) {	
		if ((out = (var_data_t*)ht_get(var_ht[i], val)) != NULL) {
			return out;
		}
	}
	return NULL;
}

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
			return TOSTRING(ADD_OPR_T_FUNC);
			break;
		case SUB_OPR_T:
			return TOSTRING(SUB_OPR_T_FUNC);
			break;
		case MUL_OPR_T:
			return TOSTRING(MUL_OPR_T_FUNC);
			break;
		case DIV_OPR_T:
			return TOSTRING(DIV_OPR_T_FUNC);
			break;
		case MOD_OPR_T:
			return TOSTRING(MOD_OPR_T_FUNC);
			break;
		case AND_OPR_T:
			return TOSTRING(AND_OPR_T_FUNC);
			break;
		case OR_OPR_T:
			return TOSTRING(OR_OPR_T_FUNC);
			break;
		case XOR_OPR_T:
			return TOSTRING(XOR_OPR_T_FUNC);
			break;
		case CGREATER_OPR_T:
			return TOSTRING(CGREATER_OPR_T_FUNC);
			break;
		case CLESS_OPR_T:
			return TOSTRING(CLESS_OPR_T_FUNC);
			break;
		case ASSIGNMENT_OPR_T:
			return TOSTRING(ASSIGNMENT_OPR_T_FUNC);
			break;
		case INC_OPR_T:
			return TOSTRING(INC_OPR_T_FUNC);
			break;
		case DEC_OPR_T:
			return TOSTRING(DEC_OPR_T_FUNC);
			break;
		case MUL_ASSIGN_OPR_T:
			return TOSTRING(MUL_ASSIGN_OPR_T_FUNC);
			break;
		case DIV_ASSIGN_OPR_T:
			return TOSTRING(DIV_ASSIGN_OPR_T_FUNC);
			break;
		case MOD_ASSIGN_OPR_T:
			return TOSTRING(MOD_ASSIGN_OPR_T_FUNC);
			break;
		case AND_ASSIGN_OPR_T:
			return TOSTRING(AND_ASSIGN_OPR_T_FUNC);
			break;
		case OR_ASSIGN_OPR_T:
			return TOSTRING(OR_ASSIGN_OPR_T_FUNC);
			break;
		case XOR_ASSIGN_OPR_T:
			return TOSTRING(XOR_ASSIGN_OPR_T_FUNC);
			break;
		case CGREATER_EQU_OPR_T:
			return TOSTRING(CGREATER_EQU_OPR_T_FUNC);
			break;
		case CLESS_EQU_OPR_T:
			return TOSTRING(CLESS_EQU_OPR_T_FUNC);
			break;
		case CEQU_OPR_T:
			return TOSTRING(CEQU_OPR_T_FUNC);
			break;
		case LAND_OPR_T:
			return TOSTRING(LAND_OPR_T_FUNC);
			break;
		case LOR_OPR_T:
			return TOSTRING(LOR_OPR_T_FUNC);
			break;
		case SHR_OPR_T:
			return TOSTRING(SHR_OPR_T_FUNC);
			break;
		case SHL_OPR_T:
			return TOSTRING(SHL_OPR_T_FUNC);
			break;
		case SHR_ASSIGN_OPR_T:
			return TOSTRING(SHR_ASSIGN_OPR_T_FUNC);
			break;
		case SHL_ASSIGN_OPR_T:
			return TOSTRING(SHL_ASSIGN_OPR_T_FUNC);
			break;
		default:
			return NULL;
	}
}

/* return false if the token is not a value (wether it's a constant or not).
 * Otherwise, append the generated output to w_area and return the first character of the generated type 
 * Note: The function wouldn't lookup to check it's existence for us, and it also wouldn't go to the next token on VARIABLE_T*/
char expr_gen_value(dstr_t* w_area) {
	token_t token = lex_peek_token();
	switch (token.tag) {
		case VARIABLE_T:
			dstr_append(w_area, token.value);
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

	char l_type = '\0';
	token_t l_tkn;
	var_data_t* l_var_data;

	char r_type[100]; /* contains types of rvalues */
	int rt_counter = 0;
	tag_t o_tag2; /* operator tag2 */

	l_tkn = lex_peek_token();
	if(l_tkn.tag2 == LEFT_PAREN_T) {
		lex_next_token();
		if(!(l_type = expr_gen(&buff, 0))) {
			return false;
		}
		if(lex_peek_token().tag2 != RIGHT_PAREN_T) {
			return false;
		}
		lex_next_token();
	} else {
	       	if (!(l_type = expr_gen_value(&buff))) {
			return false;
		}

		/* check if the variable exist on ht and store the result to l_var_data, else, error 
		 * (the expr_gen_value() wouldn't check if a name exists or not*/
		if (l_type == 'v') {
			if ((l_var_data = get_var(l_tkn.value)) == NULL) {
				char msg[17+MAX_TOKEN_VALUE];
				sprintf(msg, "undeclared name %s", l_tkn.value);
				exit_error(msg);
			}
			lex_next_token();
		}
	}

	while (lex_peek_token().tag == OPERATOR_T && expr_bp_of(lex_peek_token().tag2) > rbp) {
		/* insert ',' and rvalue */
		if(lex_peek_token().tag2 == LEFT_PAREN_T) {
			lex_next_token();
			if(!expr_gen(&buff, 0)) {
				return false;
			}
			if(lex_peek_token().tag2 != RIGHT_PAREN_T) {
				return false;
			}
			lex_next_token();
		}

		o_tag2 = lex_next_token().tag2;

		/* check if l_type is const and the operator is one of the assignment operators */
		if (l_type == 'v' && l_var_data->is_const || l_type != 'v') {
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

	bool is_const = false;
	/* parse the code */
	if (lex_next_token().tag == CONST_T) {
		is_const = true;	
	}

	token_t name_tkn = lex_peek_token();
	if (name_tkn.tag != VARIABLE_T) {
		return false;
	}
	/* don't jump to the next token because we're going to parse this with expr_gen() */

	/* check if the item exists on var_ht */
	if (get_var(name_tkn.value) != NULL) {
		char msg[17+MAX_TOKEN_VALUE];
		sprintf(msg, "redeclaration of variable %s", lex_peek_token().value);
		exit_error(msg);
	}
	
	/* if it doesn't exist, insert to the var_ht */
	var_data_t* var_val;
	var_val = malloc(sizeof(var_data_t));
	/* set to false because we will initialise it */
	var_val->is_const = false;

	ht_set(var_ht[scope], name_tkn.value, var_val);

	/* generate the code based from the parsed data */
	dstr_append(w_area, TOSTRING(E_VARIABLE_STRUCT) " ");
	dstr_append(w_area, name_tkn.value);
	
	/* if there's no init */
	if(lex_jump_peek_token().tag2 != ASSIGNMENT_OPR_T) {
		dstr_append(w_area, ";\n");
		dstr_append(w_area, TOSTRING(NULLIFY_FUNC) "(");
		dstr_append(w_area, name_tkn.value);
		dstr_append(w_area, ")");
		var_val->is_const = is_const;
		lex_next_token();
		return true;
	}

	dstr_append(w_area, ";\n");
	
	/* generate the expression */
	if(!expr_gen(w_area, 0)){
		return false;
	}

	/* actually assign the is_const to the var_val */
	var_val->is_const = is_const;

	return true;
}

/* return false if not matches the grammar of while expression.
 * Otherwise, returns true */
bool while_gen(dstr_t* w_area) {
	if (lex_peek_token().tag != WHILE_T) {
		return false;
	}
	lex_next_token();

	dstr_append(w_area, TOSTRING(WHILE_KEYWORD) "(" TOSTRING(E_IS_TRUE) "(");

	/* generate the while expression */
	if(!expr_gen(w_area, 0)) {
		return false;
	}

	dstr_append(w_area, "))");

	/* generate while body */
	if (lex_peek_token().tag2 != LEFT_CURLY_BRACKET_T) {
		return false;
	}
	lex_next_token();

	dstr_append(w_area, "{\n");
	
	/* create a new variable scope */
	scope++;
	if((var_ht[scope] = ht_create()) == NULL) {
		printf("FATAL ERROR: failed to initialize ht");
		exit(1);
	}

	do {
		/* try to match all the grammar */
		if (!match_local_grammar(w_area)) {
			return false;
		}
		
		if (lex_peek_token().tag == SEMICOLON_T) {
			lex_next_token();
		}
		dstr_append(w_area, ";\n");
	} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
	lex_next_token();

	/* delete the previously created scope */
	ht_destroy(var_ht[scope]);
	scope--;

	dstr_append(w_area, "}");
	
	return true;
}

/* return false if not matches the grammar of if, elif, and else expression.
 * Otherwise, returns true */
bool if_gen(dstr_t* w_area) {
	if (lex_peek_token().tag != IF_T) {
		return false;
	}
	lex_next_token();

	dstr_append(w_area, TOSTRING(IF_KEYWORD) "(" TOSTRING(E_IS_TRUE) "(");

	/* generate the if expression */
	if(!expr_gen(w_area, 0)) {
		return false;
	}

	dstr_append(w_area, "))");

	/* look for '{'*/
	if (lex_peek_token().tag2 != LEFT_CURLY_BRACKET_T) {
		return false;
	}
	lex_next_token();

	dstr_append(w_area, "{\n");
	
	/* create a new variable scope */
	scope++;
	if((var_ht[scope] = ht_create()) == NULL) {
		printf("FATAL ERROR: failed to initialize ht");
		exit(1);
	}

	/* generate body */
	do {
		/* try to match all the grammar */
		if (!match_local_grammar(w_area)) {
			return false;
		}
		
		if (lex_peek_token().tag == SEMICOLON_T) {
			lex_next_token();
		}
		dstr_append(w_area, ";\n");
	} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
	lex_next_token();

	/* delete the previously created scope */
	ht_destroy(var_ht[scope]);
	scope--;

	dstr_append(w_area, "}");

	/* check if the next token is elif or else */
	while (lex_peek_token().tag == ELIF_T) {
		lex_next_token();

		dstr_append(w_area, TOSTRING(ELIF_KEYWORD) "(" TOSTRING(E_IS_TRUE) "(");

		/* generate the if expression */
		if(!expr_gen(w_area, 0)) {
			return false;
		}

		dstr_append(w_area, ")){\n");

		/* look for '{'*/
		if (lex_peek_token().tag2 != LEFT_CURLY_BRACKET_T) {
			return false;
		}
		lex_next_token();
		
		/* create a new variable scope */
		scope++;
		if((var_ht[scope] = ht_create()) == NULL) {
			printf("FATAL ERROR: failed to initialize ht");
			exit(1);
		}

		/* generate body */
		do {
			/* try to match all the grammar */
			if (!match_local_grammar(w_area)) {
				return false;
			}
			
			if (lex_peek_token().tag == SEMICOLON_T) {
				lex_next_token();
			}
			dstr_append(w_area, ";\n");
		} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
		lex_next_token();

		/* delete the previously created scope */
		ht_destroy(var_ht[scope]);
		scope--;
		dstr_append(w_area, "}");
	}

	if (lex_peek_token().tag == ELSE_T) {
		lex_next_token();

		/* look for '{'*/
		if (lex_peek_token().tag2 != LEFT_CURLY_BRACKET_T) {
			return false;
		}
		lex_next_token();

		dstr_append(w_area, TOSTRING(ELSE_KEYWORD) "{\n");
		
		/* create a new variable scope */
		scope++;
		if((var_ht[scope] = ht_create()) == NULL) {
			printf("FATAL ERROR: failed to initialize ht");
			exit(1);
		}

		/* generate body */
		do {
			/* try to match all the grammar */
			if (!match_local_grammar(w_area)) {
				return false;
			}
			
			if (lex_peek_token().tag == SEMICOLON_T) {
				lex_next_token();
			}
			dstr_append(w_area, ";\n");
		} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
		lex_next_token();

		/* delete the previously created scope */
		ht_destroy(var_ht[scope]);
		scope--;
		
		dstr_append(w_area, "}");
	}
	
	return true;
}

/* return false if not matches the grammar of function declaration.
 * Otherwise, returns true */
bool funcdecl_gen(dstr_t* w_area) {
	if (lex_peek_token().tag != FN_T) {
		return false;
	}
	lex_next_token();

	/* parse the name */
	token_t name_tkn = lex_peek_token();
	if (name_tkn.tag != VARIABLE_T) {
		return false;
	}
	lex_next_token();

	dstr_append(w_area, TOSTRING(E_VARIABLE_STRUCT) " ");
	dstr_append(w_area, name_tkn.value);	

	if (lex_peek_token().tag2 != LEFT_PAREN_T) {
		return false;
	}

	dstr_append_char(w_area, '(');
	
	/* create a new variable scope, for the params and local variables */
	scope++;
	if((var_ht[scope] = ht_create()) == NULL) {
		printf("FATAL ERROR: failed to initialize ht");
		exit(1);
	}

	func_data_t* func_data;
	func_data = malloc(sizeof(func_data_t));
	func_data->param_min = 0;
	func_data->param_max = 0;
	
	int default_param_counter = 0;

	/* parse the parameters */
	while (lex_peek_token().tag2 != RIGHT_PAREN_T) {
		lex_next_token();

		if (lex_peek_token().tag2 != DECL_T) {
			return false;
		}
	
		bool is_const = false;
		/* parse the code */
		if (lex_next_token().tag == CONST_T) {
			is_const = true;	
		}
	
		token_t name_tkn = lex_peek_token();
		if (name_tkn.tag != VARIABLE_T) {
			return false;
		}
		/* don't jump to the next token because we're going to parse this with expr_gen() */
	
		/* check if the item exists on var_ht */
		if (get_var(name_tkn.value) != NULL) {
			char msg[17+MAX_TOKEN_VALUE];
			sprintf(msg, "redeclaration of variable %s", lex_peek_token().value);
			exit_error(msg);
		}
		
		/* if it doesn't exist, insert to the var_ht */
		var_data_t* var_val;
		var_val = malloc(sizeof(var_data_t));
		/* set to false because we will initialise it */
		var_val->is_const = false;
	
		ht_set(var_ht[scope], name_tkn.value, var_val);
	
		/* generate the code based from the parsed data */
		dstr_append(w_area, TOSTRING(E_VARIABLE_STRUCT) " ");
		dstr_append(w_area, name_tkn.value);
		
		/* if there's no init */
		if(lex_jump_peek_token().tag2 != ASSIGNMENT_OPR_T) {
			if (default_param_counter > 0) {
				exit_error("parameter with no default value after a list of parameter with default value");
			}
			var_val->is_const = is_const;
			lex_next_token();
			func_data->param_min++;
		} else {
			lex_next_token();
			lex_next_token();

			/* create a new dstr for the expression */
			dstr_init(&func_data->default_param[default_param_counter]);

			/* generate the expression */
			if(!expr_gen(&func_data->default_param[default_param_counter], 0)){
				return false;
			}

			/* terminate it */
			dstr_append_char(&func_data->default_param[default_param_counter], '\0');

			/* actually assign the is_const to the var_val */
			var_val->is_const = is_const;
			
			//printf("param_default: %s\n", func_data->default_param[param_counter].str);
			default_param_counter++;
		}
		func_data->param_max++;

		if (lex_peek_token().tag2 == COMMA_T) {
			dstr_append_char(w_area, ',');
			if (lex_jump_peek_token().tag2 == RIGHT_PAREN_T) {
				return false;
			}
		}
	}
	
	/* add the func_data to the func_ht */
	ht_set(func_ht, name_tkn.value, func_data);

	dstr_append(w_area, ") {\n");	

	lex_next_token();
	if (lex_peek_token().tag2 != LEFT_CURLY_BRACKET_T) {
		return false;
	}

	lex_next_token();

	/* parse body */

	do {
		/* try to match all the grammar */
		if (!match_local_grammar(w_area)) {
			return false;
		}
		
		if (lex_peek_token().tag == SEMICOLON_T) {
			lex_next_token();
		}
		dstr_append(w_area, ";\n");
	} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
	lex_next_token();

	/* delete the previously created scope */
	ht_destroy(var_ht[scope]);
	scope--;

	dstr_append(w_area, "}");

	return true;
}

/* main entry, return a char pointer to the generated C code
 * (!) NOTE: return value must be freed after use */
char* generate_code() {
	dstr_t w_area;
	dstr_init(&w_area);

	func_ht = ht_create();
	
	if((var_ht[scope] = ht_create()) == NULL) {
		printf("FATAL ERROR: failed to initialize ht");
		exit(1);
	}

	do {
		/* try to match all the grammar */
		if (!match_global_grammar(&w_area)) {
			char msg[18+MAX_TOKEN_VALUE];
			sprintf(msg, "unexpected token: %s", lex_peek_token().value);
			exit_error(msg);
		}

		if (lex_peek_token().tag == SEMICOLON_T) {
			lex_next_token();
		}
		dstr_append(&w_area, ";\n");
	} while(lex_peek_token().tag != EOF_T);

	dstr_append_char(&w_area, '\0');

	func_data_t* test;
	test = (func_data_t*)ht_get(func_ht, "hello");
	printf("def_params: %s\n\n", test->default_param[1].str);
	ht_destroy(var_ht[scope]);
	ht_destroy(func_ht);

	return w_area.str;
}

/* match all the grammar in global scope */
bool match_global_grammar(dstr_t* w_area) {
	if (
	    !expr_gen(w_area, 0) &&
	    !vardecl_gen(w_area) &&
	    !while_gen(w_area)   &&
	    !if_gen(w_area)      &&
	    !funcdecl_gen(w_area)
	    ) {
		return false;
	}
	return true;
}

/* match grammars in-function scope */
bool match_local_grammar(dstr_t* w_area) {
	if (
	    !expr_gen(w_area, 0) &&
	    !vardecl_gen(w_area) &&
	    !while_gen(w_area)   &&
	    !if_gen(w_area)
	    ) {
		return false;
	}
	return true;
}
