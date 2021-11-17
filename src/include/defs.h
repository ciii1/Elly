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
#define IS_TRUE_FUNC __istrue__
#define ADD_OPR_T_FUNC __add__
#define SUB_OPR_T_FUNC __sub__
#define MUL_OPR_T_FUNC __mul__
#define DIV_OPR_T_FUNC __div__
#define MOD_OPR_T_FUNC __mod__
#define AND_OPR_T_FUNC __and__
#define OR_OPR_T_FUNC __or__
#define XOR_OPR_T_FUNC __xor__
#define CGREATER_OPR_T_FUNC __isgreater__
#define CLESS_OPR_T_FUNC __isless__
#define ASSIGNMENT_OPR_T_FUNC __assign__
#define INC_OPR_T_FUNC __inc__
#define DEC_OPR_T_FUNC __dec__
#define MUL_ASSIGN_OPR_T_FUNC __mulassign__
#define DIV_ASSIGN_OPR_T_FUNC __divassign__
#define MOD_ASSIGN_OPR_T_FUNC __modassign__
#define AND_ASSIGN_OPR_T_FUNC __andassign__
#define OR_ASSIGN_OPR_T_FUNC __orassign__
#define XOR_ASSIGN_OPR_T_FUNC __xorassign__
#define CGREATER_EQU_OPR_T_FUNC __isgreaterequ__
#define CLESS_EQU_OPR_T_FUNC __islessequ__
#define CEQU_OPR_T_FUNC __isequ__
#define LAND_OPR_T_FUNC __logicand__
#define LOR_OPR_T_FUNC __logicor__
#define SHR_OPR_T_FUNC __shr__
#define SHL_OPR_T_FUNC __shl__
#define SHR_ASSIGN_OPR_T_FUNC __shrassign__
#define SHL_ASSIGN_OPR_T_FUNC __shlassign__
#define NULLIFY_FUNC __nullify__

#define IF_KEYWORD if
#define ELIF_KEYWORD else if
#define ELSE_KEYWORD else
#define WHILE_KEYWORD while

#define MAIN_FUNC_TYPE int
#define MAIN_FUNC_NAME main
#define MAIN_FUNC_PARAMS (int argc, char* argv[])

#define PROGRAM_INIT_FUNC __proginit__
#define PROGRAM_INIT_PARAMS (argc, &argv)

#define VAR_HT_STACK_INIT_SIZE 100
#define MAX_TOKEN_VALUE 56
#define INIT_DSTR_SPACE 56
#define INIT_DARR_SPACE 56
#define FN_PARAMS_MAX 100

#endif
