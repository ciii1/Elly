extern char* stracpy(char* source, char* dest);
extern bool is_operator(char ch);
extern bool is_identifier(char ch);
extern void dstr_init(dstr_t* dstr); 
extern void dstr_free(dstr_t* dstr); 
extern void dstr_erase(dstr_t* dstr); 
extern void dstr_append(dstr_t* dest, char* str);
extern void dstr_append_char(dstr_t* dest, char ch);
