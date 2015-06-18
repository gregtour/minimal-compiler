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
	/* any optimizations prior to converting source code to
	   an intermediate representation */

	// remove unused local variables
	// convert all references to lexical address form

}

void StaticAnalysis(SYNTAX_TREE* root)
{
	FirstPassStaticAnalyzer(root);
	SecondPassStaticAnalysis();
	ThirdPassStaticAnalysis();
}

void FirstPassStaticAnalyzer(SYNTAX_TREE* root)
{
	Assert(root != NULL);
	Assert(root->token == SYMBOL_PROGRAM);
	Assert(root->production == PROD_PROGRAM_GSTMT_STAR);
	Assert(root->numChildren == 1);

	// Analyze all top-level declarations of functions and variables
	SYNTAX_TREE* iterator = root->children[0];

	// Iterate through the list of <gstmt>'s.
	while (iterator && iterator->production == PROD_GSTMT_STAR_GSTMT)
	{
		Assert(iterator->token == SYMBOL_GSTMT_STAR);
		Assert(iterator->numChildren == 2);

		SYNTAX_TREE* statement;
		statement = iterator->children[0];

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
				Assert(list->token == SYMBOL_STRING_LIST);

				SYNTAX_TREE* string = list->children[0];
				Assert(string && string->token == gSymbolString);

				const char* sourcefile = string->string;
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
			Assert(declaration->token == SYMBOL_TYPE_DECLARATION);

			// (A) <type-declaration> ::= <identifier> : <type>
			// (B) <type-declaration> ::= <identifier> , <type-declaration>
			SYNTAX_TREE* typeTag = declaration->children[2];

			while (typeTag->token != SYMBOL_TYPE) 
			{
				Assert(typeTag->token == SYMBOL_TYPE_DECLARATION);
				Assert(typeTag->numChildren == 3);
				typeTag = typeTag->children[2];
			}

			TYPE type = DeriveType(typeTag);
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
			// <gstmt> ::= <identifier> : <type> <block>
			const char* funcName = statement->children[0]->string;
			TYPE returnType = DeriveType(statement->children[2]);
			SYNTAX_TREE* code = statement->children[3];

			// parameterless function method
			LinkFunctionRoot(returnType, funcName, NULL, code);
			break;
			}
		case PROD_GSTMT_IDENTIFIER_B:
			{
			// <gstmt> ::= <identifier> ( <param*> ) : <type> <block>
			const char* funcName = statement->children[0]->string;
			SYNTAX_TREE* params = statement->children[2];
			TYPE returnType = DeriveType(statement->children[5]);
			SYNTAX_TREE* code = statement->children[6];

			// function method
			LinkFunctionRoot(returnType, funcName, params, code);

			break;
			}
		default:
			CompilerError("Unrecognized instruction.");
			break;
		}

		// process the next statement
		iterator = iterator->children[1];
	}
}

void SecondPassStaticAnalysis()
{

}

void ThirdPassStaticAnalysis()
{

}

// Allow for outermost else-clauses on if statements
void RefactorElseStatements(SYNTAX_TREE** astPosition)
{
	int i;
	SYNTAX_TREE* sourceSyntax = *astPosition;

	const int BLOCK_FSTMT_CHILD = 0;
	const int IF_PROD_CODE_CHILD = 4;
	const int IF_PROD_ELSE_CHILD = 5;

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
	SYNTAX_TREE* branch = *astPosition;
    for (i = 0; i < branch->numChildren; i++)
    {
        RefactorElseStatements(&branch->children[i]);
		branch = *astPosition;
    }
}

// Remove excessive non-terminals from the syntax tree
void ReduceProgramAST(SYNTAX_TREE** astPosition)
{
    unsigned int i; 
    unsigned int reducible = 1;
    const unsigned int empty_production = 0xFF;
	unsigned int reducibleTokens[] = 
	{ 
		//SYMBOL_BLOCK,
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
	SYNTAX_TREE* node = *astPosition;

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
                unsigned int child;

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
    }

    // apply for all sub-trees
	SYNTAX_TREE* branch = *astPosition;
    for (i = 0; i < branch->numChildren; i++)
    {
        ReduceProgramAST(&branch->children[i]);
		branch = *astPosition;
    }
}

