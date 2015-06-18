/* code-generation.c */
#include "language.h"
#include "compiler.h"

const char* TypeString(TYPE typeIdentifier)
{
    switch (typeIdentifier.basic)
    {
    case TYPE_INTEGER: return "INT";
    case TYPE_STRING: return "STRING";
    case TYPE_INT_ARRAY: return "INT[]";
    case TYPE_VOID: return "VOID";
    }
    return "N/A";
}

/* create static information for function and global variable mapping */
void LinkFunctionRoot(TYPE type, const char* identifier, SYNTAX_TREE* parameters, SYNTAX_TREE* source)
{
    printf("Linking function %s %s.\n", TypeString(type), identifier);
}

void LinkVariableRoot(TYPE type, const char* identifier)
{
    printf("Linking variable %s %s.\n", TypeString(type), identifier);
}

void LinkSourceRoot(const char* sourceFile, SYNTAX_TREE** insertionPoint)
{
    printf("Linking source file %s.\n", sourceFile);
}

/* parse source types from type expressions */
TYPE DeriveType(SYNTAX_TREE* sourceExpression)
{
    TYPE baseType;
    baseType.basic = TYPE_VOID;
    baseType.size = 4;

    if (sourceExpression
        && sourceExpression->token == SYMBOL_TYPE)
    {
        switch (sourceExpression->production)
        {
        case PROD_TYPE_INT_A:
            baseType.basic = TYPE_INTEGER;
            baseType.size = 4;
            break;
        case PROD_TYPE_STRING:
            baseType.basic = TYPE_STRING;
            baseType.size = 4;
            break;
        case PROD_TYPE_INT_B:
            baseType.basic = TYPE_INT_ARRAY;
            baseType.size = 4;
            break;
        case PROD_TYPE_VOID:
            baseType.basic = TYPE_VOID;
            baseType.size = 4;
            break;
        default:
            CompilerError("Unknown type.");
        }
    }
    return baseType;
}
