/* assembler.c */
/* output assembly to pass to MSVC cl.exe */

#include <stdio.h>
#include <stdarg.h>

#define ERROR(...)               ;

FILE* working_assembler_output = NULL;
int emit_tabs = 0;

void EMIT(const char* str, 
          ...)
{
    va_list args;
    va_start(args, str);
    if (emit_tabs) fprintf(working_assembler_output, "    ");
    vfprintf(working_assembler_output, str, args);
    fprintf(working_assembler_output, "\r\n");
    va_end(args);
}

void EMIT_L(const char* str, 
          ...)
{
    va_list args;
    va_start(args, str);
    vfprintf(working_assembler_output, str, args);
    fprintf(working_assembler_output, "\r\n");
    va_end(args);
}

void EMIT_HEADER()
{
    EMIT("// Beginning of source file");
    EMIT("#include <stdio.h>");
    EMIT("#include <stdlib.h>");
    EMIT("");
    EMIT("int main(int arg_c, char* arg_v[]) \r\n{");
    EMIT("  __asm");
    EMIT("  {");
    emit_tabs = 1;
}

void EMIT_FOOTER()
{
    emit_tabs = 0;
    EMIT("  }");
    EMIT("  ");
    EMIT("  return 0;");
    EMIT("}");
}

void GenerateProgram();
void assemble_code(const char* output_src,
                         void* IR)
{
    FILE* output;
    output = fopen(output_src, "wb");
    if (output == NULL)
    {
        ERROR("Could not open output file %s.\n", output_src);
        return;
    } else {
        working_assembler_output = output;
    }
    
    EMIT_HEADER();
  
    EMIT("; Beginning of source assembly");
    GenerateProgram();
    EMIT("_emit 0x90 ;");
    
    EMIT_FOOTER();
    
    fprintf(output, "\r\n");
    fclose(output);
    return;
}

int main(int ac, char* av[]) { assemble_code("out.c", NULL); return 0; }

void STRINGCONST(const char* s)
{
    unsigned int i = 0;
    while (s[i]) { EMIT("_emit '%c'", s[i]); i++; }
    EMIT("_emit 0");
}

void GenerateProgram()
{
    EMIT("jp start");
    EMIT_L("head:");

    STRINGCONST("World");   // 6    
    STRINGCONST("Hello");   // 6
    STRINGCONST("%s, %s."); // 8

    EMIT_L("start:");
    EMIT("mov eax, head");
    EMIT("push eax");
    EMIT("add eax, 6");
    EMIT("push eax");
    EMIT("add eax, 6");
    EMIT("push eax");
    EMIT("call printf");
    EMIT("pop ebx");
    EMIT("pop ebx");
    EMIT("pop ebx");
}

