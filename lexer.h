//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexerDef.h"

//ARRAYS REQUIRED FOR BUILDING KEYWORD HASH MAP 
char * keyLexeme[]={"integer","real","boolean","of","array","start","end","declare","module","driver","get_value","print","use","with","parameters","true","false","takes","input","returns","AND","OR","in","switch","case","break","default","while","program","for"};
char * keyToken[]={"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","PROGRAM","FOR"};

Token* createToken(char *name, char* lexeme, int line);

void insertToken(TokenList* list, Token* token);

Token* getNextToken();

int fillFileBuf(FILE *fp);

FILE* getStream(FILE *fp);

int hashFunction(char* s, int size);

void insertIntoKeyMap(char* token, char * name, int size);

char * getRecordFromKeyMap(char *token,int size);

void buildKeywordTable();

void removeComments(char* testCaseFile, char* cleanFile);

void lexicalAnalysis(FILE* fp);

void addDollar(char * str);
