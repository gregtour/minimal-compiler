/* intermediate.c */
#include "lr_parser.h"
#include "language.h"
#include "compiler.h"
#include "tables.h"

// create expression level intermediate code for function statements
int GenerateIntermediateCode(FSTMT* statement, void* funcHeader)
{
    Assert(statement != NULL);
    Assert(funcHeader != NULL);

    FUNC_SEGMENT* segment = (FUNC_SEGMENT*)funcHeader;

    switch (statement->production)
    {
    // <fstmt> ::= <type-declaration> ;
    case PROD_FSTMT_TYPE_DECLARATION:
        break;
    // <fstmt> ::= <identifier> = <expr> ;
    case PROD_FSTMT_IDENTIFIER_A:
    {
        // create assignment IR
        break;
    }
    // <fstmt> ::= <identifier> [ <expr> ] = <expr> ;
    case PROD_FSTMT_IDENTIFIER_B:
    {
        // calculate expr_a
        // calculate expr_b
        // store assignment
        break;
    }
    // <fstmt> ::= <expr> ;
    case PROD_FSTMT_EXPR:
    {
        // evaluate expression
        // no need to store result
        break;
    }
    // <fstmt> ::= if ( <expr> ) <block> <else>
    case PROD_FSTMT_IF:
    {
        // output code to evaluate expression
        // create branch label
        // conditionally execute block
        // conditionally execute else
        break;
    }
    // <fstmt> ::= while ( <expr> ) <block>
    case PROD_FSTMT_WHILE:
    {
        // output IR code for while loop
        break;
    }
    // 


    }

    return 0;
}




