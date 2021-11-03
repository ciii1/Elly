#ifndef LEXER_H_
#define LEXER_H_

extern token_t lex_init_token(char* value, tag_t tag, tag_t tag2);
extern token_t lex_next_token();
extern token_t lex_peek_token();
extern token_t lex_jump_peek_token();

#endif
