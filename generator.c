/* generator.c */
#include "compiler.h"


/*void GenerateCode(FILE* output, FUNC_SEGMENT* method)
{
    STACKMACHINE m;
    IR_INST* stackLevelStmts;

    m = InitStackMachine();

    fprintf(output, "%s_proc:\r\n", method->methodName);

    // output function prolog //

    stackLevelStmts = method->stream;
    while (stackLevelStmts)
    {
        IR_INST op;
        // locals //
        REG unassignedReg;
        REG currentReg;
        int occupied;
        op = *stackLevelStmts;

        // state machine based on simple stack instructions //
        switch (op.instr)
        {
        case NOP:            break;
        case PUSH_VALUE:
            unassignedReg = StackFindRegister(&m, &occupied);
            if (occupied) PUSH_REG(unassignedReg);
            MOV_VAL_TO_REG(op.immediate, unassignedReg);
            break;
        case PUSH_VARIABLE:
            unassignedReg = StackFindRegister(&m, &occupied);
            if (occupied) PUSH_REG(unassignedReg);
            MOV_MEM_TO_REG(op.reference_level, op.relative_address, unassignedReg);
            break;
        case POP_VARIABLE:
            currentReg = StackLastRegister(&m);
            MOV_REG_TO_MEM(currentReg, op.reference_level, op.relative_address);
            break;
        case POP:
            currentReg = StackLastRegister(&m);
            break;
        case CALL_FUNCTION:
            break;
        case NOT:
            break;
        case MINUS:
            break;
        case AND:
            break;
        case OR:
            break;
        case ADD:
            break;
        case SUB:
            break;
        case MULT:
            break;
        case DIV:
            break;
        case CMP_EQUAL:
            break;
        case CMP_UNEQUAL:
            break;
        case CMP_LESS:
            break;
        case CMP_GREATER:
            break;
        case CMP_LESS_EQUAL:
            break;
        case CMP_GREATER_EQUAL:
            break;
        case LABEL:
            break;
        case JUMP:
            break;
        case JUMPZ:
            break;
        case LOAD:
            break;
        case STORE:
            break;
        case HOLD:
            break;
        case RETURN:
            break;
        }

        stackLevelStmts = stackLevelStmts->next;
    }

    // output function epilog //
    fprintf(output, "%s_end:\r\n", method->methodName);

    fprintf(output, "\r\n");
}*/


int EmitCodeMain(FILE* output)
{
    fprintf(output, "main:\r\n");
    return 0;
}

int EmitCodeProgram(FILE* output)
{
    return 0;
}

int EmitCodeLibrary(FILE* output)
{
    FILE* input;

    input = fopen("library.s", "rb");
    if (input == NULL)
    {
        return 1;
    }

    fclose(input);
    return 0;
}

/* code generation (x86 assembly) */
int OutputProgram(const char* destination)
{
    FILE* output;
    int nbytes;

    output = fopen(destination, "wb");
    if (output == NULL)
    {
        return 1;
    }

    nbytes = 8 * VariableCount(gPrgrmVariables);
    fprintf(output, "# Program assembly from min comp\r\n");
    fprintf(output, "# Target: 32-bit Unix assembly\r\n");
    fprintf(output, ".global main\r\n");
    fprintf(output, "\r\n");
    fprintf(output, ".data\r\n");
    fprintf(output, "static_heap:\r\n");
    fprintf(output, ".space %i, 0\r\n", nbytes);
    fprintf(output, "\r\n");
    /* output static strings and static data */

    fprintf(output, ".text\r\n");

    EmitCodeMain(output);
    EmitCodeLibrary(output);
    EmitCodeProgram(output);

    fprintf(output, "# End Of Assembly\r\n");
    fprintf(output, "\r\n");
    fclose(output);

    return 0;
}

