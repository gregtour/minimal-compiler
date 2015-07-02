/* intermediate.c */
#include "lr_parser.h"
#include "language.h"
#include "compiler.h"

const char* GenerateNewLabel()
{
    return "l";
}

// create expression level intermediate code
int GenerateExprIRCode(FSTMT* statement, FUNC_SEGMENT* segment);

#define PUSH_INST(...)          ;

// create expression level intermediate code for function statements
int GenerateIntermediateCode(FSTMT* statement, void* funcHeader)
{
    FUNC_SEGMENT* segment = (FUNC_SEGMENT*)funcHeader;

    Assert(statement != NULL);
    Assert(funcHeader != NULL);

    switch (statement->production)
    {
    // <fstmt> ::= <type-declaration> ;
    case PROD_FSTMT_TYPE_DECLARATION:
    {
        // empty section
        PUSH_INST("NOP");
        break;
    }
    // <fstmt> ::= <identifier> = <expr> ;
    case PROD_FSTMT_IDENTIFIER_A:
    {
        // create assignment IR
        // locate identifier
//        PUSH_INST(assignment, r0, r1, ... );
        EXPR* expr = (EXPR*)(statement->children[2]);
        GenerateExprIRCode(expr, segment);
        PUSH_INST("POP <identifier>");
        break;
    }
    // <fstmt> ::= <identifier> [ <expr> ] = <expr> ;
    case PROD_FSTMT_IDENTIFIER_B:
    {
        // calculate expr_a
        // calculate expr_b
        // store assignment
        EXPR* expr = (EXPR*)(statement->children[2]);

        PUSH_INST("PUSH $identifier");
        GenerateExprIRCode(expr, segment);
        PUSH_INST("ADD");
        expr = (EXPR*)(statement->children[5]);
        GenerateExprIRCode(expr, segment);
        PUSH_INST("STORE");
        break;
    }
    // <fstmt> ::= <expr> ;
    case PROD_FSTMT_EXPR:
    {
        // evaluate expression
        // no need to store result
        EXPR* expr = (EXPR*)(statement->children[0]);
        GenerateExprIRCode(expr, segment);
        PUSH_INST("POP");
        break;
    }
    // <fstmt> ::= if ( <expr> ) <block> <else>
    case PROD_FSTMT_IF:
    {
        // output code to evaluate expression
        // create branch label
        // conditionally execute block
        // conditionally execute else
        extern const char* GenerateNewLabel();
        const char* label = GenerateNewLabel();
        SYNTAX_BLOCK* block = statement->children[4];
        SYNTAX_TREE* elseBranch;
        GenerateExprIRCode(statement->children[2], segment);
        PUSH_INST("JZ label");

        ProcessCodeBlock(block, GenerateIntermediateCode, (void*)segment);
        PUSH_INST("LABEL label");

        //GenerateIntermediateCode(statement->children[5], (void*)segment);        
        elseBranch = statement->children[5];
        if (elseBranch->production == PROD_ELSE_ELSE)
        {
            SYNTAX_BLOCK* elseBlock;
            Assert(elseBranch->children[1]->token == SYMBOL_BLOCK);
            // <else> ::= else <block>
            elseBlock = (SYNTAX_BLOCK*)(elseBranch->children[1]);
            ProcessCodeBlock(elseBlock, GenerateIntermediateCode, (void*)segment);
            //GenerateIntermediateCode(elseBlock, (void*)segment);        
            //ProcessCodeBlock(elseBlock, StaticTypeChecking, funcHeader);
        }

        break;
    }
    // <fstmt> ::= while ( <expr> ) <block>
    case PROD_FSTMT_WHILE:
    {
        // output IR code for while loop
        extern const char* GenerateNewLabel();
        const char* label = GenerateNewLabel();
        SYNTAX_BLOCK* block = statement->children[4];

        PUSH_INST("LABEL label_START");
        GenerateExprIRCode(statement->children[2], segment);
        PUSH_INST("JZ label_END");

        ProcessCodeBlock(block, GenerateIntermediateCode, (void*)segment);

        PUSH_INST("JUMP label_START");
        PUSH_INST("LABEL label_END");
        break;
    }
    // <fstmt> ::= return ;
    case PROD_FSTMT_RETURN_A:
    {
        // jmp $return_address
        // or ret
        //PUSH_INST("RETURN");

        // square up stack
        PUSH_INST("PUSH 0");
        break;
    }
    // <fstmt> ::= break ;
    case PROD_FSTMT_BREAK:
    {
        //PUSH_INST("BREAK");
        // square up stack
        PUSH_INST("JUMP label_END");
        break;
    }
    // <fstmt> ::= continue ;
    case PROD_FSTMT_CONTINUE:
    {
        // square up stack
        PUSH_INST("JUMP label_START");
        //PUSH_INST("CONTINUE");
        break;
    }
    // <fstmt> ::= return <expr> ;
    case PROD_FSTMT_RETURN_B:
    {
        //PUSH_INST("RETURN EXPR");
        // square up stack
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
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("AND");
        break;
    }
    case PROD_CONDITION_OR:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("OR");
        break;
    }
    case PROD_LOGIC_NOT:
    {
        GenerateExprIRCode(expression->children[1], segment);
        PUSH_INST("NOT");
        break;
    }
    case PROD_COMPARISON_EQUAL_TO:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("CMP-EQUAL");
        break;
    }
    case PROD_COMPARISON_UNEQUAL_TO:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("CMP-UNEQUAL");
        break;
    }
    case PROD_COMPARISON_LESS:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("CMP-LESS");
        break;
    }
    case PROD_COMPARISON_GREATER:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("CMP-GREATER");
        break;
    }
    case PROD_COMPARISON_L_EQUAL:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("CMP-LESS-EQUAL");
        break;
    }
    case PROD_COMPARISON_G_EQUAL:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("CMP-GREATER-EQUAL");
        break;
    }
    case PROD_ARITHMETIC_ADD:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("ADD");
        break;
    }
    case PROD_ARITHMETIC_SUBTRACT:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("SUB");
        break;
    }
    case PROD_TERM_MULTIPLY:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("MULT");
        break;
    }
    case PROD_TERM_DIVIDE:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("DIV");
        break;
    }
    case PROD_TERM_MODULUS:
    {
        GenerateExprIRCode(expression->children[0], segment);
        GenerateExprIRCode(expression->children[2], segment);
        PUSH_INST("MOD");
        break;
    }
    case PROD_FACTOR_MINUS:
    {
        GenerateExprIRCode(expression->children[1], segment);
        PUSH_INST("MINUS");
        break;
    }
    case PROD_FACTOR_NOT:
    {
        GenerateExprIRCode(expression->children[1], segment);
        PUSH_INST("NOT");
        break;
    }
    case PROD_FINAL_ARRAY_INDEX:
    {
        EXPR* expr = (EXPR*)(expression->children[2]);
        PUSH_INST("PUSH $identifier");
        GenerateExprIRCode(expr, segment);
        PUSH_INST("ADD");
        PUSH_INST("LOAD");
        break;
    }
    case PROD_FINAL_FUNCTION_CALL:
    {
        //for each argument
        // GenerateExprIRCode();
        //  
        PUSH_INST("CALL funcname");
        break;
    }
    case PROD_FINAL_BOOLEAN:
    {
        // one of the following
        PUSH_INST("PUSH TRUE");
        PUSH_INST("PUSH FALSE");
        break;
    }
    case PROD_FINAL_IDENTIFIER:
    case PROD_FINAL_INTEGER:
    case PROD_FINAL_STRING:
    default:
        // one of the following
        PUSH_INST("PUSH <variable>");
        PUSH_INST("PUSH <final>");
        break;
    }

    return 0;
}
