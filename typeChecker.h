//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symbolTable.c"

void compareArrTypes1(char* t1, char* t2, int erline);

SymTableNode* getSymTableNode1(char * key, char * fname);
SymTableNode* getSymTableNode2(int lineno, char * key, char * fname);
SymTableNode* getAnyNode(int lineno, char * key, char * fname);

int isPrimtive(SymTableNode* node);
int isBinPlus(TreeNode* node);
int isBinMinus(TreeNode* node);
int isBinMul(TreeNode* node);
int isBinDiv(TreeNode* node);

int isArithmeticOp(TreeNode* node);
int isRelationalOp(TreeNode* node);
int isLogicalOp(TreeNode* node);
int isUnaryOp(TreeNode* node);

void checkWhileIdStmts(int lineNumber, char* tofind, int lineNumber2, char* tofind2, TreeNode* stmtsNode, char* scope, int passt);
void checkWhileIdStmt(int lineNumber, char* tofind, int lineNumber2, char* tofind2, TreeNode* stmtNode, char* scope, int passt);
void checkWhile(TreeNode* node, char* scope, TreeNode* stmtsNode);

int getExprType(TreeNode* node, char* scope);
void typeChecker(TreeNode* curr, char* scope);