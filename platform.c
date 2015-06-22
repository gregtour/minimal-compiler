/* platform.c */
/* code relevant to producing assembly for specific platforms */

#include "compiler.h"

#ifdef _LINUX

#define INSTRUCTION_ORDER_SWAPPED

const char* REG_MNC[] = 
{
    "%rax", "%rbx", "%rcx", "%rdx", 
    "%rbp", "%rsi", "%rdi", "%rsp",
    "%r8",  "%r9",  "%r10", "%r11", 
    "%r12", "%r13", "%r14", "%r15", 
};


#endif

#ifdef _WINDOWS

const char* REG_MNC[] = 
{
    "rax", "rbx", "rcx", "rdx", 
    "rbp", "rsi", "rdi", "rsp",
    "r8",  "r9",  "r10", "r11", 
    "r12", "r13", "r14", "r15",
};


#endif
