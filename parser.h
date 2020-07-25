//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parserDef.h"


// FUNCTION DECLARATIONS
void pop(Stack* st);

void push(Stack*st, StackNode* node );

StackNode* createNode(RhsNode* rnode,TreeNode * tnode);

void buildGrammar(FILE *grammarFile);

void displayMappingTable(void);

void insertIntoMap(char* s, int TorNT, SymType S);

mapRecord getRecordFromMap(char *token);

void displayGrammar(void);

void populate();

void findFirst();

void findfollow();

void fillFirst(int nt);

void fillfollow(int nt);

void fillfirstRHS();

void fillRHS(int rule);

void populate_hash();

void displayFirst();

void makeParseTable();

void displayFirstRhs();

void displayFollow();

void parseInput();

void initializeStackTree();

TreeNode * insertST(RhsNode * start);

void displayParseTree();

void displayStack();

void displayParseTable();

void display(TreeNode * root);
