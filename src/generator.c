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
#include "include/utils.h"

#define OUTPUT_SIZE 16777216 /* 8^8 */



/* return the binding power (precedence) of 'val' */
short bp_of(token_t val) {
	switch (val.tag2) {
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
	}
}

/* return the associativity of 'val' */
bool is_r_assoc(token_t val) {
	switch (val.tag2) {
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

/* return false if the token is not a value (wether it's a constant or not).
 * Otherwise, append the generated output to w_area and return true */
bool gen_value(dstr_t* w_area) {
	token_t token = lex_next_token();
	if (token.tag == VARIABLE_T) { /* TODO: check if the variable exists or not with hashtable lookup() */
		return false;
	} else if (token.tag == FLOAT_T || token.tag == INT_T) {
		dstr_append(w_area, token.value);
		return true;
	} else if (token.tag == STRING_T) {
		dstr_append(w_area, "\"");
		dstr_append(w_area, token.value);
		dstr_append(w_area, "\"");
		return true;
	} else {
		return false;
	}
}

/* return false if the token is not an operator (+, -, *, /, |, &, ^, >, <, %, =,  ||, &&, >>, <<, ==, -=, +=, -=, *=, /=, |= &=, <= , >=, >>=, <<=, ^=)
 * or if the operation is invalid (ie. <int> = <int>).
 * Otherwise, append the generated output to w_area
 * Example of generated output = 1.2 + 1 --> __faddi__( ,  "aaa" + 1 --> __saddi__( , var + 1 --> _vaddi_*/
bool gen_operator(dstr_t* w_area, bool is_loperation, token_t lvalue, token_t rvalue) {
	token_t operator = lex_next_token();

	switch (operator.tag2) {
		case ADD_OPR_T:
			dstr_append(w_area, "__add__");
			break;
		case SUB_OPR_T:
			dstr_append(w_area, "__sub__");
			break;
		case MUL_OPR_T:
			dstr_append(w_area, "__mul__");
			break;
		case DIV_OPR_T:
			dstr_append(w_area, "__div__");
			break;
		case MOD_OPR_T:
			dstr_append(w_area, "__mod__");
			break;
		case AND_OPR_T:
			dstr_append(w_area, "__and__");
			break;
		case OR_OPR_T:
			dstr_append(w_area, "__ror__");
			break;
		case XOR_OPR_T:
			dstr_append(w_area, "__xor__");
			break;
		case CGREATER_OPR_T:
			dstr_append(w_area, "__cgr__");
			break;
		case CLESS_OPR_T:
			dstr_append(w_area, "__cls__");
			break;
		case ASSIGNMENT_OPR_T:
			dstr_append(w_area, "__mov__");
			break;
		case INC_OPR_T:
			dstr_append(w_area, "__inc__");
			break;
		case DEC_OPR_T:
			dstr_append(w_area, "__dec__");
			break;
		case MUL_ASSIGN_OPR_T:
			dstr_append(w_area, "__mulm_");
			break;
		case DIV_ASSIGN_OPR_T:
			dstr_append(w_area, "__divm_");
			break;
		case MOD_ASSIGN_OPR_T:
			dstr_append(w_area, "__modm_");
			break;
		case AND_ASSIGN_OPR_T:
			dstr_append(w_area, "__andm_");
			break;
		case OR_ASSIGN_OPR_T:
			dstr_append(w_area, "__rorm_");
			break;
		case XOR_ASSIGN_OPR_T:
			dstr_append(w_area, "__xorm_");
			break;
		case CGREATER_EQU_OPR_T:
			dstr_append(w_area, "__cge__");
			break;
		case CLESS_EQU_OPR_T:
			dstr_append(w_area, "__cle__");
			break;
		case CEQU_OPR_T:
			dstr_append(w_area, "__equ__");
			break;
		case LAND_OPR_T:
			dstr_append(w_area, "__land_");
			break;
		case LOR_OPR_T:
			dstr_append(w_area, "__lor__");
			break;
		case SHR_OPR_T:
			dstr_append(w_area, "__shr__");
			break;
		case SHL_OPR_T:
			dstr_append(w_area, "__shl__");
			break;
		case SHR_ASSIGN_OPR_T:
			dstr_append(w_area, "__shrm_");
			break;
		case SHL_ASSIGN_OPR_T:
			dstr_append(w_area, "__shlm__");
			break;
		default:
			return false;
			break;
	}
}

/* return false if the expression is invalid.
 * Otherwise, append the generated output to w_area
 * example: a+b*c --> __e_add__(a, __e_mul__(b, c)). */
bool gen_operation(dstr_t* w_area) {
	dstr_t buff_area; /* area that is used to write a half finished operation */
	dstr_init(&buff_area);

	dstr_t w_t_area; /* temporary write area */
	dstr_init(&w_t_area);
	
	int bp = 0;
	int r_bp = 0;
	int min_bp = 0;

	int l_parens = 0; /* counter for how much initial function calls part we need to insert before the generated expression */
	int in_l_parens = 0;
	int r_parens = 0; /* counter for how much ')' we need to insert after the generated expression */

	/* generate the first left operator */
	if (!gen_value(&w_t_area)) {
		return false;	
	}

	token_t operator;
	token_t r_operator;
	
	while (lex_peek_token().tag == OPERATOR_T) {
		operator = lex_next_token();
		r_operator = lex_jump_peek_token();

		bp = bp_of(operator);
		r_bp = bp_of(r_operator);
		if (bp <= min_bp || min_bp == 0) {
			/* append w_t_area to buff_area */
			for (int i = 0; i < in_l_parens; i++) {
				dstr_append(&buff_area, "(");
			}
			dstr_append(&buff_area, w_t_area.str);
			for (int i = 0; i < r_parens; i++) {
				dstr_append(&buff_area, ")");
			}
			/* insert the low left assoc*/
			if (min_bp != 0) {
				dstr_append(&buff_area, ")");
				l_parens++;
			}

			min_bp = bp;
			
			/* append the operator */
			dstr_append(&buff_area, operator.value);

			in_l_parens = 0;
			r_parens = 0;

			dstr_erase(&w_t_area);

			/* append the operand */
			gen_value(&w_t_area);
		} else if (bp < r_bp) {
			/* append the operator */
			dstr_append(&w_t_area, operator.value);

			/* up the operator precedence */
			dstr_append(&w_t_area, "(");
			r_parens++;

			/* append the operand */
			gen_value(&w_t_area);
		} else {
			/* append operator and operand */
			dstr_append(&w_t_area, operator.value);
			gen_value(&w_t_area);
			
			/* down the operator precedence */
			for (int i = 0; i <= bp - r_bp && r_parens > 0; i++) {
				dstr_append(&w_t_area, ")");
				r_parens--;
			}

			/* left assoc */
			dstr_append(&w_t_area, ")");
			in_l_parens++;
		}
	}

	/* write buff_area to the real write area*/

	/* insert the low l_parens */
	for (int i = 0; i < l_parens; i++) {
		dstr_append(w_area, "(");
	}

	dstr_append(w_area, buff_area.str);
	
	/* write the lefted w_t_area */
	
	/* insert the in_l_parens */
	for (int i = 0; i < in_l_parens; i++) {
		dstr_append(w_area, "(");
	}

	dstr_append(w_area, w_t_area.str);
	
	for (int i = 0; i < r_parens; i++) {
		dstr_append(w_area, ")");
	}
	
	dstr_free(&w_t_area);
	dstr_free(&buff_area);

	return true;
}

/* main entry, return a char pointer to the generated C code
 * (!) NOTE: return value must be freed after use */
char* generate_code() {
	dstr_t w_area;
	dstr_init(&w_area);

	do {
		/* cut endlines 
		 * TODO: change the delimiter to optional ';', like js, it's easier to handle and to code with*/
		while (lex_peek_token().tag == EOL_T) {
			lex_next_token();
		}

		if (!(gen_operation(&w_area))){/* try to match all the grammar */
			char msg[18+MAX_TOKEN_VALUE];
			sprintf(msg, "unexpected token: %s", lex_peek_token().value);
			print_error(msg);
		}
		
		/* cut endlines */
		while (lex_peek_token().tag == EOL_T) {
			lex_next_token();
		}
	} while(lex_next_token().tag != EOF_T);

	return w_area.str;
}
