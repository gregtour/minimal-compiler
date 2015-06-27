/* linker.c */
/* handles library linking for user and language libraries */

#include <stdlib.h>
#include "compiler.h"

extern void EMIT(const char* str, ...);

/*SYNTAX_TREE libPrintsParams;
SYNTAX_TREE libPrintsString;
SYNTAX_TREE libPrintiParams;
SYNTAX_TREE libPrintiInt;*/

VARLIST lStringParam;
VARLIST lIntegerParam;

void LinkPrintf()
{
/*  const char*  funcName;
    SYNTAX_TREE* params;
    TYPE         returnType;
    SYNTAX_TREE* code;

    // prints and printi declarations
    funcName = "prints";
    params = &libPrintsParams;
    returnType.basic = TYPE_VOID;
    returnType.size = 0;
    code = NULL;

    // parameters
    libPrintsParams->token = ;
    libPrintsParams->production = ;
    libPrintsParams->numChildren = ;
    libPrintsParams->children = ;

    libPrintsString->token = ;
    libPrintsString->production = ;
    libPrintsString->numChildren = 0;
    libPrintsString->children = NULL;

    // Link function
    LinkFunctionRoot(returnType, funcName, params, code);

    // printi declaration
    funcName = "printi";
    params = &libPrintiParams;
    returnType.basic = TYPE_VOID;
    returnType.size = 0;
    code == NULL;

    // Link function
    LinkFunctionRoot(returnType, funcName, params, code);

    return;*/

    TYPE typeVoid;
    TYPE stringType;
    TYPE integerType;

    typeVoid.basic = TYPE_VOID;
    typeVoid.size = 0;
    stringType.basic = TYPE_STRING;
    stringType.size = 4;
    integerType.basic = TYPE_INTEGER;
    integerType.size = 4;

    lStringParam.type = stringType;
    lStringParam.identifier = "str";
    lStringParam.index = 0;
    lStringParam.next = NULL;

    lIntegerParam.type = integerType;
    lIntegerParam.identifier = "int";
    lIntegerParam.index = 0;
    lIntegerParam.next = NULL;

    AddFunctionToList(&gPrgrmFunctions, typeVoid, "prints", &lStringParam, NULL);
    AddFunctionToList(&gPrgrmFunctions, typeVoid, "printi", &lIntegerParam, NULL);

    return;
}

/*int IsLibraryDefinition(const char* sourceFile)
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
*/


