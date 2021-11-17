#ifndef DARR_H_
#define DARR_H_

/* free with free() */
typedef struct {
	void* arr;
	void* end;
	void* alloc_end;
} darr_t;

extern void darr_init(darr_t* dstr); 
extern void darr_append(darr_t* dest, void* var);
extern int darr_length(darr_t* darr);

#endif
