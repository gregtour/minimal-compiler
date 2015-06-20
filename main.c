// LR Parser
#include <stdlib.h>
#include "lr_parser.h"
#include "compiler.h"
#include "language.h"

// compiler errors
int compiler_error_count = 0;
void CLError(const char* message, const char* source, int lineNumber)
{
    compiler_error_count++;
    printf("Compiler Error %i: ", compiler_error_count);
    printf("%s [%s, %i]\n", message, source, lineNumber);
}

// debug assertions
#ifdef _DEBUG
void CLAssert(unsigned int condition, const char* message, const char* source, int lineNumber)
{
    if (!condition)
    {
        printf("\nAssert failed %s:%i.\n", source, lineNumber);
        if (message) { printf("Error: %s\n\n", message); }
        getchar();
        exit(0);
    }
}
#endif

// run simple test
int TestParser(const char* program)
{
    L_TOKEN*      lexing;
    SYNTAX_TREE*  syntax;
    char*         buffer;
    int           error;

    printf("\n");
    PrintGrammar(CONTEXT_FREE_GRAMMAR);

    // lex source
    buffer = 0;
    lexing = LexSource(program, &buffer, CONTEXT_FREE_GRAMMAR);
    if (lexing == NULL)
    {
        printf("Error lexing source or empty source file.\n");
        FreeLexing(lexing, buffer);
        return 3;
    }
    PrintLexing(lexing);

#ifdef USE_COMPRESSED_TABLES
    DecompressAndPatchParseTable(&PARSE_TABLE, 
        COMPRESSED_GOTO_TABLE, sizeof(COMPRESSED_GOTO_TABLE)/sizeof(int),
        COMPRESSED_ACTION_TABLE, sizeof(COMPRESSED_ACTION_TABLE)/sizeof(int));
#endif

    // parse source
    syntax = ParseSource(lexing, PARSE_TABLE, CONTEXT_FREE_GRAMMAR);
    if (syntax == NULL)
    {
        printf("Error parsing source.\n");
        FreeLexing(lexing, buffer);
        return 4;
    }
    
    // preprocessor
    PrePassReductions(&syntax);
    PrintParseTree(syntax, CONTEXT_FREE_GRAMMAR);

    // compilation stage 1
    SourceReductions(&syntax);
    StaticAnalysis(syntax);

    // done for now
    printf("Success.\n");
    FreeLexing(lexing, buffer);
    FreeParseTree(syntax);

    // free compiler data structures
    FreeCompiler();

#ifdef USE_COMPRESSED_TABLES
    // free uncompressed parse tables
    free(PARSE_TABLE.gotoTable);
    free(PARSE_TABLE.actionTable);
#endif

    getchar();
    return 0;
}

// memory management
void FreeCompiler()
{
    // free compiler data structures
    FreeVariableList(&gPrgrmVariables);
    FreeFunctionList(&gPrgrmFunctions);
    FreeSourceList(&gPrgrmSources);

    // zero the pointer handles
    gPrgrmVariables = NULL;
    gPrgrmFunctions = NULL;
    gPrgrmSources = NULL;

    // clear the function table
    if (gPrgrmFunctionTable) 
    {
        unsigned int i;
        for (i = 0; i < gPrgrmFunctionCount; i++)
        {
            FreeVariableList(&gPrgrmFunctionTable[i].locals);
        }

        free(gPrgrmFunctionTable);
        gPrgrmFunctionTable = NULL;
        gPrgrmFunctionTable = 0;
    }
}

// compiler frontend
int main(int argc, char** argv)
{
    /* int           error; */

    TestParser("program.txt");
    //TestParser("static-test.txt");
    getchar();

    return 0;
}

