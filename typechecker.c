/* code-generation.c */
#include "lr_parser.h"
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

// create static information for function mapping
void LinkFunctionRoot(TYPE type, const char* identifier, SYNTAX_TREE* parameters, SYNTAX_TREE* source)
{
    VARLIST* paramList = NULL;
    printf("Linking function %s %s.\n", TypeString(type), identifier);

    // parse function parameters
    if (parameters && parameters->production != PROD_PARAM_STAR)
        // <param*> ::= <epsilon>
    {
        while (parameters)
        {
            const char* identifier;
            TYPE type;
            int result;

            Assert(parameters->children && parameters->numChildren >= 3);
            Assert(parameters->children[0] && parameters->children[0]->string);
            Assert(parameters->children[2]->token == SYMBOL_TYPE);

            identifier = parameters->children[0]->string;
            type = DeriveType(parameters->children[2]);

            result = AddVariableToList(&paramList, type, identifier);
            if (result == 0)
            {
                CompilerError("Parameter variable redefined.");
            }

            // (a) <param*> ::= <identifier> : <type>
            // (b) <param*> ::= <identifier> : <type> , <param*>
            if (parameters->production == PROD_PARAM_STAR_IDENTIFIER_B)
            {   parameters = parameters->children[4]; }
            else 
            {   parameters = NULL; }
        }
    }

    Assert(source->token == SYMBOL_BLOCK);
    // check for redundant functions and link global methods
    if (AddFunctionToList(&gPrgrmFunctions, type, identifier, paramList, (SYNTAX_BLOCK*)source) == 0)
    {
        if (paramList)
        {
            FreeVariableList(&paramList);
        }

        CompilerError("Function redefined.");
    }
}

// create static information for global variable mapping
void LinkVariableRoot(TYPE type, const char* identifier)
{
    if (type.basic == TYPE_VOID)
    {
        CompilerError("Global variable defined as void.");
        return;
    }

    printf("Linking variable %s %s.\n", TypeString(type), identifier);
    if (AddVariableToList(&gPrgrmVariables, type, identifier) == 0)
    {
        CompilerError("Global variable redefined.");
    }
}

void LinkSourceRoot(const char* sourceFile, SYNTAX_TREE** tree)
{
    printf("Linking source file %s.\n", sourceFile);
    
    // allow for linking multiple source files in one project
    if (AddSourceToList(&gPrgrmSources, sourceFile, NULL, NULL) != 0)
    {
        SOURCE* source = GetSourceFromList(gPrgrmSources, sourceFile);

        source->lexing = LexSource(sourceFile, 
                                   &source->buffer, 
                                   CONTEXT_FREE_GRAMMAR);
        if (source->lexing == NULL) {
            printf("Error lexing source '%s'.\n", sourceFile);
            FreeLexing(source->lexing, source->buffer);
            return;
        }

        source->syntax = ParseSource(source->lexing,
                                     PARSE_TABLE,
                                     CONTEXT_FREE_GRAMMAR);
        if (source->syntax == NULL) {
            printf("Error parsing source '%s'.\n", sourceFile);
            FreeLexing(source->lexing, source->buffer);
            source->lexing = NULL;
            return;
        }

        PrePassReductions(&source->syntax);
        SourceReductions(&source->syntax);
        // add to intermediate framework
        FirstPassStaticAnalyzer(source->syntax);
    }
    
    // link standard library
    // if (IsLibraryDefinition(sourceFile))
    // {
    //    LinkLibrary(sourceFile);
    // }
}

// parse source types from type expressions
TYPE DeriveType(TYPE_DECL* sourceExpression)
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

// build the indexed list of local variables (scopeless)
int CreateLocalVariables(FSTMT* statement, void* funcHeader)
{
    FUNC_SEGMENT* segment = (FUNC_SEGMENT*)funcHeader;

    Assert(statement != NULL);
    Assert(funcHeader != NULL);

    // look only at type declarations
    if (statement->production == PROD_FSTMT_TYPE_DECLARATION)
    {
        // <fstmt> ::= <type-declaration> ;
        TYPE type;
        TYPE_DECL* typeTag;
        SYNTAX_TREE* declaration = statement->children[0];

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
        if (type.basic == TYPE_VOID)
        {
            CompilerError("Attempting to define variable(s) as void.");
            return 1;
        }

        // LinkVariable(<type>, <identifier>)
        while (declaration->token == SYMBOL_TYPE_DECLARATION)
        {
            VARLIST* existing;
            const char* identifier;

            Assert(declaration->children);
            Assert(declaration->children[0]);

            identifier = declaration->children[0]->string;

            // check for duplicate list with global vars
            existing = GetVariableFromList(gPrgrmVariables, identifier);
            if (existing != NULL)
            {
                CompilerError("Global variable redefined in local function.");
                Assert(0);

                return 1;
            }

            // check for duplicate list with parameters
            existing = GetVariableFromList(segment->params, identifier);
            if (existing != NULL)
            {
                CompilerError("Parameter redefined in function body.");
                Assert(0);

                return 1;
            }

            //LinkVariableRoot(type, identifier);
            printf("Linking local variable %s %s. \n", TypeString(type), identifier);
            if (AddVariableToList(&segment->locals, type, identifier) == 0)
            {
                CompilerError("Local variable redefined.");
                return 1;
            }

            declaration = declaration->children[2];
        }
    }
    else if (statement->production == PROD_FSTMT_IF)
    {

    }
    else if (statement->production == PROD_FSTMT_WHILE)
    {

    }

    return 0;
}

// check the use of types in all statements and expressions, and check for invalid break / continue statements
int StaticTypeChecking(FSTMT* statement, void* funcHeader)
{
    FUNC_SEGMENT* segment = (FUNC_SEGMENT*)funcHeader;

    Assert(statement != NULL);
    Assert(funcHeader != NULL);

    switch (statement->production)
    {
    // <fstmt> ::= <type-declaration> ;
    case PROD_FSTMT_TYPE_DECLARATION:
    {
        // do nothing
        break;
    }
    // <fstmt> ::= <identifier> = <expr> ;
    case PROD_FSTMT_IDENTIFIER_A:
    {
        const char* identifier;
        VARLIST* assignee;
        EXPR* expr;

        Assert(statement->numChildren == 4);
        Assert(statement->children);
        Assert(statement->children[0] && statement->children[0]->string);

        expr = (EXPR*)(statement->children[2]);
        identifier = statement->children[0]->string;
        assignee = GetVariableFromList(segment->locals, identifier);
        if (assignee == NULL) { assignee = GetVariableFromList(segment->params, identifier); }
        if (assignee == NULL) { assignee = GetVariableFromList(gPrgrmVariables, identifier); }

        if (assignee == NULL)
        {
            CompilerError("Assignment to undeclared variable.");
            return 1;
        }

        if (CheckExpressionType(assignee->type, expr, segment) != 0)
        {
            CompilerError("Incorrect type in assignment.");
            return 1;
        }
        break;
    }
    // <fstmt> ::= <identifier> [ <expr> ] = <expr> ;
    case PROD_FSTMT_IDENTIFIER_B:
    {
        const char* identifier;
        VARLIST* assignee;
        EXPR* expr;
        EXPR* index;
        TYPE integer;

        Assert(statement->numChildren == 6);
        Assert(statement->children);
        Assert(statement->children[0] && statement->children[0]->string);

        expr = (EXPR*)(statement->children[5]);
        index = (EXPR*)(statement->children[2]);
        identifier = statement->children[0]->string;

        assignee = GetVariableFromList(segment->locals, identifier);
        if (assignee == NULL) { assignee = GetVariableFromList(segment->params, identifier); }
        if (assignee == NULL) { assignee = GetVariableFromList(gPrgrmVariables, identifier); }

        if (assignee == NULL)
        {
            CompilerError("Assignment to undeclared array.");
            return 1;
        }

        if (assignee->type.basic != TYPE_INT_ARRAY)
        {
            CompilerError("Error attempting to index non-array type.");
            return 1;
        }

        integer.basic = TYPE_INTEGER;
        integer.size = 4;

        if (CheckExpressionType(integer, expr, segment) != 0)
        {
            CompilerError("Incorrect type in assignment.");
            return 1;
        }

        if (CheckExpressionType(integer, index, segment) != 0)
        {
            CompilerError("Non-integer index used to access array.");
            return 1;
        }
        break;
    }
    // <fstmt> ::= <expr> ;
    case PROD_FSTMT_EXPR:
    {
        TYPE type;
        EXPR* expr;

        type.basic = TYPE_ANY;

        expr = (EXPR*)(statement->children[0]);
        if (CheckExpressionType(type, expr, segment) != 0)
        {
            CompilerError("Invalid expression.");
            return 1;
        }
        break;
    }
    // <fstmt> ::= if ( <expr> ) <block> <else>
    case PROD_FSTMT_IF:
    {
        SYNTAX_BLOCK* block;
        EXPR* expr;
        SYNTAX_TREE* elseBranch;
        TYPE nonvoid;

        expr = (EXPR*)(statement->children[2]);
        block = (SYNTAX_BLOCK*)(statement->children[4]);
        elseBranch = statement->children[5];
        nonvoid.basic = TYPE_NON_VOID;
        nonvoid.size = 4;

        if (CheckExpressionType(nonvoid, expr, segment) != 0)
        {
            CompilerError("Attempt to use void type as a conditional expression.");
            return 1;
        }

        Assert(block->token == SYMBOL_BLOCK);
        ProcessCodeBlock(block, StaticTypeChecking, funcHeader);

        if (elseBranch->production == PROD_ELSE_ELSE)
        {
            SYNTAX_BLOCK* elseBlock;
            Assert(elseBranch->children[1]->token == SYMBOL_BLOCK);
            // <else> ::= else <block>
            elseBlock = (SYNTAX_BLOCK*)(elseBranch->children[1]);
            ProcessCodeBlock(elseBlock, StaticTypeChecking, funcHeader);
        }

        break;
    }
    // <fstmt> ::= while ( <expr> ) <block>
    case PROD_FSTMT_WHILE:
    {
        TYPE nonvoid;
        SYNTAX_BLOCK* block;
        EXPR* expr;
        int loopFlag;

        expr = (EXPR*)(statement->children[2]);
        block = (SYNTAX_BLOCK*)(statement->children[4]);
        nonvoid.basic = TYPE_NON_VOID;
        nonvoid.size = 4;

        if (CheckExpressionType(nonvoid, expr, segment) != 0)
        {
            CompilerError("Attempt to use void type as loop condition.");
            return 1;
        }

        loopFlag = segment->isInLoop;
        segment->isInLoop = 1;
        Assert(block->token == SYMBOL_BLOCK);
        ProcessCodeBlock(block, StaticTypeChecking, funcHeader);
        segment->isInLoop = loopFlag;
        break;
    }
    // <fstmt> ::= return ;
    case PROD_FSTMT_RETURN_A:
    {
        if (segment->returnType.basic != TYPE_VOID)
        {
            CompilerError("Error, use of void return in non-void function.");
            return 1;
        }
        break;
    }
    // <fstmt> ::= break ;
    case PROD_FSTMT_BREAK:
    {
        if (segment->isInLoop == 0)
        {
            CompilerError("Error using break statement outside of loop body.");
            return 1;
        }
        break;
    }
    // <fstmt> ::= continue ; 
    case PROD_FSTMT_CONTINUE:
    {
        if (segment->isInLoop == 0)
        {
            CompilerError("Error using continue statement outside of loop.");
            return 1;
        }
        break;
    }
    // <fstmt> ::= return <expr> ;
    case PROD_FSTMT_RETURN_B:
    {
        EXPR* expr;
        Assert(statement->children && statement->children[1]);
        expr = (EXPR*)(statement->children[1]);
        CheckExpressionType(segment->returnType, expr, segment);
        break;
    }
    default:
        AssertS(0, "Unrecognized statement-level instruction encountered.");
        CompilerError("Unknown statement.");
        break;
    }

    return 0;
}


int IsBinaryOperator(int productionNum)
{
    switch (productionNum)
    {
    case PROD_CONDITION_AND:
    case PROD_CONDITION_OR:
    case PROD_COMPARISON_EQUAL_TO:
    case PROD_COMPARISON_UNEQUAL_TO:
    case PROD_COMPARISON_LESS:
    case PROD_COMPARISON_GREATER:
    case PROD_COMPARISON_L_EQUAL:
    case PROD_COMPARISON_G_EQUAL:
    case PROD_ARITHMETIC_ADD:
    case PROD_ARITHMETIC_SUBTRACT:
    case PROD_TERM_MULTIPLY:
    case PROD_TERM_DIVIDE:
    case PROD_TERM_MODULUS:
        return 1;
    }
    return 0;
}

int IsIntegerOnlyOperator(int productionNum)
{
    switch (productionNum)
    {
    case PROD_COMPARISON_LESS:
    case PROD_COMPARISON_GREATER:
    case PROD_COMPARISON_L_EQUAL:
    case PROD_COMPARISON_G_EQUAL:
    case PROD_ARITHMETIC_SUBTRACT:
    case PROD_TERM_MULTIPLY:
    case PROD_TERM_DIVIDE:
    case PROD_TERM_MODULUS:
        return 1;
    }
    return 0;
}

int IsUnaryOperator(int productionNum)
{
    return (productionNum == PROD_LOGIC_NOT
        || productionNum == PROD_FACTOR_MINUS
        || productionNum == PROD_FACTOR_NOT);
}

int IsStringOperation(int productionNum)
{
    return (productionNum == PROD_ARITHMETIC_ADD
        || productionNum == PROD_FINAL_ARRAY_INDEX
        || productionNum == PROD_FINAL_FUNCTION_CALL);
}

int IsArrayOperation(int productionNum)
{
    return (productionNum == PROD_FINAL_ARRAY_INDEX
        || productionNum == PROD_FINAL_FUNCTION_CALL);
}


// returns 0 if a successful cast is available
int CanCoerceToType(TYPE dest, TYPE source)
{
    switch (dest.basic)
    {
    case TYPE_INTEGER:
        if (source.basic == TYPE_INTEGER)
            return 0;
        else
            return 1;
    case TYPE_STRING:
        return 0;
    case TYPE_INT_ARRAY:
        if (source.basic == TYPE_INT_ARRAY)
            return 0;
        else
            return 1;
    case TYPE_VOID:
        if (source.basic == TYPE_VOID)
            return 0;
        else
            return 1;
    case TYPE_NON_VOID:
        if (source.basic != TYPE_VOID)
            return 0;
        else
            return 1;
        break;
    case TYPE_ANY:
        return 0;
        break;
    }

    CompilerError("Unknown type encountered.");
    Assert(0);
    return 1;
}


// check to see if an expression's type is matching
// this function should be fully recursive on the set of expression subtrees
int CheckExpressionType(TYPE type, EXPR* expr, FUNC_SEGMENT* segment)
{
    AssertS(expr, "Attempting to check null expression.");

    // check tokens //
    if (expr->production == 0)
    {
        // <identifier>
        if (expr->token == gSymbolIdentifier)
        {
            const char* identifier = expr->string;
            VARLIST* variable;
            variable = GetVariableFromList(segment->locals, identifier);
            if (variable == NULL) {
                variable = GetVariableFromList(segment->params, identifier);
            } if (variable == NULL) {
                variable = GetVariableFromList(gPrgrmVariables, identifier);
            }

            if (variable == NULL)
            {
                CompilerError("Undefined variable in expression.");
                return 1;
            } else {
                return (CanCoerceToType(type, variable->type));
            }
        // <integer>
        } else if (expr->token == gSymbolInteger) {
            TYPE integer;
            integer.basic = TYPE_INTEGER;
            integer.size = 4;
            return (CanCoerceToType(type, integer));
        // <string>
        } else if (expr->token == gSymbolString) {
            TYPE string;
            string.basic = TYPE_STRING;
            string.size = 4;
            return (CanCoerceToType(type, string));
        }
    }

    // check booleans //
    if (expr->production == PROD_FINAL_BOOLEAN) {
        TYPE boolType;
        boolType.basic = TYPE_INTEGER;
        boolType.size = 4;
        return (CanCoerceToType(type, boolType));
    }

    // check binary operators //
    if (IsBinaryOperator(expr->production))
    {
        EXPR* subExprA;
        EXPR* subExprB;
        TYPE nonvoid;

        subExprA = (EXPR*)(expr->children[0]);
        subExprB = (EXPR*)(expr->children[2]);

        if (IsIntegerOnlyOperator(expr->production))
        {
            TYPE intType;
            intType.basic = TYPE_INTEGER;
            intType.size = 4;
            if (CheckExpressionType(intType, subExprA, segment) != 0)
            {
                CompilerError("Attempting to use non-integer in integer operations.");
                return 1;
            }
            if (CheckExpressionType(intType, subExprB, segment) != 0)
            {
                CompilerError("Attempting to use non-integer in integer operations.");
                return 1;
            }

            return 0;
        }

        nonvoid.basic = TYPE_NON_VOID;
        nonvoid.size = 4;

        if (CheckExpressionType(nonvoid, subExprA, segment) != 0)
        {
            CompilerError("Attempting to use void value in expression.");
            return 1;
        }
        if (CheckExpressionType(nonvoid, subExprB, segment) != 0)
        {
            CompilerError("Attempting to use void value in expression.");
            return 1;
        }

        return 0;
    }

    // check unary operators //
    if (IsUnaryOperator(expr->production))
    {
        TYPE nonvoid;
        TYPE integer;
        EXPR* subExpr = (EXPR*)(expr->children[1]);

        nonvoid.basic = TYPE_NON_VOID;
        nonvoid.size = 4;

        integer.basic = TYPE_INTEGER;
        integer.size = 4;

        if (expr->production == PROD_LOGIC_NOT
            || expr->production == PROD_FACTOR_NOT)
        {
            return CheckExpressionType(nonvoid, subExpr, segment);
        }

        return CheckExpressionType(integer, subExpr, segment);
    }

    // check array addressing //
    if (expr->production == PROD_FINAL_ARRAY_INDEX)
    {
        // <final> ::= <identifier> [ <expr> ]
        VARLIST* variable;
        EXPR* arrayIndex;
        const char* identifier;
        TYPE intType;

        identifier = expr->children[0]->string;
        arrayIndex = (EXPR*)(expr->children[2]);

        variable = GetVariableFromList(segment->locals, identifier);
        if (variable == NULL) {
            variable = GetVariableFromList(segment->params, identifier);
        } 
        if (variable == NULL) {
            variable = GetVariableFromList(gPrgrmVariables, identifier);
        }

        if (variable == NULL)
        {
            CompilerError("Reference to undeclared variable.");
            return 1;
        }

        if (variable->type.basic != TYPE_INT_ARRAY)
        {
            CompilerError("Using non-array type as an indexed array.");
            return 1;
        }

        intType.basic = TYPE_INTEGER;
        intType.size = 4;

        if (CheckExpressionType(intType, arrayIndex, segment) != 0)
        {
            CompilerError("Using non-integer type as array index.");
            return 1;
        }

        return (CanCoerceToType(type, intType));
    }

    // check function invocation //
    if (expr->production == PROD_FINAL_FUNCTION_CALL)
    {
        // <final> ::= <identifier> ( <arg*> )
        ARG_LIST* arguments;
        const char* identifier;
        FUNCTION* method;
        VARLIST* paramIterator;

        identifier = expr->children[0]->string;
        arguments = (ARG_LIST*)(expr->children[2]);

        method = GetFunctionFromList(gPrgrmFunctions, identifier);

        if (method == NULL) {
            CompilerError("Error calling undeclared function.");
            return 1;
        }

        // check argument types used as parameters, and the argument count
        if (arguments->production == PROD_ARG_STAR) {
            arguments = NULL;
        }
        // begin loop
        paramIterator = method->parameters;
        while (paramIterator && arguments)
        {
            EXPR* argExpr;
            TYPE  ptype;

            //<arg*> ::= <expr>
            //<arg*> ::= <expr> , <arg*>
            //<arg*> ::= <epsilon>
            argExpr = (EXPR*)(arguments->children[0]);
            ptype = paramIterator->type;

            if (CheckExpressionType(ptype, argExpr, segment) != 0)
            {
                CompilerError("Illegal argument type.");
                return 1;
            }

            // advance the parameter iterator
            paramIterator = paramIterator->next;
            // advance the argument iterator
            if (arguments->production == PROD_ARG_STAR_EXPR_B) {
                arguments = (ARG_LIST*)(arguments->children[2]);
                if (arguments->production == PROD_ARG_STAR) {
                    arguments = NULL;
                }
            } else {
                arguments = NULL;
            }
        }

        // check the argument vs. parameter count
        if (paramIterator != NULL || arguments != NULL)
        {
            CompilerError("Illegal argument count.");
            return 1;
        }

        // check function return type
        if (CanCoerceToType(type, method->type) != 0)
        {
            CompilerError("Type error: use of function with invalid return type.");
            return 1;
        }

        return 0;

    }

    // unhandled case : should throw an exception
    AssertS(0, "Control flow error.");
    CompilerError("Unexpected compiler state.");
    return 1;
}

