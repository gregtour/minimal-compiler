/* intermediate.c */
#include "lr_parser.h"
#include "language.h"
#include "compiler.h"

// create expression level intermediate code
int GenerateExprIRCode(FSTMT* statement, FUNC_SEGMENT* segment);

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
    {
        // empty section
        break;
    }
    // <fstmt> ::= <identifier> = <expr> ;
    case PROD_FSTMT_IDENTIFIER_A:
    {
        // create assignment IR
		// locate identifier

//		PUSH_INST(assignment, r0, r1, ... );
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
		EXPR* expr = (EXPR*)(statement->children[0]);
		GenerateExprIRCode(expr, segment);
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
    // <fstmt> ::= return ;
    case PROD_FSTMT_RETURN_A:
    {
        // jmp $return_address
        // or ret
        break;
    }
    // <fstmt> ::= break ;
    case PROD_FSTMT_BREAK:
    {
        break;
    }
    // <fstmt> ::= continue ;
    case PROD_FSTMT_CONTINUE:
    {
        break;
    }
    // <fstmt> ::= return <expr> ;
    case PROD_FSTMT_RETURN_B:
    {
        break;
    }
    default:
        AssertS(0, "Unrecognized statement-level instruction encountered.");
        CompilerError("Code generation: Unknown statement.");
        break;
    }

    return 0;
}

int GenerateExprIRCode(EXPR* expression, FUNC_SEGMENT* segment)
{
	switch (expression->production)
	{
	case PROD_CONDITION_AND:
	{
		break;
	}
	case PROD_CONDITION_OR:
	{
		break;
	}
	case PROD_LOGIC_NOT:
	{
		break;
	}
	case PROD_COMPARISON_EQUAL_TO:
	{
		break;
	}
	case PROD_COMPARISON_UNEQUAL_TO:
	{
		break;
	}
	case PROD_COMPARISON_LESS:
	{
		break;
	}
	case PROD_COMPARISON_GREATER:
	{
		break;
	}
	case PROD_COMPARISON_L_EQUAL:
	{
		break;
	}
	case PROD_COMPARISON_G_EQUAL:
	{
		break;
	}
	case PROD_ARITHMETIC_ADD:
	{
		break;
	}
	case PROD_ARITHMETIC_SUBTRACT:
	{
		break;
	}
	case PROD_TERM_MULTIPLY:
	{
		break;
	}
	case PROD_TERM_DIVIDE:
	{
		break;
	}
	case PROD_TERM_MODULUS:
	{
		break;
	}
	case PROD_FACTOR_MINUS:
	{
		break;
	}
	case PROD_FACTOR_NOT:
	{
		break;
	}
	case PROD_FINAL_ARRAY_INDEX:
	{
		break;
	}
	case PROD_FINAL_FUNCTION_CALL:
	{
		break;
	}
	case PROD_FINAL_BOOLEAN:
	{
		break;
	}
	case PROD_FINAL_IDENTIFIER:
	case PROD_FINAL_INTEGER:
	case PROD_FINAL_STRING:
	default:
		break;
	}

	return 0;
}
