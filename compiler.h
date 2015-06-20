/* compiler.h */
#ifndef _COMPILER_H
#define _COMPILER_H
/*
  Builder for the "Minimal Compilable Language" project.
  Uses a basic staticly / strongly typed language spec,
  and some basic analysis and code generation.
*/

#include "lr_parser.h"

/* Parser Sub-tree Alias Types */

typedef SYNTAX_TREE        PROGRAM;
typedef SYNTAX_TREE        GSTMT_LIST;
typedef SYNTAX_TREE        GSTMT;
typedef SYNTAX_TREE        SYNTAX_BLOCK;
typedef SYNTAX_TREE        FSTMT_LIST;
typedef SYNTAX_TREE        FSTMT;
typedef SYNTAX_TREE        EXPR_LIST;
typedef SYNTAX_TREE        TYPE_DECL;
typedef SYNTAX_TREE        EXPR;
typedef SYNTAX_TREE        ARG_LIST;


/* Defined Constants */
#define TYPE_VOID               0
#define TYPE_INTEGER            1
#define TYPE_STRING             2
#define TYPE_INT_ARRAY          3
#define TYPE_USER_TYPE          4
#define TYPE_VOID_POINTER       5
#define TYPE_UNKNOWN            6
#define TYPE_ANY                7
#define TYPE_NON_VOID           8

/* Compiler state */

extern int compiler_error_count;


/* Data Structures */

typedef struct USERTYPE
{
  const char* named;
  /*MEMBERLIST* data;*/
  void*   data;
} USERTYPE;

typedef struct TYPE {
  unsigned int size;
  unsigned int basic;
  /*USERTYPE* user;*/
} TYPE;

/* Type Checking */

typedef struct SOURCE
{
    const char*     filename;
    L_TOKEN*        lexing;
    SYNTAX_TREE*    syntax;
    unsigned int    index;
    struct SOURCE*  next;
} SOURCE;

typedef struct VARLIST
{
    TYPE            type;
    const char*     identifier;
    unsigned int    index;
    struct VARLIST* next;
} VARLIST;

typedef struct FUNCTION
{
    TYPE             type;
    const char*      identifier;
    VARLIST*         parameters;
    SYNTAX_BLOCK*    block;
    unsigned int     index;
    struct FUNCTION* next;
} FUNCTION;

/* Code Generation */

typedef struct FUNC_SEGMENT
{
    TYPE returnType;
    VARLIST* params;
    VARLIST* locals;

    // state based variables, for processing
    int isInLoop;
    unsigned int x;
    // bool isTailRecursive
} FUNC_SEGMENT;

extern unsigned int  gPrgrmFunctionCount;
extern FUNC_SEGMENT* gPrgrmFunctionTable;

/* Compiler Structures */

extern SOURCE*   gPrgrmSources;
extern VARLIST*  gPrgrmVariables;
extern FUNCTION* gPrgrmFunctions;

int AddSourceToList(SOURCE** list, const char* filename, L_TOKEN* lexing, SYNTAX_TREE* syntax);
int AddVariableToList(VARLIST** list, TYPE type, const char* name);
int AddFunctionToList(FUNCTION** list, TYPE type, const char* name, VARLIST* params, SYNTAX_BLOCK* source);

int FreeSourceList(SOURCE** list);
int FreeVariableList(VARLIST** list);
int FreeFunctionList(FUNCTION** list);

unsigned int SourceCount(SOURCE* list);
unsigned int VariableCount(VARLIST* list);
unsigned int FunctionCount(FUNCTION* list);

VARLIST*  GetVariableFromList(VARLIST* list, const char* identifier);
FUNCTION* GetFunctionFromList(FUNCTION* list, const char* identifier);

/* Code Analysis */

void PrePassReductions(SYNTAX_TREE** root);
void SourceReductions(SYNTAX_TREE** root);    // unused
void StaticAnalysis(SYNTAX_TREE* root);

/* Pre-pass reductions */
void ReduceProgramAST(SYNTAX_TREE** program);
void RefactorElseStatements(SYNTAX_TREE** astPosition);

/* Static analysis */
void FirstPassStaticAnalyzer(PROGRAM* root);
void SecondPassStaticAnalysis(SOURCE* sources, FUNCTION* functions, VARLIST* variables);
void ThirdPassStaticAnalysis();

/* Intermediate Code */

void LinkFunctionRoot(TYPE type, const char* identifier, SYNTAX_TREE* parameters, SYNTAX_TREE* source);
void LinkVariableRoot(TYPE type, const char* identifier);
void LinkSourceRoot(const char* sourceFile, SYNTAX_TREE** insertionPoint);

int CreateLocalVariables(FSTMT* statement, void* funcHeader);
int StaticTypeChecking(FSTMT* statement, void* funcHeader);
int GenerateIntermediateCode(FSTMT* statement, void* funcHeader);

/* Abstract syntax tree operations */

void ProcessCodeBlock(SYNTAX_BLOCK* block, int (*computation)(FSTMT*, void*), void* frameData);

/* Type Checking */
int  CheckExpressionType(TYPE type, EXPR* expr, FUNC_SEGMENT* segment);
// int  CheckStaticTypes(SYNTAX_BLOCK* codeblock);
TYPE DeriveType(TYPE_DECL* sourceExpression);


/* Debug tools */
#define CompilerError(message)      CLError(message, __FILE__, __LINE__);
extern void CLError(const char* message, const char* source, int lineNumber);

#ifndef _DEBUG
#define Assert(x)                   ;
#define AssertS(x,message)           ;
/*#define CompilerError(message)       ;*/
#else
/* AssertS(0, message) */
extern void CLAssert(unsigned int condition, const char* message, const char* source, int lineNumber);
#define Assert(x)                   CLAssert((int)(x), NULL, __FILE__, __LINE__);
#define AssertS(x, message)         CLAssert((int)(x), (message), __FILE__, __LINE__);
#endif

/* memory management */

void FreeCompiler();

#endif // _COMPILER_H
