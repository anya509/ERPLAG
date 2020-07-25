//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "ast_1.c"
#include "ast.c"

#define SIZE_IN_ 30
#define SIZE_OUT_ 30
#define SIZE_FT_ 30

struct symTableNode{
	int value; // if num
    int isArray; // 1 if num or  0 is array type
    char tokenName[30]; // lexeme
    char scope[30];
    char type[30];
    int width;
    int offset;
	int in_out_var; // 0 for var , 1 for input var and 2 for output vars
	int isAssigned;
	int lineNumber;
	int nestingLevel;
	char ifArrayRange[30]; // 0 for number lexemes and 1 for variables
	int ifArrayDynamic; //0 for static array and 1 for dynamic array;
    struct symTableNode * next;
};
typedef struct symTableNode SymTableNode;

struct scopeHashTable{
	SymTableNode * in_table[SIZE_IN_];
};
typedef struct scopeHashTable ScopeHashTable;

struct symbolTable{
	ScopeHashTable * out_table[SIZE_OUT_];
};
typedef struct symbolTable SymbolTable;

struct funcTableNode{
	char funcName[50];
	char inputType[10][50];
	char outputType[10][50];
	char inputParam[10][50];
	char outputParam[10][50];
	int numInputParam;
	int numOutputParam;
    int line;
    int isDefined;
	int isModule; // 1 for module and 0 for other scopes
	int numChild;
	int isDeclared;
	int startScope;
	int endScope;
	int iferror;
	struct funcTableNode * children[20];
	struct funcTableNode * parent;
};
typedef struct funcTableNode FuncTableNode;

struct funcTable{
	FuncTableNode * ftable[SIZE_FT_];
};
typedef struct funcTable FuncTable;

//typedef struct astNode{
//    int value;
//    int flag;
//    char tokenName[30];
//    char tokenLexeme[30]; // check if required to use union for num values
//    int lineNumber;
//    struct astNode * parent;
//    struct astNode * child;
//    struct astNode * right;
//}ASTNode;
