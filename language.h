/* auto-generated lr parse tables */
#ifndef _GENERATED_PARSER_H
#define _GENERATED_PARSER_H

#include "lr_parser.h"

/* main function */
int ProcessNode(SYNTAX_TREE* node);

/* symbol constants */
#define SYMBOL_PROGRAM                    0x1001
#define SYMBOL_GSTMT_STAR                 0x1002
#define SYMBOL_GSTMT                      0x1003
#define SYMBOL_STRING_LIST                0x1004
#define SYMBOL_TYPE_DECLARATION           0x1005
#define SYMBOL_TYPE                       0x1006
#define SYMBOL_BLOCK                      0x1007
#define SYMBOL_PARAM_STAR                 0x1008
#define SYMBOL_FSTMT                      0x1009
#define SYMBOL_FSTMT_STAR                 0x100A
#define SYMBOL_EXPR                       0x100B
#define SYMBOL_ELSE                       0x100C
#define SYMBOL_ARG_STAR                   0x100D
#define SYMBOL_CONDITION                  0x100E
#define SYMBOL_LOGIC                      0x100F
#define SYMBOL_COMPARISON                 0x1010
#define SYMBOL_ARITHMETIC                 0x1011
#define SYMBOL_TERM                       0x1012
#define SYMBOL_FACTOR                     0x1013
#define SYMBOL_FINAL                      0x1014
#define SYMBOL_BOOLEAN                    0x1015

/* production constants */
#define PROD_PROGRAM_GSTMT_STAR                     0x0001
#define PROD_GSTMT_STAR_GSTMT                       0x0002
#define PROD_GSTMT_STAR                             0x0003
#define PROD_GSTMT_INCLUDE                          0x0004
#define PROD_STRING_LIST_STRING_A                   0x0005
#define PROD_STRING_LIST_STRING_B                   0x0006
#define PROD_GSTMT_TYPE_DECLARATION                 0x0007
#define PROD_TYPE_DECLARATION_IDENTIFIER_A          0x0008
#define PROD_TYPE_DECLARATION_IDENTIFIER_B          0x0009
#define PROD_GSTMT_IDENTIFIER_A                     0x000A
#define PROD_GSTMT_IDENTIFIER_B                     0x000B
#define PROD_PARAM_STAR_IDENTIFIER_A                0x000C
#define PROD_PARAM_STAR_IDENTIFIER_B                0x000D
#define PROD_PARAM_STAR                             0x000E
#define PROD_BLOCK_FSTMT                            0x000F
#define PROD_BLOCK                                  0x0010
#define PROD_TYPE_INT_A                             0x0011
#define PROD_TYPE_STRING                            0x0012
#define PROD_TYPE_INT_B                             0x0013
#define PROD_TYPE_VOID                              0x0014
#define PROD_FSTMT_STAR_FSTMT                       0x0015
#define PROD_FSTMT_STAR                             0x0016
#define PROD_FSTMT_TYPE_DECLARATION                 0x0017
#define PROD_FSTMT_IDENTIFIER_A                     0x0018
#define PROD_FSTMT_IDENTIFIER_B                     0x0019
#define PROD_FSTMT_EXPR                             0x001A
#define PROD_FSTMT_IF                               0x001B
#define PROD_ELSE_ELSE                              0x001C
#define PROD_ELSE                                   0x001D
#define PROD_FSTMT_WHILE                            0x001E
#define PROD_FSTMT_RETURN_A                         0x001F
#define PROD_FSTMT_BREAK                            0x0020
#define PROD_FSTMT_CONTINUE                         0x0021
#define PROD_FSTMT_RETURN_B                         0x0022
#define PROD_ARG_STAR_EXPR_A                        0x0023
#define PROD_ARG_STAR_EXPR_B                        0x0024
#define PROD_ARG_STAR                               0x0025
#define PROD_EXPR_CONDITION                         0x0026
#define PROD_CONDITION_AND                          0x0027
#define PROD_CONDITION_OR                           0x0028
#define PROD_CONDITION_LOGIC                        0x0029
#define PROD_LOGIC_NOT                              0x002A
#define PROD_LOGIC_COMPARISON                       0x002B
#define PROD_COMPARISON_EQUAL_TO                    0x002C
#define PROD_COMPARISON_UNEQUAL_TO                  0x002D
#define PROD_COMPARISON_LESS                        0x002E
#define PROD_COMPARISON_GREATER                     0x002F
#define PROD_COMPARISON_L_EQUAL                     0x0030
#define PROD_COMPARISON_G_EQUAL                     0x0031
#define PROD_COMPARISON_ARITHMETIC                  0x0032
#define PROD_ARITHMETIC_ADD                         0x0033
#define PROD_ARITHMETIC_SUBTRACT                    0x0034
#define PROD_ARITHMETIC_TERM                        0x0035
#define PROD_TERM_MULTIPLY                          0x0036
#define PROD_TERM_DIVIDE                            0x0037
#define PROD_TERM_MODULUS                           0x0038
#define PROD_TERM_FACTOR                            0x0039
#define PROD_FACTOR_MINUS                           0x003A
#define PROD_FACTOR_NOT                             0x003B
#define PROD_FACTOR_FINAL                           0x003C
#define PROD_FINAL                                  0x003D
#define PROD_FINAL_IDENTIFIER                       0x003E
#define PROD_FINAL_ARRAY_INDEX                      0x003F
#define PROD_FINAL_FUNCTION_CALL                    0x0040
#define PROD_FINAL_INTEGER                          0x0041
#define PROD_FINAL_STRING                           0x0042
#define PROD_FINAL_BOOLEAN                          0x0043
#define PROD_BOOLEAN_TRUE                           0x0044
#define PROD_BOOLEAN_FALSE                          0x0045

/* Grammar Symbols */
extern char GRAMMAR_SYMBOLS[203];

/* Grammar Tokens */
extern char GRAMMAR_TOKENS[194];

/* Context Free Grammar */
extern GRAMMAR_TABLE CONTEXT_FREE_GRAMMAR;

/* LR(1) GOTO table */
extern int COMPRESSED_GOTO_TABLE[1691];
//extern int GOTO_TABLE[9198];

/* LR(1) ACTION table */
extern int COMPRESSED_ACTION_TABLE[14762];
//extern ACTION ACTION_TABLE[19272];

/* LR(1) parse table struct */
extern LR_TABLE PARSE_TABLE;

#endif // _GENERATED_PARSER_H
