/* linker.c */
/* handles library linking for user and language libraries */

#include <stdlib.h>
#include "compiler.h"

int IsLibraryDefinition(const char* sourceFile)
{
    if (strcmp("standard.inc", sourceFile) == 0)
    {
        return 1;
    }

    return 0;
}

void LinkLibrary(const char* sourceFile)
{
    if (strcmp("standard.inc", sourceFile) == 0)
    {
        //LinkFunctionRoot(printfDef);

    }
    // else if (strcmp("math.inc", sourceFile) == 0)
    //{
    //
    // }
    // else if (...) {
    //
    // }
}

void ProcessPrintf(SYNTAX_BLOCK* block, FUNC_SEGMENT* segment) { }
