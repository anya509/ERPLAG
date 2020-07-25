//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "lexer.h"

#define fileBufLen 5000
#define maxTokLen 25
#define fileBufInd end
#define keywordsnum 30
#define keyMapSize 47

keyRecord KeyMap[keyMapSize]; // KEYWORD MAP

char fileBuf[fileBufLen]="";
int fileBufInd;
Token* token; // GLOBAL VARIABLES THAT STORES THE LATEST TOKEN ON EACH getNextToken call
int lineno=1,end=0,start=0,total_tok;
FILE* fp; // POINTER TO THE SOURCE CODE FILE
int endInd;
int lenflag;

//-------------MODULES-------------------------------

// FUNCTIONS FOR HASHING THE KEYWORD LEXEMES
int hashFunction(char* token,int size){

    int index=0,hash=31;
    for(int i=0;i<strlen(token);i++){
        index=(index+hash+(i+1)*11+token[i])%size;
    }
    return index;
}

void insertIntoKeyMap(char* token, char* name, int size){

    int index=hashFunction(token,size);
    //printf("Index: %d\n",index);
    int i=index;

    //----HANDLING COLLISIONS-----
    while(strcmp(KeyMap[i].KeyLexeme,"")!=0){
        i=(i+1)%size;
    }
  	//----------------------------
    strcpy(KeyMap[i].KeyLexeme,token);
    strcpy(KeyMap[i].KeyName,name);
}


char * getRecordFromKeyMap(char *token,int size){ // Used to check if the given identifer is a keyword
    int index,i;
    index=hashFunction(token,size);
    i=index;

    //-----IN CASE OF COLLISIONS-----
    while(strcmp(KeyMap[i].KeyLexeme,token)!=0){
        i++;
        i=i%size;
        if(i==index){ // when the token is not present in the map i.e it is not a keyword
            return NULL;
        }
    }
    //----------------------------
    return KeyMap[i].KeyName; // returns the keyword token name
}


void buildKeywordTable(){ // construct the keyword hash map

    for(int i=0;i<keywordsnum;i++){
        insertIntoKeyMap(keyLexeme[i], keyToken[i],keyMapSize);
    }
}


// SUPPORTING MODULES
Token* createToken(char *lexeme, char* name, int line){

	Token* newToken = (Token*) malloc(sizeof(Token));

	strcpy(newToken->tokenName, name);
	strcpy(newToken->tokenLexeme, lexeme);
	newToken->lineNumber = line;
	newToken->next = NULL;

	return newToken;
}
/*
void insertToken(TokenList* list, Token* token){

	if(list->first==NULL){
		list->first = token;
		list->last = token;
		list->count = 1;
		return;
	}

	Token* temp = list->last;
	temp->next = token;
	list->last = token;
}
*/
int checkForAlpha(char ch){

	if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')){
		return 1;
	}
	return 0;
}

int checkFordigit(char ch){

	if(ch>='0' && ch<='9'){
		return 1;
	}
	return 0;
}

//---------REMOVE COMMENTS MODULE-----------------------------------------------------------

void removeComments(char* testCaseFile, char* cleanFile){

    FILE* fipo = fopen(testCaseFile,"r"); //File containing the source code
    if(fipo==NULL){
        printf("Error opening the source code file\n");
        exit(0);
    }
    FILE* fp_clean=fopen(cleanFile,"w+"); //File containing the clean code
    if(fp_clean==NULL){
        printf("Error opening the clean file\n");
        exit(0);
    }

    char testFileBuffer[fileBufLen];
    char cleanFileBuffer[fileBufLen];
    int clean_count=0;
    int prev,comment=0; //To track the comment marks
    int count=0;

    count=fread(testFileBuffer,1,(size_t)fileBufLen,fipo);
    while(count>0){
        int i=0;
        while(i<count){
            //printf("%c,",testFileBuffer[i]);
            if(testFileBuffer[i]=='*'){
                if(prev==1){
                    comment=1-comment;
                    prev=0;
                }
                else
                    prev=1;
            }
            else{
                if(prev==1){
                    if(!comment){
                        if(clean_count>=fileBufLen){
                            //fputs(cleanFileBuffer,fp_clean);
                            fwrite(cleanFileBuffer,(size_t)fileBufLen,1, fp_clean);
                            printf("%s",cleanFileBuffer);
                            clean_count=0;
                        }
                        cleanFileBuffer[clean_count]='*';
                        clean_count++;
                    }
                    prev=0;
                }

                if(clean_count>=fileBufLen){
                    //fputs(cleanFileBuffer,fp_clean);
                    fwrite(cleanFileBuffer,1,(size_t)fileBufLen, fp_clean);
                    printf("%s",cleanFileBuffer);
                    clean_count=0;
                }
                if(!comment || testFileBuffer[i]=='\n'){
                    cleanFileBuffer[clean_count]=testFileBuffer[i];
                    clean_count++;
                }
            }
            i++;

        }
        count=fread(testFileBuffer,1,(size_t)fileBufLen,fipo);
    }
    //Write the last buffer block
    fwrite(cleanFileBuffer,1,(size_t)clean_count, fp_clean);
    cleanFileBuffer[clean_count]='\0';
    printf("%s",cleanFileBuffer);
    // Incase the last char is a single *
    if(prev==1){
        fputc('*',fp_clean);
        printf("%c",'*');
    }
  	fseek(fipo, 0, SEEK_SET);
  	fseek(fp_clean, 0, SEEK_SET);
    //fclose(fipo);
    //fclose(fp_clean);

}


//-----------------MAIN MODULE FOR LEXICAL ANALYSIS: RETURNS A TOKEN-------------------------
Token* getNextToken(){

	char token_name[25];
	char token_value[25];
	int y;
	int pointer=0;

	for(y=0;y<25;y++){
		token_name[y]='\0';
		token_value[y]='\0';
	}

	int state=0;char ch;

	while(1){

		ch=fileBuf[end];

		switch (state) {
					//case 0 begins
					case 0:{
							//printf("\n case 0 %d %d %d %d %c\n", start, end,fileBufInd,endInd, fileBuf[start]); //REMOVE
								if(ch==' ' || ch =='\t'){  // HANDLES SPACES AND TABS
									end++;
									start=end;
									continue;
								}
								else if(ch=='\n'){ //HANDLES NEW LINES AND INCREMENTS THE LINENO
									end++;
									start=end;
									lineno++;
									continue;
								}
								else if(ch=='+'){
									total_tok++;
									Token* x=createToken("+","PLUS",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='$'){
									total_tok++;
									Token* x=createToken("$","DOLLAR",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='-'){
									total_tok++;
									Token* x=createToken("-","MINUS",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='/'){
									total_tok++;
									Token* x=createToken("/","DIV",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch==';'){
									total_tok++;
									Token* x=createToken(";","SEMICOL",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch==','){
									total_tok++;
									Token* x=createToken(",","COMMA",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='['){
									total_tok++;
									Token* x=createToken("[","SQBO",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch==']'){
									total_tok++;
									Token* x=createToken("]","SQBC",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='('){
									total_tok++;
									Token* x=createToken("(","BO",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch==')'){
									total_tok++;
									Token* x=createToken(")","BC",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='<'){
									total_tok++;
									end++;
									state=1;
									continue;
								}
								else if(ch=='>'){
									total_tok++;
									end++;
									state=2;
									continue;
								}
								else if(ch=='=' && fileBuf[end+1]=='='){
									total_tok++;
									Token* x=createToken("==","EQ",lineno);
									end=end+2;
									start=end;
									return x;
								}
								else if(ch=='!' && fileBuf[end+1]=='='){
									total_tok++;
									Token* x=createToken("!=","NE",lineno);
									end=end+2;
									start=end;
									return x;
								}
								else if(ch==':'){
									total_tok++;
									end++;
									state=3;
									continue;
								}
								else if(ch=='*'){
									total_tok++;
									end++;
									state=4;
									continue;
								}
								else if(checkForAlpha(ch)){
									end++;
									state=5;
									token_value[pointer++]=ch;
									continue;
								}
								else if(checkFordigit(ch)){
									end++;
									state=6;
									token_value[pointer++]=ch;
									continue;
								}
								else if(ch=='.' && fileBuf[end+1]=='.'){
									end=end+2;
									start=end;
									Token* x=createToken("..","RANGEOP",lineno);
									return x;
								}
								else{
									printf("Lexer.c.%d(LEXICAL ERROR): Lexer could not recognise the input(invalid input)\n",lineno);
									end++;
									state=0;
									return NULL;
								}

							}
							//case 0 ends


					//case 1 begins
					case 1:{
								if(ch=='='){
									Token* x=createToken("<=","LE",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='<' && fileBuf[end+1]!='<'){
									Token* x=createToken("<<","DEF",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='<' && fileBuf[end+1]=='<'){
									Token* x=createToken("<<<","DRIVERDEF",lineno);
									end=end+2;
									start=end;
									return x;
								}
								else {Token* x=createToken("<","LT",lineno);start=end;return x;}
							}
							//case 1 ends


					//case 2 1begins
					case 2:{
								if(ch=='='){
									Token* x=createToken(">=","GE",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='>' && fileBuf[end+1]!='>'){
									Token* x=createToken(">>","ENDDEF",lineno);
									end++;
									start=end;
									return x;
								}
								else if(ch=='>' && fileBuf[end+1]=='>'){
									Token* x=createToken(">>>","DRIVERENDDEF",lineno);
									end=end+2;
									start=end;
									return x;
								}
								else {
									Token* x=createToken(">","GT",lineno);
									start=end;
									return x;
								}
							}
							//case 2 ends


					//case 3 begins
					case 3:{
								if(ch=='='){
									Token* x=createToken(":=","ASSIGNOP",lineno);
									end++;
									start=end;
									return x;
								}
								else {
									Token* x=createToken(":","COLON",lineno);
									start=end;
									return x;
								}
							}
							//case 3 ends


					//case 4 begins
					case 4:{    // REMOVING COMMENTS
								if(ch=='*'){
									end++;
									while(!(fileBuf[end]=='*' && fileBuf[end+1]=='*')){
										if(fileBuf[end]=='\n'){lineno++;}
										if(end==endInd && !feof(fp))
											endInd=fillFileBuf(fp);/*call fileBufrefill everytime*/
										else if(end==endInd && feof(fp)){
											printf("Lexer.c.%d(LEXICAL ERROR): End of file, COMMENT didn't end\n",lineno);
											return NULL;
										}
										end++;
									}

									//printf("comment %d %c", end, fileBuf[end]);
									end=end+2;
									state=0;
									start=end;
									Token* x=createToken("","COMMENT",lineno);
									return x;
								}
								else{
									Token* x=createToken("*","MUL",lineno);
									start=end;
									return x;
								}
							}
							//case 4 ends


					//case 5 begins
					case 5:{
								if((checkForAlpha(ch) || ch=='_' || checkFordigit(ch)) && pointer<20){
									token_value[pointer++]=ch;
									end++;
									state=5;
									continue;
								}
								else if(pointer>=20 && (checkForAlpha(ch) || ch=='_' || checkFordigit(ch))){
									lenflag=1;
									//printf("LEXER.c--Error--exceeded max length of ID");
									state=5;
									end++;
									//return NULL;
									break;
								}
								else if(lenflag==1 && pointer>=20){
									printf("Lexer.c.%d(LEXICAL ERROR): Exceeded max length(20) of identifer \n",lineno);
									state=0;
									start=end;
									lenflag=0;
									return NULL;
									continue;
								}
								else{
									char *name_k;
									char* val_k;
									if((name_k=getRecordFromKeyMap(token_value,keyMapSize))){
										Token* x=createToken("",name_k,lineno);
										start=end;
										state=0;
										int g;

										for(g=0;g<pointer;g++){
											x->tokenLexeme[g]=token_value[g];
										}
										x->tokenLexeme[g]='\0';

										return x;
									}

									else{
										//puts("check");
										Token* x=createToken("","ID",lineno);start=end;state=0;
										int g=0;

										for(g=0;g<pointer;g++){
											x->tokenLexeme[g]=token_value[g];
										}
										x->tokenLexeme[g]='\0';

										return x;
									}


								}


							}
							//case 5 ends


					//case 6 begins
					case 6:{
								if(checkFordigit(ch)){
									token_value[pointer++]=ch;
									end++;
									state=6;
									continue;
								}
								else if(ch=='.' && fileBuf[end+1]!='.'){
				 					token_value[pointer++]=ch;

				 					if(checkFordigit(fileBuf[end+1])){
				 						token_value[pointer++]=fileBuf[end+1];
				 						end=end+2;
				 						state=7;
				 						continue;
									}
			 						else{
			 							printf("Lexer.c.%d(LEXICAL ERROR): Digit absent after decimal\n",lineno);
			 							state=0;
			 							return NULL;
			 						}
								}
								else if(ch=='.' && fileBuf[end+1]=='.'){
									start=end;
									Token* x=createToken("","NUM",lineno);
									int g=0;
									state=0;

									for(g=0;g<pointer;g++){
										x->tokenLexeme[g]=token_value[g];
									}
									x->tokenLexeme[g]='\0';

									return x;
								}
								else{
									Token* x=createToken("","NUM",lineno);
									start=end;
									int g=0;
									state=0;

									for(g=0;g<pointer;g++){
										x->tokenLexeme[g]=token_value[g];
									}
									x->tokenLexeme[g]='\0';

									return x;
								}


							}
							//case 6 ends


					//case 7 begins
					case 7:{
								if(checkFordigit(ch)){
									token_value[pointer++]=ch;
									end++;
									state=7;
									continue;
								}
								else if(ch=='E' || ch=='e'){

									if(fileBuf[end+1]=='+' || fileBuf[end+1]=='-' || checkFordigit(fileBuf[end+1])){
										token_value[pointer++]=ch;
										end++;
										state=7;
										continue;
									}
									else{
										Token* x=createToken("","RNUM",lineno);
										start=end;
				                     	int g=0;
				                     	state=0;

				                     	for(g=0;g<pointer;g++){
					                 		x->tokenLexeme[g]=token_value[g];
										}
				           				x->tokenLexeme[g]='\0';

				           				return x;
				           			}

								}
								else if((ch=='+' || ch=='-')){
									if((fileBuf[end-1]=='E'|| fileBuf[end-1]=='e')){
										if(checkFordigit(fileBuf[end+1])){
											token_value[pointer++]=ch;
											end++;
											state=7;
											continue;
										}
										else{
				          					Token* x=createToken("","RNUM",lineno);end=end-1;start=end;state=0;
				          					int g=0;

				          					for(g=0;g<pointer-1;g++){
				          						x->tokenLexeme[g]=token_value[g];
				          					}
					      					x->tokenLexeme[g]='\0';

				          					return x;
										}
									}
									else{
										Token* x=createToken("","RNUM",lineno);
										start=end;
										int g=0;
										state=0;

										for(g=0;g<pointer;g++){
											x->tokenLexeme[g]=token_value[g];
										}
										x->tokenLexeme[g]='\0';

										return x;
									}


								}
								else{
									Token* x=createToken("","RNUM",lineno);
									start=end;
									int g=0;
									state=0;

									for(g=0;g<pointer;g++){
										x->tokenLexeme[g]=token_value[g];
									}
									x->tokenLexeme[g]='\0';

									return x;
								}

							}
							//case 7 ends

						}//SWITCH ENDS

			}//WHILE ENDS

}// GET TOKEN ENDS




//MODULE TO REFILL THE FILE BUFFER
int fillFileBuf(FILE *fp){

	if(fp==NULL){
		printf("File does not exist; \n");
		exit(0);
	}
	int endInd;
	endInd = fread(fileBuf, 1, fileBufLen, fp);
	fileBuf[endInd]='\0';
	//printf("las : %c. %c\n", fileBuf[endInd], fileBuf[endInd-1]);
	fileBufInd=0;
	return endInd;
}

//MODULE TO LOAD TH FILE BUFFER FROM THE THE FILE INPUT STREAM
FILE* getStream(FILE *filep){

	//printf("file\n");
	fp = filep;
	if(!feof(fp) && strcmp(fileBuf,"")==0)
		endInd = fillFileBuf(fp);

	while(!feof(fp)){
		while(fileBufLen-fileBufInd>=maxTokLen){
			token = getNextToken();
			if(token!=NULL){
					//puts(token->tokenName);
					//puts(token->tokenLexeme);
					return fp;
			}
			//printf("%d\n", end);
		}

		fseek(fp, fileBufInd-fileBufLen, SEEK_CUR);
		endInd = fillFileBuf(fp);
	}
	while(fileBufInd<endInd){
		token = getNextToken();
			if(token!=NULL){
				//puts(token->tokenName);
				//puts(token->tokenLexeme);
				return fp;
			}
		//printf("%d\n", fileBufInd);
	}
    return fp;
	//printf("%d %d", fileBufInd, endInd);
}

void addDollar(char * str){
	FILE * ft1=fopen(str,"a+");
	if(ft1 == NULL){
		printf("Unable to open test case file\n");
		exit(0);
	}
	fseek(ft1,-1,SEEK_END);
	char c=fgetc(ft1);
	if(c!='$'){
		fseek(ft1,0,SEEK_END);
		fputc('$',ft1);
	}
	//fseek(ft1,0,SEEK_SET);
	fclose(ft1);
}

void lexicalAnalysis(FILE* fp1){
	fp=fp1;
	printf("\nLine Number\tLexeme\t\tToken Name\t\n");
	if(!feof(fp) && strcmp(fileBuf,"")==0)
		endInd = fillFileBuf(fp);

	while(!feof(fp)){
		while(fileBufLen-fileBufInd>=maxTokLen){
			token = getNextToken();
			//if(token!=NULL){
			if(token!=NULL && strcmp(token->tokenName,"COMMENT")!=0){
					printf("%d\t\t%s\t\t%s\t\n", token->lineNumber,token->tokenLexeme,token->tokenName);
					//return fp;
			}
			if(fileBufInd>endInd-1&&feof(fp)) break;
		}
		if(fileBufInd>endInd-1&&feof(fp)) break;
		fseek(fp, fileBufInd-fileBufLen, SEEK_CUR);
		endInd = fillFileBuf(fp);
	}
	//printf("chill %d %d\n", fileBufInd, endInd);
	while(fileBufInd<=endInd-1){
		//printf("%d %d\n", fileBufInd, endInd);
		token = getNextToken();
		//printf("%d %d\n", fileBufInd, endInd);
			//if(token!=NULL){
			if(token!=NULL && strcmp(token->tokenName,"COMMENT")!=0){
				printf("%d\t\t%s\t\t%s\t\n", token->lineNumber,token->tokenLexeme,token->tokenName);
				//puts(token->tokenLexeme);
				//return fp;
			}
		//printf("%d\n", fileBufInd);
	}
	fseek(fp, 0, SEEK_SET);
	endInd=0;
	fileBufInd=0;
	strcpy(fileBuf,"");

}
