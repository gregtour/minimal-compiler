/* generator.c */
#include "compiler.h"

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
    fprintf(output, ".text\r\n");

    EmitCodeMain(output);
    EmitCodeLibrary(output);
    EmitCodeProgram(output);

    fprintf(output, "# End Of Assembly\r\n");
    fprintf(output, "\r\n");
    fclose(output);

    return 0;
}

