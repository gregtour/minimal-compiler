// LR Parser
#include <stdlib.h>
#include "lr_parser.h"
#include "compiler.h"
#include "language.h"

// debug assertions
#ifdef _DEBUG
void ClAssert(unsigned int condition, const char* message, const char* source, int lineNumber)
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

#ifdef USE_COMPRESSED_TABLES
    // free uncompressed parse tables
    free(PARSE_TABLE.gotoTable);
    free(PARSE_TABLE.actionTable);
#endif

    getchar();
    return 0;
}

// compiler frontend
int main(int argc, char** argv)
{
    /* int           error; */

    TestParser("static-test.txt");
    getchar();

    return 0;
}

