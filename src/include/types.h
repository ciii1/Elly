typedef short tag_t;

typedef struct {	
	char value[MAX_TOKEN_VALUE];
	tag_t tag;
	tag_t tag2; /* tag2, more specific tag (eg. + -> the tag is OPERATOR_T, and the tag2 is ADD_OPR_T */
} token_t;

typedef struct {
	char* str;
	char* end;
} dstr_t;
