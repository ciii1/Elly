#ifndef DEFS_H_
#define DEFS_H_


/* TAG */
enum tags{
 EOF_T,
 EOL_T,
 VARIABLE_T ,
 INT_T ,
 FLOAT_T ,
 STRING_T,
 OPERATOR_T ,
 SYMBOL_T,
 FOR_T,
 WHILE_T,
 FN_T,
 RETURN_T,
 IMPORT_T,
 IF_T,
 ELIF_T,
 ELSE_T,
 BREAK_T,
 CONTINUE_T,
 SEMICOLON_T,
 VAR_T,
 CONST_T,
};
/* TAG2 */
enum tags2{
 ADD_OPR_T,
 SUB_OPR_T,
 MUL_OPR_T,
 DIV_OPR_T,
 MOD_OPR_T,
 AND_OPR_T,
 OR_OPR_T,
 XOR_OPR_T,
 CGREATER_OPR_T,
 CLESS_OPR_T,
 ASSIGNMENT_OPR_T,
 INC_OPR_T,
 DEC_OPR_T,
 MUL_ASSIGN_OPR_T,
 DIV_ASSIGN_OPR_T,
 MOD_ASSIGN_OPR_T,
 AND_ASSIGN_OPR_T,
 OR_ASSIGN_OPR_T,
 XOR_ASSIGN_OPR_T,
 CGREATER_EQU_OPR_T,
 CLESS_EQU_OPR_T,
 CEQU_OPR_T,
 LAND_OPR_T,
 LOR_OPR_T,
 SHR_OPR_T,
 SHL_OPR_T,
 SHR_ASSIGN_OPR_T,
 SHL_ASSIGN_OPR_T,
 LEFT_PAREN_T,
 RIGHT_PAREN_T,
 LEFT_CURLY_BRACKET_T,
 RIGHT_CURLY_BRACKET_T,
 LEFT_BRACKET_T,
 RIGHT_BRACKET_T, 
 COMMA_T,
 DECL_T,
};

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define VARIABLE_STRUCT var_t
#define IS_TRUE_FUNC istrue
#define ADD_OPR_T_FUNC add
#define SUB_OPR_T_FUNC sub
#define MUL_OPR_T_FUNC mul
#define DIV_OPR_T_FUNC div
#define MOD_OPR_T_FUNC mod
#define AND_OPR_T_FUNC and
#define OR_OPR_T_FUNC or
#define XOR_OPR_T_FUNC xor
#define CGREATER_OPR_T_FUNC isgreater
#define CLESS_OPR_T_FUNC isless
#define ASSIGNMENT_OPR_T_FUNC assign
#define INC_OPR_T_FUNC inc
#define DEC_OPR_T_FUNC dec
#define MUL_ASSIGN_OPR_T_FUNC mulassign
#define DIV_ASSIGN_OPR_T_FUNC divassign
#define MOD_ASSIGN_OPR_T_FUNC modassign
#define AND_ASSIGN_OPR_T_FUNC andassign
#define OR_ASSIGN_OPR_T_FUNC orassign
#define XOR_ASSIGN_OPR_T_FUNC xorassign
#define CGREATER_EQU_OPR_T_FUNC isgreaterequ
#define CLESS_EQU_OPR_T_FUNC islessequ
#define CEQU_OPR_T_FUNC isequ
#define LAND_OPR_T_FUNC logicand
#define LOR_OPR_T_FUNC logicor
#define SHR_OPR_T_FUNC shr
#define SHL_OPR_T_FUNC shl
#define SHR_ASSIGN_OPR_T_FUNC shrassign
#define SHL_ASSIGN_OPR_T_FUNC shlassign
#define NULLIFY_FUNC nullify

#define C_IF_KEYWORD if
#define C_ELIF_KEYWORD else if
#define C_ELSE_KEYWORD else
#define C_WHILE_KEYWORD while

#define C_MAIN_FUNC_TYPE int
#define C_MAIN_FUNC_NAME main
#define C_MAIN_FUNC_PARAMS (int argc, char* argv[])

#define PROGRAM_INIT_FUNC proginit
#define C_PROGRAM_INIT_PARAMS (argc, &argv)

#define VAR_HT_STACK_INIT_SIZE 100
#define MAX_TOKEN_VALUE 56
#define INIT_DSTR_SPACE 56
#define INIT_DARR_SPACE 56
#define FN_PARAMS_MAX 100

#endif
