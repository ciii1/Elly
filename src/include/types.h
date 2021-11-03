#ifndef TYPES_H_
#define TYPES_H_

#include <stdbool.h>
#include "dstr.h"

typedef short tag_t;

typedef struct {	
	char value[MAX_TOKEN_VALUE];
	tag_t tag;
	tag_t tag2; /* tag2, more specific tag (eg. + -> the tag is OPERATOR_T, and the tag2 is ADD_OPR_T */
} token_t;


typedef struct {
	bool is_const;
} var_data_t;

typedef struct {
	int param_max;
	int param_min;
	dstr_t default_param[FN_PARAMS_MAX]; /* an array contains the generated code of default params, eg. if the input is a = 10+20, will contains add(10,20). */
} func_data_t;

#endif
