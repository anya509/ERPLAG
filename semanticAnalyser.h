//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "typeChecker.c"

void checkRecursion(FuncTableNode* ftnode, char* presscope, int lineNumber);

void checkForStmts(int lineNumber, char* tofind, TreeNode* stmtsNode, char* scope);

void checkForStmt(int lineNumber, char* tofind, TreeNode* stmtNode, char* scope);

int compareArrTypes(char* t1, char* t2, int erline, char* ername);

void compareListOp(TreeNode* idlnode, FuncTableNode* ftnode, char* scope, TreeNode* curr);

void compareListIp(TreeNode* idlnode, FuncTableNode* ftnode, char* scope, TreeNode* curr);

void semanticAnalyser(TreeNode* curr, char* scope);

void semanticAnalysis(TreeNode* curr, char* scope);
