/* The file will output a pointer to the generated C code by the generate_code() function.
 * It parses using recursive descent parser, analyze and generate the source code in one pass;
 *
 * It aims to be fast and simple, it doesn't even have an AST, rather the code are generated directly everytime a token is parsed. */

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
#include "include/utils.h"
#include "include/darr.h"
#include "backend/c_backend.h"
#include "ht/ht.h"

#define OUTPUT_SIZE 16777216 /* 8^8 */

/* TODO: make this dynamic */
ht* var_ht[VAR_HT_STACK_INIT_SIZE];
int scope = 0;

ht* func_ht;

bool match_global_grammar(dstr_t* w_area_main, dstr_t* w_area_glob);
bool match_local_grammar(dstr_t* w_area);
bool match_global_definition();
char expr_gen(dstr_t* w_area, int rbp);

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

/* free var_ht items and key of the current scope */
void var_destroy_scope() {
	hti var_ht_iter;
	var_ht_iter = ht_iterator(var_ht[scope]);
	while (ht_next(&var_ht_iter)) {
		free(var_ht_iter.value);
	}
	ht_destroy(var_ht[scope]);
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

/* return false if not matches the grammar of function declaration.
 * Otherwise, returns true */
bool func_decl_gen(dstr_t* w_area) {
	if (lex_peek_token().tag != FN_T) {
	        return false;
	}
	lex_next_token();
	
	func_data_t* func_data; /* to be appended to func_ht */
	func_data = malloc(sizeof(func_data_t));
	func_data->param_min = 0;
	func_data->param_max = 0;
	
	/* parse the name */
	token_t name_tkn = lex_peek_token();
	if (name_tkn.tag != VARIABLE_T) {
	        return false;
	}
	strcpy(func_data->name, name_tkn.value);
	
	lex_next_token();
	
	dstr_append(w_area, TOSTRING(VARIABLE_STRUCT) " ");
	dstr_append(w_area, name_tkn.value);    
	
	if (lex_peek_token().tag2 != LEFT_PAREN_T) {
	        return false;
	}
	
	dstr_append_char(w_area, '(');
	
	lex_next_token();

	/* create a new variable scope, for the params and local variables */
	scope++;
	if((var_ht[scope] = ht_create()) == NULL) {
	        printf("FATAL ERROR: failed to initialize ht");
	        exit(1);
	}

	/* parse the parameters */
	int default_param_counter = 0;
	while (lex_peek_token().tag2 != RIGHT_PAREN_T) {
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
	        /* set to false because we will initialize it */
	        var_val->is_const = false;
	
	        ht_set(var_ht[scope], name_tkn.value, var_val);
	
	        /* generate the code based from the parsed data */
	        dstr_append(w_area, TOSTRING(VARIABLE_STRUCT) " ");
	        dstr_append(w_area, name_tkn.value);
	        
	        /* if there's no init */
	        if(lex_jump_peek_token().tag2 != ASSIGNMENT_OPR_T) {
	                if (default_param_counter > 0) {
	                        exit_error("parameter with no default value after a parameter with default value");
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
	
	        //printf("lex_peek_token(): %s\n", lex_peek_token().value);
	        if (lex_peek_token().tag2 == COMMA_T) {
	                dstr_append_char(w_area, ',');
	                lex_next_token();
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
	} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
	lex_next_token();
	
	/* delete the previously created scope */
	var_destroy_scope(var_ht[scope]);
	scope--;
	
	dstr_append(w_area, "}");
	
	return true;
}

/* return FALSE if the grammar is not matched, else, return true */
bool func_call_gen(dstr_t* w_area) {
	token_t name_tkn = lex_peek_token();
	if (name_tkn.tag != VARIABLE_T) {
		return false;
	}
	dstr_append(w_area, name_tkn.value);

	func_data_t* func_data;
	if (!(func_data = (func_data_t*)ht_get(func_ht, name_tkn.value))) {
		char msg[17+MAX_TOKEN_VALUE];
		sprintf(msg, "undeclared name %s", name_tkn.value);
		exit_error(msg);
	}

	lex_next_token();
	
	if(lex_peek_token().tag2 != LEFT_PAREN_T) {
		return false;
	}
	dstr_append_char(w_area, '(');

	int arg_counter = 0;
	while (lex_peek_token().tag2 != RIGHT_PAREN_T) {
		lex_next_token();

		/* generate the arg expression */
		if(!expr_gen(w_area, 0)){
			return false;
		}

		if (lex_peek_token().tag2 == COMMA_T) {
			dstr_append_char(w_area, ',');
			if (lex_jump_peek_token().tag2 == RIGHT_PAREN_T) {
				return false;
			}
			arg_counter++;
		}
	}
	lex_next_token();
	dstr_append_char(w_area, ')');

	return true;
}

/* return false if the token is not a value (wether it's a constant or not).
 * Otherwise, append the generated output to w_area and return the first character of the generated type 
 * Note: The function wouldn't lookup to check it's existence for us, and it also wouldn't go to the next token on VARIABLE_T*/
char expr_gen_value(dstr_t* w_area) {
	token_t token = lex_peek_token();
	switch (token.tag) {
		case VARIABLE_T:
			/* check if it's a function call */
			if (lex_jump_peek_token().tag2 == LEFT_PAREN_T) {
				if (!func_call_gen(w_area)) {
					return false;
				}
				return 'f';
			} else {
				dstr_append(w_area, token.value);
			}
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
			free(buff.str);
			return false;
		}
		if(lex_peek_token().tag2 != RIGHT_PAREN_T) {
			free(buff.str);
			return false;
		}
		lex_next_token();
	} else {
	       	if (!(l_type = expr_gen_value(&buff))) {
			free(buff.str);
			return false;
		}

		/* check if the variable exist on ht and store the result to l_var_data, else, error 
		 * (the expr_gen_value() wouldn't check if a name exists or not*/
		if (l_type == 'v') {
			if ((l_var_data = get_var(l_tkn.value)) == NULL) {
				free(buff.str);
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
				free(buff.str);
				return false;
			}
			if(lex_peek_token().tag2 != RIGHT_PAREN_T) {
				free(buff.str);
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
					free(buff.str);
					exit_error("trying to assign to a constant");
					break;
			}
		}

		dstr_append(&buff, ", ");
		if (!expr_is_r_assoc(o_tag2)) {
			if(!(r_type[rt_counter] = expr_gen(&buff, expr_bp_of(o_tag2)))) {
				free(buff.str);
				return false;	
			}
		} else {
			if(!(r_type[rt_counter] = expr_gen(&buff, expr_bp_of(o_tag2)-1))) {
				free(buff.str);
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
	
	free(buff.str);

	return l_type;
}

/* return false if not matches the grammar of variable declaration.
 * Otherwise, returns true */
bool var_decl_gen(dstr_t* w_area) {
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

	if (!ht_set(var_ht[scope], name_tkn.value, var_val)) {
		printf("FATAL: can't allocate space for ht");
		exit(1);
	}

	/* if there's no init */
	if(lex_jump_peek_token().tag2 != ASSIGNMENT_OPR_T) {
		/* actually assign the is_const to the var_val */
		var_val->is_const = is_const;
		lex_next_token();
		c_var_decl(w_area, name_tkn.value, NULL, is_const);
		return true;
	}

	dstr_t expr;
	dstr_init(&expr);
	/* generate the expression */
	if(!expr_gen(&expr, 0)){
		return false;
	}

	c_var_decl(w_area, name_tkn.value, expr.str, is_const);

	/* actually assign the is_const to the var_val */
	var_val->is_const = is_const;

	free(expr.str);

	return true;
}

/* return false if not matches the grammar of while expression.
 * Otherwise, returns true */
bool while_gen(dstr_t* w_area) {
	if (lex_peek_token().tag != WHILE_T) {
		return false;
	}
	lex_next_token();

	dstr_append(w_area, TOSTRING(WHILE_KEYWORD) "(" TOSTRING(IS_TRUE_FUNC) "(");

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
		
	} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
	lex_next_token();

	/* delete the previously created scope */
	var_destroy_scope(var_ht[scope]);
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

	dstr_append(w_area, TOSTRING(IF_KEYWORD) "(" TOSTRING(IS_TRUE_FUNC) "(");

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
	} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
	lex_next_token();

	/* delete the previously created scope */
	var_destroy_scope(var_ht[scope]);
	scope--;

	dstr_append(w_area, "}");

	/* check if the next token is elif or else */
	while (lex_peek_token().tag == ELIF_T) {
		lex_next_token();

		dstr_append(w_area, TOSTRING(ELIF_KEYWORD) "(" TOSTRING(IS_TRUE_FUNC) "(");

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
		} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
		lex_next_token();

		/* delete the previously created scope */
		var_destroy_scope(var_ht[scope]);
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
		} while(lex_peek_token().tag2 != RIGHT_CURLY_BRACKET_T);
		lex_next_token();

		/* delete the previously created scope */
		var_destroy_scope(var_ht[scope]);
		scope--;
		
		dstr_append(w_area, "}");
	}
	
	return true;
}

/* main entry, return a char pointer to the generated C code
 * (!) NOTE: return value must be freed after use */
char* generate_code() {
	dstr_t w_area_main;
	dstr_init(&w_area_main);

	dstr_t w_area_glob;
	dstr_init(&w_area_glob);

	/* final output area */
	dstr_t w_area;
	dstr_init(&w_area);

	func_ht = ht_create();

	/* match code */
	if((var_ht[scope] = ht_create()) == NULL) {
		printf("FATAL ERROR: failed to initialize ht");
		exit(1);
	}

	do {
		/* try to match all the grammar */
		if (!match_global_grammar(&w_area_main, &w_area_glob)) {
			char msg[18+MAX_TOKEN_VALUE];
			sprintf(msg, "unexpected token: %s", lex_peek_token().value);
			exit_error(msg);
		}
	} while(lex_peek_token().tag != EOF_T);

	dstr_append_char(&w_area_main, '\0');
	dstr_append_char(&w_area_glob, '\0');

	/* destroy the global variable scope */
	var_destroy_scope();

	/* generate the header of each func_ht items to w_area_glob
         * and free each of it by iteration */
	hti func_ht_iter;
	func_ht_iter = ht_iterator(func_ht);
	func_data_t* func_data;
	while (ht_next(&func_ht_iter)) {
		func_data = (func_data_t*)func_ht_iter.value;

 		/* generate header */
                dstr_append(&w_area, TOSTRING(VARIABLE_STRUCT) " ");
                dstr_append(&w_area, func_data->name);
 
                /* generate the header params */
                dstr_append_char(&w_area, '(');
                for (int i = 0; i < func_data->param_max; i++) {
                        dstr_append(&w_area, TOSTRING(VARIABLE_STRUCT) " param");
                        char i_str[3];
                        sprintf(i_str, "%i", i);
                        dstr_append(&w_area, i_str);
                        if (i+1 < func_data->param_max)
                                dstr_append_char(&w_area, ',');
                }
                dstr_append(&w_area, ");\n");
 
		/* free the func data */
		for (int i = 0; i < (func_data->param_max - func_data->param_min); i++) {
			free(func_data->default_param[i].str);
		}
		free(func_data);
	}
	ht_destroy(func_ht);
	
	dstr_append_char(&w_area, '\n');
	
	/* append the generated output to w_area neatly */
	dstr_append(&w_area, w_area_glob.str);
	/* the main function header */
	dstr_append(&w_area, "\n" TOSTRING(C_MAIN_FUNC_TYPE) " " TOSTRING(C_MAIN_FUNC_NAME) TOSTRING(C_MAIN_FUNC_PARAMS) " {\n");
	dstr_append(&w_area, TOSTRING(PROGRAM_INIT_FUNC) TOSTRING(C_PROGRAM_INIT_PARAMS) ";\n"); 
	dstr_append(&w_area, w_area_main.str);
	dstr_append(&w_area, "};\n");

	dstr_append_char(&w_area, '\0');

	free(w_area_main.str);
	free(w_area_glob.str);

	return w_area.str;
}

/* match all the grammar in global scope */
bool match_global_grammar(dstr_t* w_area_main, dstr_t* w_area_glob) {
	if (
	    !expr_gen(w_area_main, 0) &&
	    !var_decl_gen(w_area_main) &&
	    !while_gen(w_area_main)   &&
	    !if_gen(w_area_main)) {
	    	if (!func_decl_gen(w_area_glob)) {
			return false;
	    	} else {
			dstr_append(w_area_glob, ";\n");
		}
	} else {
		dstr_append(w_area_main, "\n");
	}
	if (lex_peek_token().tag == SEMICOLON_T) {
		lex_next_token();
	}
	return true;
}

/* match grammars in-function scope and append ';' at the end of the generated code */
bool match_local_grammar(dstr_t* w_area) {
	if (
	    !expr_gen(w_area, 0) &&
	    !var_decl_gen(w_area) &&
	    !while_gen(w_area)   &&
	    !if_gen(w_area)
	    ) {
		return false;
	}
	if (lex_peek_token().tag == SEMICOLON_T) {
		lex_next_token();
	}
	dstr_append(w_area, "\n");
	return true;
}
