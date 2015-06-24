/* auto-generated compiler stubs */
#include "lr_parser.h"
#include "language.h"
#include "compiler.h"


/* production stubs */

/* 1. <program> -> <gstmt*> */
int ReduceProgram(SYNTAX_TREE* node)
{
    SYNTAX_TREE* gstmt_1 = node->children[0];
    Assert(node->numChildren == 1);

    int error = 0;
    error = (error ? error : ProcessNode(gstmt_1));

    return error;
}

/* 2. <gstmt*> -> <gstmt> <gstmt*> */
int ReduceGstmtStarA(SYNTAX_TREE* node)
{
    SYNTAX_TREE* gstmt1 = node->children[0];
    SYNTAX_TREE* gstmt_1 = node->children[1];
    Assert(node->numChildren == 2);

    int error = 0;
    error = (error ? error : ProcessNode(gstmt1));
    error = (error ? error : ProcessNode(gstmt_1));

    return error;
}

/* 3. <gstmt*> -> */
int ReduceGstmtStarB(SYNTAX_TREE* node)
{
    int error = 0;
    Assert(node->numChildren == 0);

    return error;
}

/* 4. <gstmt> -> include <string-list> ; */
int ReduceGstmtA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* string_list1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(string_list1));

    return error;
}

/* 5. <string-list> -> <string> , <string-list> */
int ReduceStringListA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* string1 = node->children[0];
    SYNTAX_TREE* string_list1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(string1));
    error = (error ? error : ProcessNode(string_list1));

    return error;
}

/* 6. <string-list> -> <string> */
int ReduceStringListB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* string1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(string1));

    return error;
}

/* 7. <gstmt> -> <type-declaration> ; */
int ReduceGstmtB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* type_declaration1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(type_declaration1));

    return error;
}

/* 8. <type-declaration> -> <identifier> , <type-declaration> */
int ReduceTypeDeclarationA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* type_declaration1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(type_declaration1));

    return error;
}

/* 9. <type-declaration> -> <identifier> : <type> */
int ReduceTypeDeclarationB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* type1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(type1));

    return error;
}

/* 10. <gstmt> -> <identifier> : <type> <block> */
int ReduceGstmtC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 4);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* type1 = node->children[2];
    SYNTAX_TREE* block1 = node->children[3];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(type1));
    error = (error ? error : ProcessNode(block1));

    return error;
}

/* 11. <gstmt> -> <identifier> ( <param*> ) : <type> <block> */
int ReduceGstmtD(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 7);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* param_1 = node->children[2];
    SYNTAX_TREE* type1 = node->children[5];
    SYNTAX_TREE* block1 = node->children[6];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(param_1));
    error = (error ? error : ProcessNode(type1));
    error = (error ? error : ProcessNode(block1));

    return error;
}

/* 12. <param*> -> <identifier> : <type> */
int ReduceParamStarA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* type1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(type1));

    return error;
}

/* 13. <param*> -> <identifier> : <type> , <param*> */
int ReduceParamStarB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 5);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* type1 = node->children[2];
    SYNTAX_TREE* param_1 = node->children[4];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(type1));
    error = (error ? error : ProcessNode(param_1));

    return error;
}

/* 14. <param*> -> */
int ReduceParamStarC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 0);

    int error = 0;

    return error;
}

/* 15. <block> -> <fstmt> */
int ReduceBlockA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* fstmt1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(fstmt1));

    return error;
}

/* 16. <block> -> { <fstmt*> } */
int ReduceBlockB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* fstmt_1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(fstmt_1));

    return error;
}

/* 17. <type> -> int */
int ReduceTypeA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);

    int error = 0;

    return error;
}

/* 18. <type> -> string */
int ReduceTypeB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);

    int error = 0;

    return error;
}

/* 19. <type> -> int [ ] */
int ReduceTypeC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);

    int error = 0;

    return error;
}

/* 20. <type> -> void */
int ReduceTypeD(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);

    int error = 0;

    return error;
}

/* 21. <fstmt*> -> <fstmt> <fstmt*> */
int ReduceFstmtStarA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* fstmt1 = node->children[0];
    SYNTAX_TREE* fstmt_1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(fstmt1));
    error = (error ? error : ProcessNode(fstmt_1));

    return error;
}

/* 22. <fstmt*> -> */
int ReduceFstmtStarB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 0);

    int error = 0;

    return error;
}

/* 23. <fstmt> -> <type-declaration> ; */
int ReduceFstmtA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* type_declaration1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(type_declaration1));

    return error;
}

/* 24. <fstmt> -> <identifier> = <expr> ; */
int ReduceFstmtB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 4);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* expr1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(expr1));

    return error;
}

/* 25. <fstmt> -> <identifier> [ <expr> ] = <expr> ; */
int ReduceFstmtC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 7);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* expr1 = node->children[2];
    SYNTAX_TREE* expr2 = node->children[5];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(expr1));
    error = (error ? error : ProcessNode(expr2));

    return error;
}

/* 26. <fstmt> -> <expr> ; */
int ReduceFstmtD(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* expr1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));

    return error;
}

/* 27. <fstmt> -> if ( <expr> ) <block> <else> */
int ReduceFstmtE(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 6);
    SYNTAX_TREE* expr1 = node->children[2];
    SYNTAX_TREE* block1 = node->children[4];
    SYNTAX_TREE* else1 = node->children[5];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));
    error = (error ? error : ProcessNode(block1));
    error = (error ? error : ProcessNode(else1));

    return error;
}

/* 28. <else> -> else <block> */
int ReduceElseA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* block1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(block1));

    return error;
}

/* 29. <else> -> */
int ReduceElseB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 0);

    int error = 0;

    return error;
}

/* 30. <fstmt> -> while ( <expr> ) <block> */
int ReduceFstmtF(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 5);
    SYNTAX_TREE* expr1 = node->children[2];
    SYNTAX_TREE* block1 = node->children[4];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));
    error = (error ? error : ProcessNode(block1));

    return error;
}

/* 31. <fstmt> -> return ; */
int ReduceFstmtG(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);

    int error = 0;

    return error;
}

/* 32. <fstmt> -> break ; */
int ReduceFstmtH(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);

    int error = 0;

    return error;
}

/* 33. <fstmt> -> continue ; */
int ReduceFstmtI(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);

    int error = 0;

    return error;
}

/* 34. <fstmt> -> return <expr> ; */
int ReduceFstmtJ(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* expr1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));

    return error;
}

/* 35. <arg*> -> <expr> */
int ReduceArgStarA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* expr1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));

    return error;
}

/* 36. <arg*> -> <expr> , <arg*> */
int ReduceArgStarB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* expr1 = node->children[0];
    SYNTAX_TREE* arg_1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));
    error = (error ? error : ProcessNode(arg_1));

    return error;
}

/* 37. <arg*> -> */
int ReduceArgStarC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 0);

    int error = 0;

    return error;
}

/* 38. <expr> -> <condition> */
int ReduceExpr(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* condition1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(condition1));

    return error;
}

/* 39. <condition> -> <condition> and <logic> */
int ReduceConditionA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* condition1 = node->children[0];
    SYNTAX_TREE* logic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(condition1));
    error = (error ? error : ProcessNode(logic1));

    return error;
}

/* 40. <condition> -> <condition> or <logic> */
int ReduceConditionB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* condition1 = node->children[0];
    SYNTAX_TREE* logic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(condition1));
    error = (error ? error : ProcessNode(logic1));

    return error;
}

/* 41. <condition> -> <logic> */
int ReduceConditionC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* logic1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(logic1));

    return error;
}

/* 42. <logic> -> not <comparison> */
int ReduceLogicA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* comparison1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));

    return error;
}

/* 43. <logic> -> <comparison> */
int ReduceLogicB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* comparison1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));

    return error;
}

/* 44. <comparison> -> <comparison> == <arithmetic> */
int ReduceComparisonA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* comparison1 = node->children[0];
    SYNTAX_TREE* arithmetic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 45. <comparison> -> <comparison> != <arithmetic> */
int ReduceComparisonB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* comparison1 = node->children[0];
    SYNTAX_TREE* arithmetic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 46. <comparison> -> <comparison> < <arithmetic> */
int ReduceComparisonC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* comparison1 = node->children[0];
    SYNTAX_TREE* arithmetic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 47. <comparison> -> <comparison> > <arithmetic> */
int ReduceComparisonD(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* comparison1 = node->children[0];
    SYNTAX_TREE* arithmetic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 48. <comparison> -> <comparison> <= <arithmetic> */
int ReduceComparisonE(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* comparison1 = node->children[0];
    SYNTAX_TREE* arithmetic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 49. <comparison> -> <comparison> >= <arithmetic> */
int ReduceComparisonF(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* comparison1 = node->children[0];
    SYNTAX_TREE* arithmetic1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(comparison1));
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 50. <comparison> -> <arithmetic> */
int ReduceComparisonG(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* arithmetic1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(arithmetic1));

    return error;
}

/* 51. <arithmetic> -> <arithmetic> + <term> */
int ReduceArithmeticA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* arithmetic1 = node->children[0];
    SYNTAX_TREE* term1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(arithmetic1));
    error = (error ? error : ProcessNode(term1));

    return error;
}

/* 52. <arithmetic> -> <arithmetic> - <term> */
int ReduceArithmeticB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* arithmetic1 = node->children[0];
    SYNTAX_TREE* term1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(arithmetic1));
    error = (error ? error : ProcessNode(term1));

    return error;
}

/* 53. <arithmetic> -> <term> */
int ReduceArithmeticC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* term1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(term1));

    return error;
}

/* 54. <term> -> <term> * <factor> */
int ReduceTermA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* term1 = node->children[0];
    SYNTAX_TREE* factor1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(term1));
    error = (error ? error : ProcessNode(factor1));

    return error;
}

/* 55. <term> -> <term> / <factor> */
int ReduceTermB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* term1 = node->children[0];
    SYNTAX_TREE* factor1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(term1));
    error = (error ? error : ProcessNode(factor1));

    return error;
}

/* 56. <term> -> <term> mod <factor> */
int ReduceTermC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* term1 = node->children[0];
    SYNTAX_TREE* factor1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(term1));
    error = (error ? error : ProcessNode(factor1));

    return error;
}

/* 57. <term> -> <factor> */
int ReduceTermD(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* factor1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(factor1));

    return error;
}

/* 58. <factor> -> - <factor> */
int ReduceFactorA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* factor1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(factor1));

    return error;
}

/* 59. <factor> -> ! <factor> */
int ReduceFactorB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 2);
    SYNTAX_TREE* factor1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(factor1));

    return error;
}

/* 60. <factor> -> <final> */
int ReduceFactorC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* final1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(final1));

    return error;
}

/* 61. <final> -> ( <expr> ) */
int ReduceFinalA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 3);
    SYNTAX_TREE* expr1 = node->children[1];

    int error = 0;
    error = (error ? error : ProcessNode(expr1));

    return error;
}

/* 62. <final> -> <identifier> */
int ReduceFinalB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* identifier1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));

    return error;
}

/* 63. <final> -> <identifier> [ <expr> ] */
int ReduceFinalC(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 4);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* expr1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(expr1));

    return error;
}

/* 64. <final> -> <identifier> ( <arg*> ) */
int ReduceFinalD(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 4);
    SYNTAX_TREE* identifier1 = node->children[0];
    SYNTAX_TREE* arg_1 = node->children[2];

    int error = 0;
    error = (error ? error : ProcessNode(identifier1));
    error = (error ? error : ProcessNode(arg_1));

    return error;
}

/* 65. <final> -> <integer> */
int ReduceFinalE(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* integer1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(integer1));

    return error;
}

/* 66. <final> -> <string> */
int ReduceFinalF(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* string1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(string1));

    return error;
}

/* 67. <final> -> <boolean> */
int ReduceFinalG(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);
    SYNTAX_TREE* boolean1 = node->children[0];

    int error = 0;
    error = (error ? error : ProcessNode(boolean1));

    return error;
}

/* 68. <boolean> -> true */
int ReduceBooleanA(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);

    int error = 0;

    return error;
}

/* 69. <boolean> -> false */
int ReduceBooleanB(SYNTAX_TREE* node)
{
    Assert(node->numChildren == 1);

    int error = 0;

    return error;
}

/* reduce one node */
int ProcessNode(SYNTAX_TREE* node)
{
    if (node == NULL || node->production == 0)
        return 1;

    switch (node->production)
    {
        case 0x01: return ReduceProgram(node);
        case 0x02: return ReduceGstmtStarA(node);
        case 0x03: return ReduceGstmtStarB(node);
        case 0x04: return ReduceGstmtA(node);
        case 0x05: return ReduceStringListA(node);
        case 0x06: return ReduceStringListB(node);
        case 0x07: return ReduceGstmtB(node);
        case 0x08: return ReduceTypeDeclarationA(node);
        case 0x09: return ReduceTypeDeclarationB(node);
        case 0x0A: return ReduceGstmtC(node);
        case 0x0B: return ReduceGstmtD(node);
        case 0x0C: return ReduceParamStarA(node);
        case 0x0D: return ReduceParamStarB(node);
        case 0x0E: return ReduceParamStarC(node);
        case 0x0F: return ReduceBlockA(node);
        case 0x10: return ReduceBlockB(node);
        case 0x11: return ReduceTypeA(node);
        case 0x12: return ReduceTypeB(node);
        case 0x13: return ReduceTypeC(node);
        case 0x14: return ReduceTypeD(node);
        case 0x15: return ReduceFstmtStarA(node);
        case 0x16: return ReduceFstmtStarB(node);
        case 0x17: return ReduceFstmtA(node);
        case 0x18: return ReduceFstmtB(node);
        case 0x19: return ReduceFstmtC(node);
        case 0x1A: return ReduceFstmtD(node);
        case 0x1B: return ReduceFstmtE(node);
        case 0x1C: return ReduceElseA(node);
        case 0x1D: return ReduceElseB(node);
        case 0x1E: return ReduceFstmtF(node);
        case 0x1F: return ReduceFstmtG(node);
        case 0x20: return ReduceFstmtH(node);
        case 0x21: return ReduceFstmtI(node);
        case 0x22: return ReduceFstmtJ(node);
        case 0x23: return ReduceArgStarA(node);
        case 0x24: return ReduceArgStarB(node);
        case 0x25: return ReduceArgStarC(node);
        case 0x26: return ReduceExpr(node);
        case 0x27: return ReduceConditionA(node);
        case 0x28: return ReduceConditionB(node);
        case 0x29: return ReduceConditionC(node);
        case 0x2A: return ReduceLogicA(node);
        case 0x2B: return ReduceLogicB(node);
        case 0x2C: return ReduceComparisonA(node);
        case 0x2D: return ReduceComparisonB(node);
        case 0x2E: return ReduceComparisonC(node);
        case 0x2F: return ReduceComparisonD(node);
        case 0x30: return ReduceComparisonE(node);
        case 0x31: return ReduceComparisonF(node);
        case 0x32: return ReduceComparisonG(node);
        case 0x33: return ReduceArithmeticA(node);
        case 0x34: return ReduceArithmeticB(node);
        case 0x35: return ReduceArithmeticC(node);
        case 0x36: return ReduceTermA(node);
        case 0x37: return ReduceTermB(node);
        case 0x38: return ReduceTermC(node);
        case 0x39: return ReduceTermD(node);
        case 0x3A: return ReduceFactorA(node);
        case 0x3B: return ReduceFactorB(node);
        case 0x3C: return ReduceFactorC(node);
        case 0x3D: return ReduceFinalA(node);
        case 0x3E: return ReduceFinalB(node);
        case 0x3F: return ReduceFinalC(node);
        case 0x40: return ReduceFinalD(node);
        case 0x41: return ReduceFinalE(node);
        case 0x42: return ReduceFinalF(node);
        case 0x43: return ReduceFinalG(node);
        case 0x44: return ReduceBooleanA(node);
        case 0x45: return ReduceBooleanB(node);
    default:
        printf("Unknown production %i.\n", node->production);
        return 1;
    }
}
