/* Static Analysis */
#include "lr_parser.h"
#include "language.h"
#include "compiler.h"

void PrePassReductions(SYNTAX_TREE** root)
{
    // perform important source-level translations
    RefactorElseStatements(root);
    ReduceProgramAST(root);
}

void SourceReductions(SYNTAX_TREE** root)
{
    // any optimizations prior to converting source code to
    // an intermediate representation 
    // remove unused local variables
    // convert all references to lexical address form
}

void StaticAnalysis(SYNTAX_TREE* root)
{
    FirstPassStaticAnalyzer((PROGRAM*)root);
    SecondPassStaticAnalysis(gPrgrmSources, gPrgrmFunctions, gPrgrmVariables);
    ThirdPassStaticAnalysis();
}

// Builds tables of functions and global variables
void FirstPassStaticAnalyzer(PROGRAM* root)
{
    GSTMT_LIST* iterator;

    Assert(root != NULL);
    Assert(root->token == SYMBOL_PROGRAM);
    Assert(root->production == PROD_PROGRAM_GSTMT_STAR);
    Assert(root->numChildren == 1);

    // Analyze all top-level declarations of functions and variables
    iterator = root->children[0];

    // Iterate through the list of <gstmt>'s.
    while (iterator && iterator->production == PROD_GSTMT_STAR_GSTMT)
    {
        GSTMT* statement;

        Assert(iterator->token == SYMBOL_GSTMT_STAR);
        Assert(iterator->numChildren == 2);

        statement = (GSTMT*)iterator->children[0];

        Assert(statement != NULL);
        Assert(statement->token == SYMBOL_GSTMT);

        // Link definitions of top level statements
        switch (statement->production)
        {
        case PROD_GSTMT_INCLUDE:
            {
            SYNTAX_TREE* list = statement->children[1];
            // <gstmt> ::= include <string-list> ;
            while (list)
            {
                SYNTAX_TREE* string = list->children[0];
                const char* sourcefile = string->string;

                Assert(list->token == SYMBOL_STRING_LIST);
                Assert(string && string->token == gSymbolString);

                LinkSourceRoot(sourcefile, &statement);

                if (list->numChildren > 1) 
                {
                    // <string-list> ::= <string> , <string-list>
                    Assert(list->numChildren == 3);
                    list = list->children[2];
                } 
                else 
                {
                    // <string-list> ::= <string>
                    list = NULL;
                }
            }

            break;
            }
        case PROD_GSTMT_TYPE_DECLARATION:
            {
            // <gstmt> ::= <type-declaration> ;
            SYNTAX_TREE* declaration = statement->children[0];
            TYPE_DECL* typeTag;
            TYPE type;
            Assert(declaration->token == SYMBOL_TYPE_DECLARATION);

            // (A) <type-declaration> ::= <identifier> : <type>
            // (B) <type-declaration> ::= <identifier> , <type-declaration>
            typeTag = (TYPE_DECL*)declaration->children[2];

            while (typeTag->token != SYMBOL_TYPE) 
            {
                Assert(typeTag->token == SYMBOL_TYPE_DECLARATION);
                Assert(typeTag->numChildren == 3);
                typeTag = typeTag->children[2];
            }

            type = DeriveType(typeTag);
            // LinkVariable(<type>, <identifier>)
            while (declaration->token == SYMBOL_TYPE_DECLARATION)
            {
                const char* identifier = declaration->children[0]->string;
                LinkVariableRoot(type, identifier);

                declaration = declaration->children[2];
            }

            break;
            }
        case PROD_GSTMT_IDENTIFIER_A:
            {
            const char* funcName;
            TYPE returnType;
            SYNTAX_TREE* code;
            Assert(statement->numChildren == 4);

            // <gstmt> ::= <identifier> : <type> <block>
            funcName = statement->children[0]->string;
            returnType = DeriveType(statement->children[2]);
            code = statement->children[3];

            // parameterless function method
            LinkFunctionRoot(returnType, funcName, NULL, code);
            break;
            }
        case PROD_GSTMT_IDENTIFIER_B:
            {
            const char* funcName; 
            SYNTAX_TREE* params;
            TYPE returnType;
            SYNTAX_TREE* code;

            Assert(statement->numChildren == 7);

            // <gstmt> ::= <identifier> ( <param*> ) : <type> <block>
            funcName = statement->children[0]->string;
            params = statement->children[2];
            returnType = DeriveType(statement->children[5]);
            code = statement->children[6];

            // function method
            LinkFunctionRoot(returnType, funcName, params, code);

            break;
            }
        default:
            AssertS(0, "Unrecognized instruction encountered.");
            CompilerError("Unknown instruction.");
            break;
        }

        // process the next statement
        iterator = iterator->children[1];
    }
}

// data
unsigned int    gPrgrmFunctionCount = 0;
FUNC_SEGMENT*   gPrgrmFunctionTable = NULL;

// Processes function statements in body
void SecondPassStaticAnalysis(SOURCE* sources, FUNCTION* functions, VARLIST* variables)
{
    FUNCTION* iterator;
    unsigned int funcIndex;

    // allocate data table
    gPrgrmFunctionCount = FunctionCount(functions);
    gPrgrmFunctionTable = (FUNC_SEGMENT*)malloc(gPrgrmFunctionCount * sizeof(FUNC_SEGMENT));

    AssertS(gPrgrmFunctionCount > 0, "Program contains no functions.");

    memset((void*)gPrgrmFunctionTable, 0, sizeof(FUNC_SEGMENT) * gPrgrmFunctionCount);

    // process each function
    iterator = functions;
    funcIndex = 0;
    while (iterator)
    {
        SYNTAX_BLOCK* block = iterator->block;

        Assert(funcIndex == iterator->index);

        // set function header properly
        gPrgrmFunctionTable[funcIndex].locals = NULL;
        gPrgrmFunctionTable[funcIndex].params = iterator->parameters;
        gPrgrmFunctionTable[funcIndex].returnType = iterator->type;

        // step zero, create local variable list
        ProcessCodeBlock(block, CreateLocalVariables, (void*)&gPrgrmFunctionTable[funcIndex]);

        // step one, perform type-checking on all statements
        gPrgrmFunctionTable[funcIndex].isInLoop = 0;
        ProcessCodeBlock(block, StaticTypeChecking, (void*)&gPrgrmFunctionTable[funcIndex]);

        // should halt on compiler _errors_ here
        if (compiler_error_count > 0)
        {
            printf("Too many errors. (%i)\n", compiler_error_count);
            return;
        }

        // our one built in function
        if (strcmp("printf", iterator->identifier) == 0)
        {
            Assert(iterator->type.basic == TYPE_INTEGER);
            Assert(iterator->parameters);
            Assert(iterator->parameters->type.basic == TYPE_STRING);
            // ...
            ProcessPrintf(block, &gPrgrmFunctionTable[funcIndex]);
            iterator = iterator->next;
            funcIndex++;
            continue;
        }

        // step two, generate intermediate code
        ProcessCodeBlock(block, GenerateIntermediateCode, (void*)&gPrgrmFunctionTable[funcIndex]);

        // ...

        // iterate on all functions
        iterator = iterator->next;
        funcIndex++;
    }
}

void ThirdPassStaticAnalysis()
{

}

// Allow for outermost else-clauses on if statements
void RefactorElseStatements(SYNTAX_TREE** astPosition)
{
    SYNTAX_TREE* branch;
    SYNTAX_TREE* sourceSyntax;
    int i;

    const int BLOCK_FSTMT_CHILD = 0;
    const int IF_PROD_CODE_CHILD = 4;
    const int IF_PROD_ELSE_CHILD = 5;

    sourceSyntax = *astPosition;

    // <fstmt> ::= if ( <expr> ) <block> <else> 
    for (; sourceSyntax->production == PROD_FSTMT_IF;)
    {
        SYNTAX_TREE* elseBlock;
        SYNTAX_TREE* codeBlock;

        elseBlock = sourceSyntax->children[IF_PROD_ELSE_CHILD];

        // <else> ::= else <block>
        if (elseBlock &&
            elseBlock->production == PROD_ELSE_ELSE)
        {
            break;
        }

        // <else> ::= <epsilon>
        codeBlock = sourceSyntax->children[IF_PROD_CODE_CHILD];

        // <block> ::= <fstmt>
        while (codeBlock &&
               codeBlock->production == PROD_BLOCK_FSTMT)
        {
            SYNTAX_TREE* blockStmt = codeBlock->children[BLOCK_FSTMT_CHILD];

            // <fstmt> ::= if ( <expr> ) <block> <else>
            if (blockStmt && blockStmt->production == PROD_FSTMT_IF)
            {
                SYNTAX_TREE* innerElse = blockStmt->children[IF_PROD_ELSE_CHILD];
                if (innerElse &&
                    innerElse->production == PROD_ELSE_ELSE)
                {
                    // <else> ::= else <block>
                    sourceSyntax->children[IF_PROD_ELSE_CHILD] = innerElse;
                    blockStmt->children[IF_PROD_ELSE_CHILD] = elseBlock;
                    break;
                }
                else
                {
                    // <else> ::= <epsilon>
                    codeBlock = blockStmt->children[IF_PROD_CODE_CHILD];
                }
            }
            else
            {
                // <fstmt> ::= ...
                codeBlock = NULL;
            }
        }

        break; 
    }

    // apply for all sub-trees
    branch = *astPosition;
    for (i = 0; i < branch->numChildren; i++)
    {
        RefactorElseStatements(&branch->children[i]);
        branch = *astPosition;
    }
}

// Remove excessive non-terminals from the syntax tree
void ReduceProgramAST(SYNTAX_TREE** astPosition)
{
    SYNTAX_TREE* node;
    SYNTAX_TREE* branch;
    unsigned int reducible = 1;
    const unsigned int empty_production = 0;
    int i;

    unsigned int reducibleTokens[] = 
    { //SYMBOL_BLOCK,
        SYMBOL_EXPR, 
        SYMBOL_CONDITION, 
        SYMBOL_LOGIC, 
        SYMBOL_COMPARISON, 
        SYMBOL_ARITHMETIC, 
        SYMBOL_TERM, 
        SYMBOL_FACTOR, 
        SYMBOL_FINAL
    };

    if (astPosition == NULL) return;
    node = *astPosition;

    // continue while reducible
    while (reducible)
    {
        reducible = 0;

        // productions that reduce to node->children[0]
        for (i = 0; i < sizeof(reducibleTokens)/sizeof(int); i++)
        {
            if (node->token == reducibleTokens[i] &&
                node->numChildren == 1)
            {
                SYNTAX_TREE* cur = *astPosition;
                int child;

                for (child = 1; child < cur->numChildren; child++)
                {
                    FreeParseTree(cur->children[child]);
                }

                *astPosition = node->children[0];
                node = *astPosition;
                free(cur->children);
                free(cur);
                reducible = 1;
            }
        }

        // parentheses expression
        if (node->production == PROD_FINAL)
        {
            SYNTAX_TREE* cur = *astPosition;
            *astPosition = node->children[1];
            node = *astPosition;
            free(cur->children);
            free(cur);
            reducible = 1;
        }
    }

    // apply for all sub-trees
    branch = *astPosition;
    for (i = 0; i < branch->numChildren; i++)
    {
        ReduceProgramAST(&branch->children[i]);
        branch = *astPosition;
    }
}

// perform an operation on a list of fstmt's arranged in a code { block }
void ProcessCodeBlock(SYNTAX_BLOCK* block, int (*computation)(FSTMT*, void*), void* frameData)
{
    FSTMT_LIST* fstmtList = NULL;
    FSTMT*          fstmt = NULL;

    Assert(block);
    Assert(block->children && block->numChildren >= 0);

    // derive statement block type
    if (block->production == PROD_BLOCK_FSTMT)
    {
        // <block> ::= <fstmt> [PROD_BLOCK_FSTMT]
        Assert(block->children[0]->token == SYMBOL_FSTMT);
        fstmt = (FSTMT*)block->children[0];

    } else if (block->production == PROD_BLOCK) {
        // <block> ::= { <fstmt*> } [PROD_BLOCK]
        Assert(block->children[1]->token == SYMBOL_FSTMT_STAR);
        fstmtList = (FSTMT_LIST*)block->children[1];

        //if (fstmtList->children[0]->token == SYMBOL_FSTMT)
        if (fstmtList->production == PROD_FSTMT_STAR_FSTMT)
        {
            // <fstmt*> ::= <fstmt> <fstmt*> [PROD_FSTMT_STAR_FSTMT]
            fstmt = (FSTMT*)fstmtList->children[0];
        } else {
            // <fstmt*> ::= <epsilon> [PROD_FSTMT_STAR]
            fstmt = NULL;
        }
    } else {
        // ????
        Assert(0);
    }

    // process statements
    if (fstmt)
    {
        do {
            // iterate to next statement
            if (fstmtList) 
            { 
                Assert(fstmtList->production == PROD_FSTMT_STAR_FSTMT);

                fstmt = (FSTMT*)fstmtList->children[0];
                fstmtList = fstmtList->children[1];
                if (fstmtList->production == PROD_FSTMT_STAR)
                    { fstmtList = NULL; }
            }

            // perform calculations
            if (computation(fstmt, frameData) != 0)
            {
                // returning non-zero halts the iteration
                break;
            }

        } while (fstmtList);
    }
}

