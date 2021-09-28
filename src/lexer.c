#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "include/defs.h"
#include "include/types.h"
#include "include/compiler.h"
#include "include/utils.h"

/* the following function returns true if the argument 'ch' given is one of the first operator combination 
 * (if the operator is += then + is the first operator combination) and returns false otherwise */
bool is_operator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || 
	    ch == '=' || ch == '!' || ch == '>' || ch == '<' || 
	    ch == '&' || ch == '|' || ch == '^') {
		return true;
	} else {
		return false;
	}
}

/* the following function returns true if 'ch' is one of the allowed first characters of identifier */
bool is_identifier(char ch) {
	if (isalpha(ch) || ch == '_') {
		return true;
	} else {
		return false;
	}
}

/* the following function returns true if 'ch' is one of valid elly symbols ('(' ')' '{' '}' '[' ']' ',') */
bool is_symbol(char ch) {
	if (ch == '(' || ch == ')' ||
	    ch == '{' || ch == '}' ||
	    ch == '[' || ch == ']' ||
	    ch == ',') {
		return true;
	} else {
		return false;
	}
}

/* the following function returns the tag of a keyword if the given char* argument is a keyword, else it returns VARIABLE_T */
tag_t get_keyword_tag(char *str) {
	if (strcmp(str, "for") == 0) {
		return FOR_T;
	} else if (strcmp(str, "while") == 0) {
		return WHILE_T;
	} else if (strcmp(str, "if") == 0) {
		return IF_T;
	} else if (strcmp(str, "else") == 0) {
		return ELSE_T;
	} else if (strcmp(str, "elif") == 0) {
		return ELIF_T;
	} else if (strcmp(str, "fn") == 0) {
		return FN_T;
	} else if (strcmp(str, "return") == 0) {
		return RETURN_T;
	} else if (strcmp(str, "import") == 0) {
		return IMPORT_T;	
	} else if (strcmp(str, "break") == 0) {
		return BREAK_T;
	} else if (strcmp(str, "continue") == 0) {
		return CONTINUE_T;
	} else {
		return VARIABLE_T;
	}
}


/* get current character of the file without moving the fseek */
char PEEK() {
	char ch;
	ch = fgetc(SOURCE);
	ungetc(ch, SOURCE);
	return ch;
}

/* get next character of the file and move the fseek */
char NEXT() {
	char ch;
	ch = fgetc(SOURCE);
	COLUMN++;
	CHAR++;
	if (ch == '\n') {
		LINE++;
		COLUMN = 1;
	}
	return PEEK();
}

/* generate current token and move fseek*/
token_t lex_next_token() {
	signed char ch = PEEK();
	char str[32];
	int str_counter = 0;
	tag_t tag;
	tag_t tag2;

	/* skip whitespaces and comments*/
	while (ch == '#') {
		while (ch != '\n') {
			ch = NEXT();
		}
       		while (ch == ' ' || ch == '\t' || ch == '\n') {
			ch = NEXT();
		}
	} 
       	while (ch == ' ' || ch == '\t' || ch == '\n') {
		ch = NEXT();
	}

	if (ch == EOF) {
		tag = EOF_T;
	} else if (ch == '\n') {
		ch = NEXT();
		tag = EOL_T;
	} else if (is_identifier(ch)) { /* check if it's an identifier */
		while (isalnum(ch) || ch == '_' || ch == '-') {	
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
		}
		str[str_counter] = '\0';	
		tag = get_keyword_tag(str); /* analyze if it's a keyword */
	} else if (isdigit(ch)) { /*if it's a number*/
		bool is_float = false;
		while (isdigit(ch)) {	
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
			if(ch == '.' && is_float == false) {/* check if its a float */
				str[str_counter] = ch;
				str_counter++;
				ch = NEXT();
				is_float = true;
			}
		}	
		if(is_float) {
			tag = FLOAT_T;	
		} else {
			tag = INT_T;
		}
	} else if (ch == '\'' || ch == '\"') { /* if it's a string */
		char str_symbol = ch; /* symbol that were used to make the string */
		ch = NEXT();
		while (ch != str_symbol) {
			str[str_counter] = ch;
			str_counter++;
			ch = NEXT();
			if(ch == '\n') {
				print_error("Unexpected end of line while scanning string literal");
			}
		}
		ch = NEXT();
		tag = STRING_T;
	} else if (is_operator(ch)) { /* if it's one of the operators */
		switch (ch) { /* generate tag2 */
			case '+':
				tag2 = ADD_OPR_T;
				break;
			case '-':
				tag2 = SUB_OPR_T;
				break;
			case '*':
				tag2 = MUL_OPR_T;
				break;
			case '/':
				tag2 = DIV_OPR_T;
				break;
			case '|':
				tag2 = OR_OPR_T;
				break;
			case '&':
				tag2 = AND_OPR_T;
				break;
			case '^':
				tag2 = XOR_OPR_T;	
				break;
			case '<':
				tag2 = CLESS_OPR_T;
				break;
			case '>':
				tag2 = CGREATER_OPR_T;
				break;
			case '%':
				tag2 = MOD_OPR_T;	
				break;
			case '=':
				tag2 = ASSIGNMENT_OPR_T;	
				break;
		}
		str[str_counter] = ch;
		str_counter++;
		char head_operator = ch; /* to be checked with the trailing operator */
		ch = NEXT();
		if (is_operator(ch)) {
			if (ch == '=') {  /* allow = after all operator */
				switch (ch) { /* generate tag2 */
					case '+':
						tag2 = INC_OPR_T;
						break;
					case '-':
						tag2 = DEC_OPR_T;
						break;
					case '*':
						tag2 = MUL_ASSIGN_OPR_T;
						break;
					case '/':
						tag2 = DIV_ASSIGN_OPR_T;
						break;
					case '|':
						tag2 = OR_ASSIGN_OPR_T;
						break;
					case '&':
						tag2 = AND_ASSIGN_OPR_T;
						break;
					case '^':
						tag2 = XOR_ASSIGN_OPR_T;	
						break;
					case '<':
						tag2 = CLESS_EQU_OPR_T;
						break;
					case '>':
						tag2 = CGREATER_EQU_OPR_T;
						break;
					case '%':
						tag2 = MOD_ASSIGN_OPR_T;
						break;
					case '=':
						tag2 = CEQU_OPR_T;	
						break;
				}
				str[str_counter] = ch;
				str_counter++;
				ch = NEXT();
			} else if (ch == '&' && head_operator == '&' ) { /* allow & after & (&&)*/
				tag2 = LAND_OPR_T;
				str[str_counter] = ch;
				str_counter++;
				ch = NEXT();
			} else if (ch == '|' && head_operator == '|' ) { /* allow | after | (||)*/
				tag2 = LOR_OPR_T;
				str[str_counter] = ch;
				str_counter++;
				ch = NEXT();
			} else if (ch == '>' && head_operator == '>' || /* allow > after > (>>) */
			           ch == '<' && head_operator == '<' ){ /* allow < after < (<<) */	
				if (head_operator == '>') {
					tag2 = SHR_OPR_T;
				} else if (head_operator == '<') {
					tag2 = SHL_OPR_T;
				}
				str[str_counter] = ch;
				str_counter++;
				ch = NEXT();
				if (ch == '=') {
					if (head_operator == '>') {
						tag2 = SHR_ASSIGN_OPR_T;
					} else if (head_operator == '<') {
						tag2 = SHL_ASSIGN_OPR_T;
					}
					str[str_counter] = ch;
					str_counter++;
					ch = NEXT();
				}
			}
		}
		tag = OPERATOR_T;
	} else if (is_symbol(ch)) { /* if it's one of the symbols */
		switch(ch) {
			case '(':
				tag2 = LEFT_PAREN_T;
				break;
			case ')':
				tag2 = RIGHT_PAREN_T;
				break;
			case '{':
				tag2 = LEFT_SEMICOLON_T;
				break;
			case '}':
				tag2 = RIGHT_SEMICOLON_T;
				break;
			case '[':
				tag2 = LEFT_BLOCK_T;
				break;
			case ']':
				tag2 = RIGHT_BLOCK_T;
				break;
			case ',':
				tag2 = COMMA_T;	
				break;
		}
	 	str[str_counter] = ch;
		ch = NEXT();
  		str_counter++;
		tag = SYMBOL_T;	
	} else if (ch == ';') {
	 	str[str_counter] = ch;
		ch = NEXT();
  		str_counter++;
		tag = SEMICOLON_T;
	} else { /*else, error*/
		print_error("Invalid token");
	}

	/* add an escape character after the string */	
	str[str_counter] = '\0';

	/* skip whitespaces and comments*/
	while (ch == '#') {
		while (ch != '\n') {
			ch = NEXT();
		}
       		while (ch == ' ' || ch == '\t' || ch == '\n') {
			ch = NEXT();
		}
	} 
       	while (ch == ' ' || ch == '\t' || ch == '\n') {
		ch = NEXT();
	}

	return t_init_token(str, tag, tag2);
}	

/* generate current token but don't move the fseek */
token_t lex_peek_token() {
	/* store all the compiler variables */
	int curr_char = CHAR;
	int curr_col = COLUMN;
	int curr_line = LINE;

	/* get next token */
	token_t token = lex_next_token();

	/* fseek back */
	fseek(SOURCE, curr_char-1, SEEK_SET);

	/* restore the compiler variables */
	CHAR = curr_char;
	COLUMN = curr_col;
	LINE = curr_line;

	return token;
}
/* generate next token and don't move the fseek */
token_t lex_jump_peek_token() {
	/* store all the compiler variables */
	int curr_char = CHAR;
	int curr_col = COLUMN;
	int curr_line = LINE;

	/* get next of next token */
	lex_next_token();	
	token_t token = lex_next_token();

	/* fseek back */
	fseek(SOURCE, curr_char-1, SEEK_SET);

	/* restore the compiler variables */
	CHAR = curr_char;
	COLUMN = curr_col;
	LINE = curr_line;

	return token;
}
