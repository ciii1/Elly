#ifndef DSTR_H_
#define DSTR_H_

/* free with free() */
typedef struct {
	char* str;
	char* end;
	char* alloc_end;
} dstr_t;

extern bool is_operator(char ch);
extern bool is_identifier(char ch);
extern void dstr_init(dstr_t* dstr); 
extern void dstr_append(dstr_t* dest, char* str);
extern void dstr_append_char(dstr_t* dest, char ch);

#endif
