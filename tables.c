/*
    LR(1) and SLR table decompressors for RLE compressed tables.
*/

#include "compiler.h"
#include "lr_parser.h"

SOURCE*   gPrgrmSources = NULL;
VARLIST*  gPrgrmVariables = NULL;
FUNCTION* gPrgrmFunctions = NULL;

int AddSourceToList(SOURCE** list, const char* filename, L_TOKEN* lexing, SYNTAX_TREE* syntax)
{
    SOURCE* iterator;
    unsigned int index = 0;

    iterator = *list;
    while (iterator)
    {
        if (strcmp(iterator->filename, filename) == 0)
        { 
            return 0;
        }

        index++;
        if (iterator->next == NULL) break;
        iterator = iterator->next;
    }

    if (iterator)
    {
        iterator->next = (SOURCE*)malloc(sizeof(SOURCE));
        iterator = iterator->next;
    } else {
        *list = (SOURCE*)malloc(sizeof(SOURCE));
        iterator = *list;
    }

    iterator->filename = filename;
    iterator->lexing = lexing;
    iterator->syntax = syntax;
    iterator->next = NULL;
    iterator->index = index;
    return (index + 1);
}

int AddVariableToList(VARLIST** list, TYPE type, const char* name)
{
    VARLIST* iterator;
    unsigned int index = 0;

    iterator = *list;
    while (iterator)
    {
        if (strcmp(iterator->identifier, name) == 0)
        { 
            return 0;
        }

        index++;
        if (iterator->next == NULL) break;
        iterator = iterator->next;
    }

    if (iterator)
    {
        iterator->next = (VARLIST*)malloc(sizeof(VARLIST));
        iterator = iterator->next;
    } else {
        *list = (VARLIST*)malloc(sizeof(VARLIST));
        iterator = *list;
    }

    iterator->identifier = name;
    iterator->type = type;
    iterator->next = NULL;
    iterator->index = index;
    return (index + 1);
}

int AddFunctionToList(FUNCTION** list, TYPE type, const char* name, VARLIST* params, SYNTAX_BLOCK* source)
{
    FUNCTION* iterator;
    unsigned int index = 0;

    iterator = *list;
    while (iterator)
    {
        if (strcmp(iterator->identifier, name) == 0)
        { 
            return 0;
        }

        index++;
        if (iterator->next == NULL) break;
        iterator = iterator->next;
    }

    if (iterator)
    {
        iterator->next = (FUNCTION*)malloc(sizeof(FUNCTION));
        iterator = iterator->next;
    } else {
        *list = (FUNCTION*)malloc(sizeof(FUNCTION));
        iterator = *list;
    }

    iterator->identifier = name;
    iterator->type = type;
    iterator->parameters = params;
    iterator->block = source;
    iterator->next = NULL;
    iterator->index = index;
    return (index + 1);
}

int FreeSourceList(SOURCE** list)
{
    int count = 0;

    SOURCE* itr;
    itr = *list;
    while (itr)
    {
        *list = itr->next;
        free(itr);
        itr = *list;
    }

    *list = NULL;
    return count;
}

int FreeVariableList(VARLIST** list)
{
    int count = 0;
    VARLIST* itr;
    itr = *list;
    while (itr)
    {
        *list = itr->next;
        free(itr);
        itr = *list;
    }

    *list = NULL;
    return count;
}

int FreeFunctionList(FUNCTION** list)
{
    int count = 0;
    FUNCTION* itr;
    itr = *list;
    while (itr)
    {
        *list = itr->next;
        FreeVariableList(&itr->parameters);
        free(itr);
        itr = *list;
    }

    *list = NULL;
    return count;
}

unsigned int SourceCount(SOURCE* list)
{
    unsigned int count = 0;
    while (list)
    {
        count++;
        list = list->next;
    }
    return count;
}

unsigned int VariableCount(VARLIST* list)
{
    unsigned int count = 0;
    while (list)
    {
        count++;
        list = list->next;
    }
    return count;
}

unsigned int FunctionCount(FUNCTION* list)
{
    unsigned int count = 0;
    while (list)
    {
        count++;
        list = list->next;
    }
    return count;
}

VARLIST*  GetVariableFromList(VARLIST* variableList, const char* identifier)
{
    while (variableList)
    {
        if (strcmp(variableList->identifier, identifier) == 0)
            return variableList;
        variableList = variableList->next;
    }

    return NULL;
}

FUNCTION* GetFunctionFromList(FUNCTION* functionList, const char* identifier)
{
    while (functionList)
    {
        if (strcmp(functionList->identifier, identifier) == 0)
            return functionList;
        functionList = functionList->next;
    }

    return NULL;
}



#ifdef USE_COMPRESSED_TABLES
void DecompressAndPatchParseTable(LR_TABLE* parser, int gotoTable[], int gotoSz, int actionTable[], int actionSz)
{
     parser->gotoTable = (int*)malloc(parser->numStates * parser->numSymbols * sizeof(int));
     parser->actionTable = (ACTION*)malloc(parser->numStates * parser->numTokens * sizeof(ACTION));

     DecompressGotoTable(gotoTable, parser->gotoTable, gotoSz, parser->numStates * parser->numSymbols);
     DecompressActionTable(actionTable, parser->actionTable, actionSz, parser->numStates * parser->numTokens);
}

void DecompressGotoTable(int* source_data, int* dest_data, unsigned int compressed_size, unsigned int total_size)
{
    unsigned int mode = 0;
    unsigned int index = 0;
    unsigned int read_index = 0;
    int lengthRep = 0;
    int copy_value = 0;

    while (/*read_index < compressed_size && */index < total_size)
    {
        if (mode == 0)
        {
            lengthRep = source_data[read_index];
            read_index++;
            while (lengthRep && /*read_index < compressed_size &&*/
                    index < total_size)
            {
                dest_data[index] = source_data[read_index];
                index++; read_index++;
                lengthRep--;
            }
            lengthRep = 0;
            mode = 1;
        } 
        else if (mode == 1) 
        {
            lengthRep = source_data[read_index];
            read_index++;
            copy_value = source_data[read_index];
            read_index++;
            while (lengthRep && index < total_size)
            {
                dest_data[index] = copy_value;
                index++;
                lengthRep--;
            }
            lengthRep = 0;
            mode = 0;
        }
    }
}

void DecompressActionTable(int* source_data, ACTION* dest_data, unsigned int compressed_size, unsigned int total_size)
{
    unsigned int mode = 0;
    unsigned int index = 0;
    unsigned int read_index = 0;
    int lengthRep = 0;
    ACTION copy_value;
    copy_value.type = 0;
    copy_value.value = 0;

    while (/*read_index < compressed_size &&*/ index < total_size)
    {
        if (mode == 0)
        {
            lengthRep = source_data[read_index];
            read_index++;
            while (lengthRep && /*read_index < compressed_size &&*/
                    index < total_size)
            {
                dest_data[index].type = source_data[read_index];
                read_index++;
                dest_data[index].value = source_data[read_index];
                read_index++;
                index++;
                lengthRep--;
            }
            lengthRep = 0;
            mode = 1;
        } 
        else if (mode == 1) 
        {
            lengthRep = source_data[read_index];
            read_index++;
            copy_value.type = source_data[read_index];
            read_index++;
            copy_value.value = source_data[read_index];
            read_index++;
            while (lengthRep && index < total_size)
            {
                dest_data[index].type = copy_value.type;
                dest_data[index].value = copy_value.value;
                index++;
                lengthRep--;
            }
            lengthRep = 0;
            mode = 0;
        }
    }
}
#endif

