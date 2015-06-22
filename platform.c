/* platform.c */
/* code relevant to producing assembly for specific platforms */

#include "compiler.h"

#ifdef _LINUX

const char* REG_MNC[] = 
{
    "%rax", "%rbx", "%rdx", "%rsp",
    "%rbp", "%rsi", "%rdi", "%r8",
    "%r9",  "%r10", "%r11", "%r12",
    "%r13", "%r14", "%r15", "%r16",
};


#endif

#ifdef _WINDOWS

const char* REG_MNC[] = 
{
    "rax", "rbx", "rdx", "rsp",
    "rbp", "rsi", "rdi", "r8",
    "r9",  "r10", "r11", "r12",
    "r13", "r14", "r15", "r16",
};


#endif
