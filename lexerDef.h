//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include<stdio.h>
#include<stdlib.h>


typedef struct{
	char tokenName[30];
	char tokenLexeme[30];
	int lineNumber;
	struct Token* next;
} Token;


typedef struct{
	int count;
	Token *first;
	Token *last;
} TokenList;

typedef struct{
	char KeyLexeme[30];
	char KeyName[30];
}keyRecord;

