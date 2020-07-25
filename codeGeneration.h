//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct lineAssembly{
    char * instruction;
    struct lineAssembly * nextInstruction;
}LineAssembly;

typedef struct codeAssembyL{
    LineAssembly * startLine;
    LineAssembly * lastLine;
    int numLines;
}CodeAssembyL;
