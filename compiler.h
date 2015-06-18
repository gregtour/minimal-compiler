/* compiler.h */
#ifndef _COMPILER_H
#define _COMPILER_H
/*
  Builder for the "Minimal Compilable Language" project.
  Uses a basic staticly / strongly typed language spec,
  and some basic analysis and code generation.
*/

#include "lr_parser.h"

/* Defined Constants */

#define TYPE_VOID               0
#define TYPE_INTEGER            1
#define TYPE_STRING             2
#define TYPE_INT_ARRAY          3
#define TYPE_USER_TYPE          4
#define TYPE_VOID_POINTER       5
#define TYPE_UNKNOWN            6

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

/* Code Analysis */

void ReduceProgramAST(SYNTAX_TREE** program);
void RefactorElseStatements(SYNTAX_TREE** astPosition);

void FirstPassStaticAnalyzer(SYNTAX_TREE* root);
void SecondPassStaticAnalysis();
void ThirdPassStaticAnalysis();

void PrePassReductions(SYNTAX_TREE** root);
void SourceReductions(SYNTAX_TREE** root);
void StaticAnalysis(SYNTAX_TREE* root);

/* Intermediate Code */

void LinkFunctionRoot(TYPE type, const char* identifier, SYNTAX_TREE* parameters, SYNTAX_TREE* source);
void LinkVariableRoot(TYPE type, const char* identifier);
void LinkSourceRoot(const char* sourceFile, SYNTAX_TREE** insertionPoint);

TYPE DeriveType(SYNTAX_TREE* sourceExpression);

/* Code Generation */


/* Debug tools */
#ifndef _DEBUG
#define Assert(x)                   ;
#define Assert(x,message)           ;
#define CompileError(message)       ;
#else
#define CompilerError(message)      AssertS(0, message)
extern void ClAssert(unsigned int condition, const char* message, const char* source, int lineNumber);
#define Assert(x)                   ClAssert((int)(x), NULL, __FILE__, __LINE__);
#define AssertS(x, message)         ClAssert((int)(x), (message), __FILE__, __LINE__);
#endif

#endif // _COMPILER_H
