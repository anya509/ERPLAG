//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTableDef.h"

SymTableNode* createSymTableNode(char * token, char * scope, char * type, int flag, int value, int width,int in_out_var, int isAssigned, int lineNumber, int offset);

FuncTableNode* createFuncTableNode(char * fname, char ** inT, char ** outT, char ** inP, char ** outP, int numIn, int numOut, int line, int isModule, int numChild);

void insertIntoFuncTable(FuncTableNode* ft);

int getFuncTableIndex(char *fname);

void insertIntoSymTable(SymTableNode * nodeST);

int getSymTableIndex(char * key, char * fname, int in_out_var);

int ifFunctionExists(char * fname);

int ifSymEntryExists(char * key, char * scope, int in_out_var);

int extractInput(TreeNode * curr,int iP);

int extractInput(TreeNode * curr,int oP);

FuncTableNode *  extractFunctionDetails(TreeNode * mod, char * fname);

void addFunction(TreeNode * astNode, char * fname);

void insertAndExtractIDParams(TreeNode * curr, char * scope);

void buildSymbolTable(TreeNode * astTree, char * scope, int nestingLevel);

void initializeTables();
