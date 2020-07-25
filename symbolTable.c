//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
//#include "ast_1.c"

SymbolTable * ST;
FuncTable * FT;
int offset;
char prevScope[100];
//int nestingLevel;


SymTableNode* createSymTableNode(char * token, char * scope, char * type, int isArray, int value, int width, int in_out_var, int isAssigned, int lineNumber, int offset){
	SymTableNode* newNode=(SymTableNode *)malloc(sizeof(SymTableNode));
	strcpy(newNode->tokenName,token);
	strcpy(newNode->scope,scope);
	strcpy(newNode->type,type);
	strcpy(newNode->ifArrayRange,"---");
	newNode->isArray=isArray;
	if(!isArray)
		newNode->value=value;
	//newNode->next=NULL;
	newNode->offset=offset;
	newNode->width=width;
	newNode->in_out_var=in_out_var;
	newNode->isAssigned=isAssigned;
	newNode->lineNumber=lineNumber;
	newNode->nestingLevel=1;
	//newNode->ifArrayRange=0;
	newNode->ifArrayDynamic=0;
	return newNode;
}

FuncTableNode * createFuncTableNode(char * fname, char ** inT, char ** outT,char **inP, char ** outP,int numIn, int numOut, int line,int isModule, int numChild){
	FuncTableNode * newNode=(FuncTableNode *)malloc(sizeof(FuncTableNode));
	strcpy(newNode->funcName,fname);
	for(int i=0;i<numIn;i++){
		strcpy(newNode->inputType[i],inT[i]);
		strcpy(newNode->inputParam[i],inP[i]);
	}
	for(int i=0;i<numOut;i++){
		strcpy(newNode->outputType[i],outT[i]);
		strcpy(newNode->outputParam[i],outP[i]);
	}
	newNode->isDeclared=0;
	newNode->numInputParam=numIn;
	newNode->numOutputParam=numOut;
	newNode->line=line;
	newNode->isModule=isModule;
	newNode->numChild=0;
	newNode->isDefined=0;
	newNode->startScope=0;
	newNode->endScope=0;
	newNode->parent=NULL;
	newNode->iferror=1;
	return newNode;

}

void initializeTables(){
    int i;
	offset=0;
	strcpy(prevScope,"");
    FT=(FuncTable *)malloc(sizeof(FuncTable));
    ST=(SymbolTable *)malloc(sizeof(SymbolTable));
    for(i=0; i<SIZE_FT_;i++){
        FT->ftable[i]=createFuncTableNode("",NULL,NULL,NULL,NULL,0,0,0,1,0);
    }
    for(i=0;i<SIZE_OUT_;i++){
        ScopeHashTable * outT=(ScopeHashTable * )malloc(sizeof(struct scopeHashTable));
        for(int j=0;j<SIZE_IN_;j++){
            outT->in_table[j]=createSymTableNode("","","",0,0,0,0,0,0,0);
        }
        ST->out_table[i]=outT;
    }
    //    for(i=0; i<SIZE_FT_;i++){
    //        printf("%s ,",FT->ftable[i]->funcName);
    //    }

}

void insertIntoFuncTable(FuncTableNode* ft){
    int index=hashFunction(ft->funcName,SIZE_FT_);
    int i=index;
    //printf("%d", i);
    //printf("\n%s",FT->ftable[i]->funcName);
    while(strcmp(FT->ftable[i]->funcName,"")!=0){
        i=(i+1)%SIZE_FT_;
    }
    FT->ftable[i]=ft;
    //exit(0);
}

int getFuncTableIndex(char *fname){
    int index=hashFunction(fname,SIZE_FT_);
    int i=index;
    while(strcmp(FT->ftable[i]->funcName,fname)!=0){
        i++;
        i=i%SIZE_FT_;
        if(i==index){
            return -1;
        }
    }
    return i;
}
void insertIntoSymTable(SymTableNode * nodeST){
	if(nodeST==NULL)
		return;
    //printf("  %s  ", nodeST->scope);
	int index_out=getFuncTableIndex(nodeST->scope);
    //printf("Index_out %d\n",index_out);
	int index_in= hashFunction(nodeST->tokenName,SIZE_IN_);
	int i=index_in;
    while(strcmp(ST->out_table[index_out]->in_table[i]->tokenName,"")!=0){
        i=(i+1)%SIZE_IN_;
    }
    ST->out_table[index_out]->in_table[i]=nodeST;
}

int getSymTableIndex(char * key, char * fname,int in_out_var){
	int index_out=getFuncTableIndex(fname);
	//printf("IN OUT: %d\n",index_out);
	int index_in= hashFunction(key,SIZE_IN_);
	int i=index_in;
    while(strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)!=0 || ((strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0) && ST->out_table[index_out]->in_table[i]->in_out_var!=in_out_var)){
        i++;
        i=i%SIZE_IN_;
        if(i==index_in)
            return -1;
    }
	return i;
}

//---------Supporting Modules-----------------
int ifFunctionExists(char * fname){
	if(getFuncTableIndex(fname)==-1)
		return 0;
	else
		return 1;
}

int ifSymEntryExists(char * key, char * scope,int in_out_var){
	if(getSymTableIndex(key,scope,in_out_var)==-1)
		return 0;
	else
		return 1;
}

SymTableNode * getSymTableNode(char * id,char * scope, int in_out_var){
	int index_out=getFuncTableIndex(scope);
	int index_in=getSymTableIndex(id,scope,in_out_var);
	if(index_in==-1)
		return NULL;
	else
		return ST->out_table[index_out]->in_table[index_in];
}

FuncTableNode * getFuncTableEntry(char * scope){
	int index=getFuncTableIndex(scope);
	if(index==-1)
		return NULL;
	else
		return FT->ftable[index];

}


SymTableNode* getSNode(char* tokenLexeme, char * scopeT){
    char tempScope[100];
    strcpy(tempScope,scopeT);
    //ASTNode * temp=curr;

	if(ifSymEntryExists(tokenLexeme,tempScope,0)){
        return getSymTableNode(tokenLexeme,tempScope,0);
    }
    else if(ifSymEntryExists(tokenLexeme,tempScope,1)){
        return getSymTableNode(tokenLexeme,tempScope,1);
    }
    else if(ifSymEntryExists(tokenLexeme,tempScope,2)){
        return getSymTableNode(tokenLexeme,tempScope,2);
    }

    while(FT->ftable[getFuncTableIndex(tempScope)]->parent!=NULL){

        strcpy(tempScope,FT->ftable[getFuncTableIndex(tempScope)]->parent->funcName);

        if(ifSymEntryExists(tokenLexeme,tempScope,0)){
            return getSymTableNode(tokenLexeme,tempScope,0);
        }
        else if(ifSymEntryExists(tokenLexeme,tempScope,1)){
            return getSymTableNode(tokenLexeme,tempScope,1);
        }
        else if(ifSymEntryExists(tokenLexeme,tempScope,2)){
            return getSymTableNode(tokenLexeme,tempScope,2);
        }

    }
	return NULL;
}

char * getOuterMostScope(char* tokenLexeme, char * scopeT){
	char tempScope[100];
    strcpy(tempScope,scopeT);
	FuncTableNode * ftemp=getFuncTableEntry(tempScope);
    while(ftemp->parent!=NULL){
		ftemp=ftemp->parent;
        //strcpy(tempScope,FT->ftable[getFuncTableIndex(tempScope)]->parent->funcName);

    }
	return ftemp->funcName;
}



//----------------------------------------------------------------------------------------
//----------------Functions for populating the function table-----------------------------

char * inputT[50];
char * outputT[50];
char * inputP[50];
char * outputP[50];

int extractInput(TreeNode * curr,int iP){

    if(curr==NULL || (!curr->flag &&curr->value==ret)){
        //printf("Return\n");
        return iP;
    }

       // printf("Inside extract Input function\n");
	   //
       // if(curr->flag)
       //     printf("\n %s ", terms[curr->value]);
       // else
       //     printf("\n %s ", nterms[curr->value]);

	iP=extractInput(curr->ASTChild,iP);

    if(curr->flag && curr->value==ID && curr->ASTParent->flag==0 && curr->ASTParent->value==input_plist){
        //printf(" YES ");
		inputP[iP]=(char*)malloc(sizeof(char)*strlen(curr->tokenLexeme));
		strcpy(inputP[iP],curr->tokenLexeme);
        char type[200];
        switch(curr->ASTChild->value){
            case INTEGER:{
                inputT[iP]=(char*)malloc(sizeof(char)*strlen("INTEGER"));
                strcpy(inputT[iP],"INTEGER");
                iP++;
                break;
            }
            case REAL:{
                inputT[iP]=(char*)malloc(sizeof(char)*strlen("REAL"));
                strcpy(inputT[iP],"REAL");
                iP++;
                break;
            }
            case BOOLEAN:{
                inputT[iP]=(char*)malloc(sizeof(char)*strlen("BOOLEAN"));
                strcpy(inputT[iP],"BOOLEAN");
                iP++;
                break;
            }
            case ARRAY:{
				//printf("YES\n");
                TreeNode * c1=curr->ASTChild->ASTChild;
                TreeNode * c2=c1->ASTRight;
				TreeNode * typeArr;
				char bIn[20], bOut[20];
				strcpy(bIn,c1->tokenLexeme);
				strcpy(bOut,c2->tokenLexeme);
                int bound_out;
                int bound_in,size_array;

                bound_in=atoi(c1->tokenLexeme);
                bound_out=atoi(c2->tokenLexeme);

                if(!((bound_in==0 && strcmp(c1->tokenLexeme,"0")!=0) || (bound_out==0 && strcmp(c2->tokenLexeme,"0")!=0))) {
					size_array=bound_out-bound_in+1;
					// printf("YES\n");
					//return iP;
                }
                switch(c2->ASTRight->value){
                    case INTEGER:
                        sprintf(type,"ARRAY(INTEGER,%s,%s)", bIn,bOut);
                        break;
                    case REAL:
                        sprintf(type,"ARRAY(REAL,%s,%s)", bIn,bOut);
                        break;
                    case BOOLEAN:
                        sprintf(type,"ARRAY(BOOLEAN,%s,%s)", bIn,bOut);
                        break;
                }
				//printf("%s\n",type);
                inputT[iP]=(char*)malloc(sizeof(char)*strlen(type));
                strcpy(inputT[iP],type);
                iP++; //printf("%s Type of input\n",type);
				 //printf("YES\n");
                break;
            }
        }

    }
	iP=extractInput(curr->ASTRight,iP);
	return iP;
}

int extractOutput(TreeNode * curr,int oP){
	if(curr==NULL || (!curr->flag && curr->value==statements))
		return oP;
    //printf("Inside extract Output function\n");
    /*
    if(curr->flag)
        printf("\n %s ", terms[curr->value]);
    else
        printf("\n %s ", nterms[curr->value]);
    */

    oP=extractOutput(curr->ASTChild,oP);

	if(curr->value==ID && curr->ASTChild!=NULL){
		outputP[oP]=(char*)malloc(sizeof(char)*strlen(curr->tokenLexeme));
		strcpy(outputP[oP],curr->tokenLexeme);
		switch(curr->ASTChild->value){
            case INTEGER:{
                outputT[oP]=(char*)malloc(sizeof(char)*strlen("INTEGER"));
				strcpy(outputT[oP],"INTEGER");
                oP++;
				break;
            }
            case REAL:{
                outputT[oP]=(char*)malloc(sizeof(char)*strlen("REAL"));
				strcpy(outputT[oP],"REAL");
                oP++;
				break;
            }
            case BOOLEAN:{
                outputT[oP]=(char*)malloc(sizeof(char)*strlen("BOOLEAN"));
				strcpy(outputT[oP],"BOOLEAN");
                oP++;
                break;
            }
		}
	}
	oP=extractOutput(curr->ASTRight,oP);
	return oP;
}

FuncTableNode *  extractFunctionDetails(TreeNode * mod, char * fname){
	//printf("\nIn Extract Function Details function:\n");
    //printf(" In: %s and Out: %s\n",nterms[mod->ASTRight->value],nterms[mod->ASTRight->ASTRight->value]);
    TreeNode * inputList=mod->ASTRight;
    TreeNode * outputList=inputList->ASTRight;
    int line_,inP=0,outP=0;
    line_=mod->lineNumber;

    //Extract Input and Output Parameter types
    inP=extractInput(inputList,0); //exit(0);printf("YES\n");
    //printf("Number of Input paramters: %d\n",inP);
    outP=extractOutput(outputList,0);
	//for(int k=0;k<outP;k++)
	//	printf("%s\n",output[k]);
    //printf("Number of Output paramters: %d\n",outP);
    FuncTableNode  * fn=createFuncTableNode(fname,inputT,outputT,inputP,outputP,inP,outP,line_,1,0);
    return fn;
}

void addFunction(TreeNode * treeNode, char * fname){
	if(treeNode==NULL)
		return;
	addFunction(treeNode->ASTChild,fname);

	if(!treeNode->ASTParent->flag && strcmp(nterms[treeNode->ASTParent->value],"module")==0){
        //printf("\nIn Add function: %s ",nterms[treeNode->ASTParent->value]);
		if(strcmp(treeNode->tokenLexeme,fname)==0){
			//printf("%s\n",treeNode->tokenLexeme);
			if(getFuncTableIndex(treeNode->tokenLexeme)==-1){
				//printf("Inside %s\n",treeNode->tokenLexeme);
				FuncTableNode * fnode= extractFunctionDetails(treeNode,fname);
				insertIntoFuncTable(fnode);
				return;
			}

			//fnode->isDefined=1;

            //printf("Added into f table:");
			return;
		}
	}
	addFunction(treeNode->ASTRight,fname);
}
//----------Function for handling the input and output param of functions-------------------------

void insertAndExtractIDParams(TreeNode * curr, char * scope){
	if(curr== NULL)
		return;
    //printf("Inside Function Extract Parameters\n");

     // if(curr->flag)
     //     printf("\n %s ", curr->tokenLexeme);
     // else
     //     printf("\n %s ", nterms[curr->value]);

    switch(curr->ASTChild->value){
        case INTEGER:{
            SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"INTEGER",0,1,2,1,0,curr->lineNumber,offset);
			newEntry->nestingLevel=0;
            insertIntoSymTable(newEntry);
            offset+=2;
            break;
        }
        case REAL:{
            SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"REAL",0,1,4,1,0,curr->lineNumber,offset);
            newEntry->nestingLevel=0;
			insertIntoSymTable(newEntry);
            offset+=4;
            break;
        }
        case BOOLEAN:{
            SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"BOOLEAN",0,1,1,1,0,curr->lineNumber,offset);
            newEntry->nestingLevel=0;
			insertIntoSymTable(newEntry);
            offset+=1;
            break;
        }
        case ARRAY:{
            //printf("Extracting array\n");
            char type[50];
            TreeNode * c1= (curr->ASTChild)->ASTChild;
            TreeNode * c2=c1->ASTRight;
            // print out statements
            // int bound_in=atoi(c1->tokenLexeme);
            // int bound_out=atoi(c2->tokenLexeme);
            // if((bound_in==0 && strcmp(c2->tokenLexeme,"0")!=0) || (bound_out==0 && strcmp(c1->tokenLexeme,"0")!=0)){
            //     return;
            // }
            // int size_array=bound_out-bound_in+1;
            // int width;
            // switch(c2->ASTRight->value){
            //     case INTEGER:
            //         width=2;
            //         sprintf(type,"ARRAY(INTEGER,%d,%d)", bound_in,bound_out);
            //         break;
            //     case REAL:
            //         width=4;
            //         sprintf(type,"ARRAY(REAL,%d,%d)", bound_in,bound_out);
            //         break;
            //     case BOOLEAN:
            //         width=1;
            //         sprintf(type,"ARRAY(BOOLEAN,%d,%d)", bound_in,bound_out);
            //         break;
            // }
			char bIn[20], bOut[20];
			strcpy(bIn,c1->tokenLexeme);
			strcpy(bOut,c2->tokenLexeme);
			int bound_out;
			int bound_in,size_array,width;

            bound_in=atoi(c1->tokenLexeme);
            bound_out=atoi(c2->tokenLexeme);
			int ifDynamic=0;
			if(!((bound_in==0 && strcmp(c1->tokenLexeme,"0")!=0) || (bound_out==0 && strcmp(c2->tokenLexeme,"0")!=0))) {

				size_array=bound_out-bound_in+1;
				//return iP;
			}
			switch(c2->ASTRight->value){
				case INTEGER:
					//width=2;
					sprintf(type,"ARRAY(INTEGER,%s,%s)", bIn,bOut);
					break;
				case REAL:
					//width=4;
					sprintf(type,"ARRAY(REAL,%s,%s)", bIn,bOut);
					break;
				case BOOLEAN:
					//width=1;
					sprintf(type,"ARRAY(BOOLEAN,%s,%s)", bIn,bOut);
					break;
			}
			width=5;
			if(!((bound_in==0 && strcmp(c2->tokenLexeme,"0")!=0) || (bound_out==0 && strcmp(c1->tokenLexeme,"0")!=0))) {
				//width=width*size_array+1;
			}
			else{

				ifDynamic=1;
			}
            SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,type,1,0,width,1,0,curr->lineNumber,offset);
			newEntry->nestingLevel=0;
            newEntry->offset=offset;
			newEntry->ifArrayDynamic=ifDynamic;
			sprintf(newEntry->ifArrayRange,"[%s, %s]",bIn,bOut);
            //printf("\n%s",type);
            insertIntoSymTable(newEntry);
            offset+=width;
            break;
        }
    }
}

void buildSymbolTable(TreeNode * astTree, char * scope, int nestingLevel){

	if(astTree==NULL)
        return;

		// if(astTree->flag)
	    //     printf("\n %s , %d :", terms[astTree->value],astTree->lineNumber);
	    // else
	    //     printf("\n %s , %d :", nterms[astTree->value],astTree->lineNumber);
		//
		// printf("Scope %s\n",scope);


    if(!astTree->flag && strcmp(nterms[astTree->value],"driverModule")==0){
    	//printf("Inside Driver Module\n");
        FuncTableNode * init=createFuncTableNode("driver",NULL,NULL,NULL,NULL,0,0,astTree->lineNumber,1,0);
		init->parent=NULL;
		int start=astTree->child->right->right->right->right->child->lineNumber;
		int end=astTree->child->right->right->right->right->child->right->right->lineNumber;
		//printf("start and end %d, %d\n",start,end);
		//init->startScope=astTree->lineNumber;startScope=start;
		init->startScope=start;
		init->endScope=end;
        insertIntoFuncTable(init);
        strcpy(scope,"driver");
		nestingLevel= 1;
		offset=0;
		//buildSymbolTable(astTree->ASTChild, scope, nestingLevel);
    }

	buildSymbolTable(astTree->ASTChild, scope, nestingLevel);

    // printf("Inside Build Symbol Table\n");

	TreeNode * curr= astTree;

    if(curr->flag && curr->value==ID){

		//printf("\n %s , %d :", curr->tokenLexeme,curr->lineNumber);
		//printf("\n AST Parent%s , %d , %d :", nterms[curr->ASTParent->value],curr->lineNumber,curr->ASTParent->flag);
		//printf("\n Parse Tree Parent %s , %d , %d :", nterms[curr->parent->value],curr->lineNumber,curr->parent->flag);
		if(curr->ASTParent!=NULL){ /// make sure that the ASTParent is a non terminal
			switch(curr->ASTParent->value){

				// ASTParent is moduleDeclaration
                case moduleDeclarations:{
                    //printf("Inside Module Declarations case\n");
					//printf("\n%s Parent %s Scope %s \n",curr->tokenLexeme, nterms[curr->ASTParent->value],scope);
                    //printf("%d ",getFuncTableIndex(curr->tokenLexeme));
                    if(getFuncTableIndex(curr->tokenLexeme)==-1){
						//printf("%s \n",curr->tokenLexeme);
						offset=0;
						addFunction(curr->ASTParent,curr->tokenLexeme);
						int i=getFuncTableIndex(curr->tokenLexeme);
						FuncTableNode * fnode;
						if(i == -1){
							//printf("YES");
							//printf("%s \n",curr->tokenLexeme);
							fnode= createFuncTableNode(curr->tokenLexeme,NULL,NULL,NULL,NULL,0,0,0,1,0);
							fnode->isDefined=0;
							fnode->isDeclared=1;
							insertIntoFuncTable(fnode);
						}
						else{
							fnode=FT->ftable[i];
							fnode->isDeclared=1;
						}

                    }
					// add when fucntion is declared but not defined
					else{
                        sterr=1;
						printf("Line(%d). Function %s is already declared. Redeclaration is not allowed\n",curr->lineNumber,curr->tokenLexeme);
                    }

					break;
                }

				// ASTParent is module
                case module:{
					    //printf("\n%s Parent %s Scope %s \n",curr->tokenLexeme, nterms[curr->ASTParent->value],scope);
					int start=curr->ASTParent->ASTChild->ASTRight->ASTRight->right->child->lineNumber;
					int end=curr->ASTParent->ASTChild->ASTRight->ASTRight->right->child->right->right->lineNumber;
					//printf("start and end %d, %d\n",start,end);
					//init->startScope=astTree->lineNumber;startScope=start;
					int dec=0;
					//*nestingLevel=1;
					if(getFuncTableIndex(curr->tokenLexeme)==-1){
						//printf("YES1");
						//printf("%s \n",curr->tokenLexeme);
						//printf("%d \n",getFuncTableIndex(curr->tokenLexeme));
						offset=0;
						addFunction(curr,curr->tokenLexeme);
					}
					else{
						if(getFuncTableEntry(curr->tokenLexeme)->isDefined){
                            sterr=1;
							printf("Line(%d). Function %s is overloading. Function Overloading is not allowed\n",curr->lineNumber,curr->tokenLexeme);
							//uildSymbolTable(curr->ASTParent->ASTRight,scope, nestingLevel);
							//while()
							//printf(" %s \n",nterms[curr->ASTParent->value]);
							return;//curr=curr->ASTParent;
							break;
							//return;
						}
						dec=1;
					}
					int i=getFuncTableIndex(curr->tokenLexeme);
					FuncTableNode * fnode=FT->ftable[i];
					fnode->isDefined=1;
					fnode->isDeclared=dec;
					fnode->startScope=start;
					fnode->endScope=end;
					strcpy(scope,curr->tokenLexeme);
					nestingLevel=1;
					break;
                }

				// ASTParent is input parameter list for a function
                case input_plist:{
					    //printf("\n%s Parent %s Scope %s \n",curr->tokenLexeme, nterms[curr->ASTParent->value],scope);
					if(ifSymEntryExists(curr->tokenLexeme,scope,1)){
                        sterr=1;
						printf("Line(%d). %s is an already defined input variable for function %s. Redefinition is not allowed\n",curr->lineNumber,curr->tokenLexeme,scope);
					}
                    else{
						//offset=0;
						insertAndExtractIDParams(curr,scope);
					}
					if(curr->ASTRight==NULL && curr->ASTParent->ASTRight->ASTChild==NULL)
						offset=0;
                    break;
                }

				// ASTParent is output parameter list for the function
                case ret:{
					    //printf("\n%s Parent %s Scope %s \n",curr->tokenLexeme, nterms[curr->ASTParent->value],scope);
					if(ifSymEntryExists(curr->tokenLexeme,scope,2)){
                        sterr=1;
						printf("Line(%d). %s is an already defined output variable for function %s. Redefinition is not allowed\n",curr->lineNumber,curr->tokenLexeme,scope);
						break;
					}
					switch(curr->ASTChild->value){
						case INTEGER:{
							SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"INTEGER",0,1,2,2,0,curr->lineNumber,offset);
							newEntry->nestingLevel=0;
							insertIntoSymTable(newEntry);
							offset+=2;
							break;
						}
						case REAL:{
							SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"REAL",0,1,4,2,0,curr->lineNumber,offset);
							newEntry->nestingLevel=0;
							insertIntoSymTable(newEntry);
							offset+=4;
							break;
						}
						case BOOLEAN:{
							SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"BOOLEAN",0,1,1,2,0,curr->lineNumber,offset);
							newEntry->nestingLevel=0;
							insertIntoSymTable(newEntry);
							offset+=1;
							break;
						}
					}
					if(curr->ASTRight==NULL)
						offset=0;
					break;
                }

				case moduleReuseStmt:{
					FuncTableNode * fnode=getFuncTableEntry(curr->tokenLexeme);
					//printf("YES");
					//printf("%s yy",curr->tokenLexeme);
					if(fnode!=NULL){
						//printf("YES2");
						//printf(" %d and %d \n",fnode->isDeclared,fnode->isDefined);
						if(fnode->isDeclared && fnode->isDefined && fnode->iferror){
                            sterr=1;
							printf("Line(%d). Function %s has redundant definition and declaration\n",fnode->line,curr->tokenLexeme);
						}
						else if(fnode->isDeclared && !fnode->isDefined && fnode->iferror){
							fnode->iferror=0;
						}
					}
					else{
                        sterr=1;
						printf("Line(%d). Function %s is not declared\n",curr->lineNumber,curr->tokenLexeme);
                    }

					break;
				}

				// ASTParent is the idList for declare statements
                case idList:{
					//printf("\n%s Parent %s Scope %s \n",curr->tokenLexeme, nterms[curr->ASTParent->value],scope);
						//if(curr->ASTChild!=NULL)
						//printf("%s  \n",terms[curr->ASTParent->ASTRight->value]);
					//printf("\nInside idList case\n");
					//printf("%s ",scope);
					// DECLARE STATEMENTS
					if(curr->ASTParent->ASTChild!=NULL && curr->ASTParent->ASTRight!=NULL){// idList in case of reuse statements

						//if(ifSymEntryExists(curr->tokenLexeme,scope,0))
						//if(getFuncTableEntry(scope)->isModule && (ifSymEntryExists(curr->tokenLexeme,scope,1)||ifSymEntryExists(curr->tokenLexeme,scope,2)));
						//printf("%d index\n",getSymTableIndex(curr->tokenLexeme,scope,0));
				 		if(ifSymEntryExists(curr->tokenLexeme,scope,0)){
                            sterr=1;
							printf("Line(%d). Variable %s is already declared in the scope ""%s"". Redefinition is not allowed\n",curr->lineNumber,curr->tokenLexeme, scope);
                        }
						else{
							//printf("YES");
							if(curr== NULL)
								return;
							//printf("%s hh", terms[curr->ASTParent->ASTRight->value]);
							switch(curr->ASTParent->ASTRight->value){
								case INTEGER:{
									//printf("YES");
									SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"INTEGER",0,1,2,0,0,curr->lineNumber,offset);
									newEntry->nestingLevel=nestingLevel;
									insertIntoSymTable(newEntry);
									offset+=2;
									break;
								}
								case REAL:{
									SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"REAL",0,1,4,0,0,curr->lineNumber,offset);
									newEntry->nestingLevel=nestingLevel;
									insertIntoSymTable(newEntry);
									offset+=4;
									break;
								}
								case BOOLEAN:{
									SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,"BOOLEAN",0,1,1,0,0,curr->lineNumber,offset);
									newEntry->nestingLevel=nestingLevel;
									insertIntoSymTable(newEntry);
									offset+=1;
									break;
								}

								case ARRAY:{
									char type[50];
									TreeNode * c1= (curr->ASTParent->ASTRight->ASTChild);
									TreeNode * c2=c1->ASTRight;

									char bIn[20], bOut[20];
									strcpy(bIn,c1->tokenLexeme);
									strcpy(bOut,c2->tokenLexeme);
									int bound_out;
									int bound_in,size_array,width;

                                    bound_in=atoi(c1->tokenLexeme);
                                    bound_out=atoi(c2->tokenLexeme);

									if(!((bound_in==0 && strcmp(c1->tokenLexeme,"0")!=0) || (bound_out==0 && strcmp(c2->tokenLexeme,"0")!=0))) {

										size_array=bound_out-bound_in+1;
										//return iP;
									}
									switch(c2->ASTRight->value){
										case INTEGER:
											width=2;
											sprintf(type,"ARRAY(INTEGER,%s,%s)", bIn,bOut);
											break;
										case REAL:
											width=4;
											sprintf(type,"ARRAY(REAL,%s,%s)", bIn,bOut);
											break;
										case BOOLEAN:
											width=1;
											sprintf(type,"ARRAY(BOOLEAN,%s,%s)", bIn,bOut);
											break;
									}
									int ifDynamic=0;
									if(!((bound_in==0 && strcmp(c2->tokenLexeme,"0")!=0) || (bound_out==0 && strcmp(c1->tokenLexeme,"0")!=0))) {
										width=width*size_array+1;
									}
									else{
										width=1;
										ifDynamic=1;
									}
									SymTableNode * newEntry= createSymTableNode(curr->tokenLexeme, scope,type,1,0,width,0,0,curr->lineNumber,offset);
									newEntry->nestingLevel=nestingLevel;
									newEntry->offset=offset;
									newEntry->ifArrayDynamic=ifDynamic;
									sprintf(newEntry->ifArrayRange,"[%s, %s]",bIn,bOut);
									//printf("\n%s",type);
									insertIntoSymTable(newEntry);
									offset+=width;
									break;
								}
							}
						}
						break;
					}
					break;
				}
				default:{
					//printf("here\n");
					char tempScope[100];
					strcpy(tempScope,scope);
					//ASTNode * temp=curr;

					while(FT->ftable[getFuncTableIndex(tempScope)]->parent!=NULL){
						//printf("YESSS");

						//printf("%s, %d, %d \n",curr->tokenLexeme,ifSymEntryExists(curr->tokenLexeme,tempScope,0),FT->ftable[getFuncTableIndex(tempScope)]->isModule);

						if(ifSymEntryExists(curr->tokenLexeme,tempScope,1) || ifSymEntryExists(curr->tokenLexeme,tempScope,0) || ifSymEntryExists(curr->tokenLexeme,tempScope,2))
							break;
						strcpy(tempScope,FT->ftable[getFuncTableIndex(tempScope)]->parent->funcName);
						// if(!ifSymEntryExists(curr->tokenLexeme,tempScope,0) && !(FT->ftable[getFuncTableIndex(tempScope)]->isModule)){
						// 	strcpy(tempScope,FT->ftable[getFuncTableIndex(tempScope)]->parent->funcName);
						// }
						// if(FT->ftable[getFuncTableIndex(tempScope)]->isModule && !ifSymEntryExists(curr->tokenLexeme,tempScope,0) && !ifSymEntryExists(curr->tokenLexeme,tempScope,1) && !ifSymEntryExists(curr->tokenLexeme,tempScope,2) ){
						// 	printf("YESSS");
						// 	strcpy(tempScope,FT->ftable[getFuncTableIndex(tempScope)]->parent->funcName);
						// }

					}

					if(!ifSymEntryExists(curr->tokenLexeme,tempScope,0) && !ifSymEntryExists(curr->tokenLexeme,tempScope,1) && !ifSymEntryExists(curr->tokenLexeme,tempScope,2)){
						sterr=1;
						if(curr->ASTParent->flag && curr->ASTParent->value==ARRAY && curr->ASTParent->ASTParent->ASTParent!=NULL && curr->ASTParent->ASTParent->ASTParent->flag==0 && curr->ASTParent->ASTParent->ASTParent->value==input_plist);
                        else
							printf("Line(%d). Variable %s is not declared\n",curr->lineNumber,curr->tokenLexeme);
                    }

					break;
				}


				// case moduleReuseStmt:{
				// 	if(curr->ASTRight->value==idList){
				// 		if(ifFunctionExists(curr->tokenLexeme)==0)
				// 			printf("SymbolTable.%d Function %s is not declared.",curr->lineNumber,curr->tokenLexeme);
				// 		else{
				// 			if(!FT->ftable[getFuncTableIndex(curr->tokenLexeme)]->isDefined)
				// 			printf("SymbolTable.%d Function %s definition not found.",curr->lineNumber,curr->tokenLexeme);
				// 		}
				// 	}
				// 	break;
				// }

				// type check for module reuse statements
				// type checking for array index -> should always be an integer.

			}
			// type checking for other cases where head is not ID
		}

		// Add typing checking code;
	}
	//if(!curr->flag && curr->value)
	// handling the for case
	// if(!curr->flag && curr->value==statements){
	//
	// 	printf("Statement Left: %s\n",terms[curr->right->value]);
	//
	// }

	// if(curr->flag==0 && curr->value==statements && curr->ASTParent->flag==0 && curr->ASTParent->value==module){
	// 	printf("YES");
	// 	offset=0;
	// }

	if(curr->flag && curr->value==FOR){
		//printf("%d\n", curr->lineNumber);
		char newscope[50];
		char * index[1];
		sprintf(newscope,"FOR_%d",curr->lineNumber);
		int start=curr->right->right->right->right->right->right->lineNumber;
		int end=curr->right->right->right->right->right->right->right->right->lineNumber;
		//strcpy(prevScope,scope);
		//printf("%s",newscope);
		// if(!ifSymEntryExists(curr->ASTRight->tokenLexeme,scope))
		// 	printf("SymbolTable.%d Variable %s is not already declared in scope %s.",curr->lineNumber,curr->ASTRight->tokenLexeme, scope);
		index[0]=(char*)malloc(sizeof(char)*strlen(curr->ASTRight->tokenLexeme));
		strcpy(index[0],curr->ASTRight->tokenLexeme);
		FuncTableNode * currScope=FT->ftable[getFuncTableIndex(scope)];
		//printf("\n %s, %d  ",currScope->funcName, currScope->numChild);
		FuncTableNode * init=createFuncTableNode(newscope,NULL,NULL,index,NULL,0,0,curr->lineNumber,0,0);
		currScope->children[currScope->numChild]=init;
		currScope->numChild++;
		init->parent=currScope;
		init->startScope=start;
		init->endScope=end;
		//init->startScope=curr->lineNumber;
		//printf("\n %s, %d  ",currScope->children[0]->funcName, currScope->numChild);
		insertIntoFuncTable(init);
		//*nestingLevel= * nestingLevel+1;
		buildSymbolTable(curr->ASTRight,newscope,nestingLevel+1);
		//printf("END: SCOPE %s\n",scope);
		//FuncTableNode * temp=getFuncTableEntry(prevScope);
		//if(temp!=NULL)
		return;
		//strcpy(scope,newscope);
	}
	// change for scope for while statments
	if(curr->flag && curr->value==WHILE){
		//printf("%d\n", curr->lineNumber);
		char newscope[50];
		char * index[1];
		sprintf(newscope,"WHILE_%d",curr->lineNumber);
		int start=curr->right->right->right->right->lineNumber;
		int end=curr->right->right->right->right->right->right->lineNumber;
		//printf("%s",newscope);
		// if(!ifSymEntryExists(curr->ASTRight->tokenLexeme,scope))
		// 	printf("SymbolTable.%d Variable %s is not already declared in scope %s.",curr->lineNumber,curr->ASTRight->tokenLexeme, scope);
		//index[0]=(char*)malloc(sizeof(char)*strlen(curr->ASTRight->tokenLexeme));
		//strcpy(index[0],curr->ASTRight->tokenLexeme);
		FuncTableNode * currScope=FT->ftable[getFuncTableIndex(scope)];
		FuncTableNode * init=createFuncTableNode(newscope,NULL,NULL,NULL,NULL,0,0,curr->lineNumber,0,0);
		currScope->children[currScope->numChild++]=init;
		init->parent=currScope;
		init->startScope=start;
		init->endScope=end;
		//init->startScope=curr->lineNumber;
		insertIntoFuncTable(init);
		//strcpy(scope,newscope);
		//*nestingLevel= * nestingLevel+1;
		buildSymbolTable(curr->ASTRight,newscope,nestingLevel+1);
		return;
	}

	// change of scope of switch statements
	if(curr->flag && curr->value==SWITCH){
		//printf("%d\n", curr->lineNumber);
		char newscope[50];
		char * index[1];
		sprintf(newscope,"SWITCH_%d",curr->lineNumber);
		int start=curr->right->right->right->right->lineNumber;
		int end=curr->right->right->right->right->right->right->right->lineNumber;
		//printf("%s",newscope);
		index[0]=(char*)malloc(sizeof(char)*strlen(curr->ASTRight->tokenLexeme));
		strcpy(index[0],curr->ASTRight->tokenLexeme);
		FuncTableNode * currScope=FT->ftable[getFuncTableIndex(scope)];
		FuncTableNode * init=createFuncTableNode(newscope,index,NULL,NULL,NULL,0,0,curr->lineNumber,0,0);
		currScope->children[currScope->numChild++]=init;
		init->parent=currScope;
		init->startScope=start;
		init->endScope=end;
		//init->startScope=curr->lineNumber;
		insertIntoFuncTable(init);
		//strcpy(scope,newscope);
		//*nestingLevel= * nestingLevel+1;
		buildSymbolTable(curr->ASTRight,newscope,nestingLevel+1);
		//FuncTableNode * curr=getSymTableNode()
		return;

	}


    buildSymbolTable(curr->ASTRight,scope,nestingLevel);
	return;
}

void displayFormat(SymTableNode * stn, char * scopeT){
	char scopeTemp[100],outScope[100];

	strcpy(scopeTemp,scopeT);
	strcpy(outScope,getOuterMostScope(stn->tokenName,scopeTemp));
	int i=getFuncTableIndex(scopeTemp);
	//printf("SCOPE: %s\n",curr->funcName);
	char scopeRange[100];
	sprintf(scopeRange,"%d - %d",FT->ftable[i]->startScope,FT->ftable[i]->endScope);

	char type[100];

	if(stn->isArray){
		//strcpy(isArray,"yes");
		switch(stn->type[6]){
			case 'B':
				strcpy(type,"BOOLEAN"); break;
			case 'I':
				strcpy(type,"INTEGER"); break;
			case 'R':
				strcpy(type,"REAL"); break;
		}
		if(stn->ifArrayDynamic){
			printf("%8s\t%18s\t%10s\t%5s\t%10s\t%10s\t%10s\t%5d\t%5d\t%10d\t\n",stn->tokenName,outScope,scopeRange,"yes","dynamic",stn->ifArrayRange,type,stn->width,stn->offset,stn->nestingLevel);

		}
		else{
			printf("%8s\t%18s\t%10s\t%5s\t%10s\t%10s\t%10s\t%5d\t%5d\t%10d\t\n",stn->tokenName,outScope,scopeRange,"yes","static",stn->ifArrayRange,type,stn->width,stn->offset,stn->nestingLevel);
		}
	}
	else{
		printf("%8s\t%18s\t%10s\t%5s\t%10s\t%10s\t%10s\t%5d\t%5d\t%10d\t\n",stn->tokenName,outScope,scopeRange,"no","---","---",stn->type,stn->width,stn->offset,stn->nestingLevel);

	}
}



void displaySymbolTable(TreeNode * astNode,char * scope){
	if(astNode==NULL)
		return;

	if(!astNode->flag && astNode->value==driverModule){
		strcpy(scope,"driver");
	}

	displaySymbolTable(astNode->ASTChild,scope);

	TreeNode * curr=astNode;

 	if(curr->flag && curr->value==ID && !curr->ASTParent->flag){
	 	if((curr->ASTParent->value==idList && curr->ASTParent->ASTParent->ASTChild->value==DECLARE)){
		 	SymTableNode * stnode=getSymTableNode(curr->tokenLexeme,scope,0);
			if(stnode!=NULL && stnode->lineNumber==curr->lineNumber){
				displayFormat(stnode,scope);
			}
		}
		else if(curr->ASTParent->value==input_plist){
		 	SymTableNode * stnode=getSymTableNode(curr->tokenLexeme,scope,1);
		 	if(stnode!=NULL && stnode->lineNumber==curr->lineNumber){
				displayFormat(stnode,scope);
			}
		}
		else if(curr->ASTParent->value==ret){
			SymTableNode * stnode=getSymTableNode(curr->tokenLexeme,scope,2);
			if(stnode!=NULL && stnode->lineNumber==curr->lineNumber){
				displayFormat(stnode,scope);
			}
		}
		else if(curr->ASTParent->value==module){
			strcpy(scope,curr->tokenLexeme);
		}
 	}

 	if(curr->flag && curr->value==FOR){
		//printf("%d\n", curr->lineNumber);
		char newscope[50];
		sprintf(newscope,"FOR_%d",curr->lineNumber);
		displaySymbolTable(curr->ASTRight,newscope);
		return;
		//strcpy(scope,newscope);
	}
	// change for scope for while statments
	if(curr->flag && curr->value==WHILE){
		char newscope[50];
		sprintf(newscope,"WHILE_%d",curr->lineNumber);
		displaySymbolTable(curr->ASTRight,newscope);
		return;
	}
	// change of scope of switch statements
	if(curr->flag && curr->value==SWITCH){
		char newscope[50];
		sprintf(newscope,"SWITCH_%d",curr->lineNumber);
		displaySymbolTable(curr->ASTRight,newscope);
		return;
	}
 	displaySymbolTable(astNode->ASTRight,scope);
 	return;

}

int getActivationRecSize(FuncTableNode * curr,int width){
	if(curr==NULL)
		return width;

	int i=getFuncTableIndex(curr->funcName);

	for(int j=0;j<SIZE_IN_;j++){
		if(strcmp(ST->out_table[i]->in_table[j]->tokenName,"")!=0){
			//printf("%d\n",ST->out_table[i]->in_table[j]->width);
			width+=ST->out_table[i]->in_table[j]->width;
			//printf("WIDTH: %d\n",width);

		}
	}

	for(int k=0;k<curr->numChild;k++){
		//printf("child %d %s\n",k,curr->children[k]->funcName);
		//printf("WIDTH2: %d\n",width);
		width+=getActivationRecSize(curr->children[k],0);
		//printf("WIDTH3: %d\n",width);
	}
	return width;
}

void getArrayVariables(FuncTableNode * curr ,FuncTableNode * out){
	if(curr==NULL)
		return ;

	int i=getFuncTableIndex(curr->funcName);
	char scopeRange[100];
	sprintf(scopeRange,"%d - %d",out->startScope,out->endScope);
	for(int j=0;j<SIZE_IN_;j++){
		if(strcmp(ST->out_table[i]->in_table[j]->tokenName,"")!=0){
			SymTableNode * stn=ST->out_table[i]->in_table[j];
			if(stn->isArray){
				char type[50];
				switch(stn->type[6]){
					case 'B':
						strcpy(type,"BOOLEAN"); break;
					case 'I':
						strcpy(type,"INTEGER"); break;
					case 'R':
						strcpy(type,"REAL"); break;
				}
				if(stn->ifArrayDynamic){
					printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",out->funcName,stn->tokenName,scopeRange,"dynamic",stn->ifArrayRange,type);

				}
				else{
					printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",out->funcName,stn->tokenName,scopeRange,"static",stn->ifArrayRange,type);

				}
			}
		}
	}

	for(int k=0;k<curr->numChild;k++){
		//printf("child %d %s\n",k,curr->children[k]->funcName);
		getArrayVariables(curr->children[k],out);
	}
	return;
}


void printSymbolTable(TreeNode * astNode, char * scope){
	printf("=============================================================================================================================================\n");
	printf(BOLDWHITE "%70s\n" RESET,"SYMBOL TABLE");
	printf("=============================================================================================================================================\n");
	printf(BOLDGREEN "%8s\t%18s\t%10s\t%5s\t%10s\t%10s\t%10s\t%5s\t%5s\t%10s\t\n" RESET,"VARIABLE NAME","SCOPE","START/END","IS ARR","IS DYNAMIC","RANGE ARRAY","TYPE","WIDTH","OFFSET","NESTING LEVEL");
	printf("=============================================================================================================================================\n");

	displaySymbolTable(astNode,scope);

	printf("=============================================================================================================================================\n");

}

void displayActivationRecord(){

	printf("=====================================\n");
	printf(BOLDWHITE "%30s\n" RESET,"ACTIVATION RECORD SIZE");
	printf("====================================\n");
	printf(BOLDGREEN "%10s\t%10s\n" RESET,"MODULE NAME","SIZE");
	printf("====================================\n");

	for(int i=0;i<SIZE_FT_;i++){
		if(strcmp(FT->ftable[i]->funcName,"")!=0){
			//printf("%s\n",)
			FuncTableNode * curr=FT->ftable[i];
			if(curr->parent==NULL){
				int size=getActivationRecSize(curr,0);
				printf("%10s\t%10d\t\n",curr->funcName,size);
			}
		}
	}
	printf("====================================\n");

}

void displayArrayElements(){
	printf("===============================================================================================\n");
	printf(BOLDWHITE "%50s\n" RESET,"ARRAY ELEMENTS");
	printf("===============================================================================================\n");
	printf(BOLDGREEN "%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n" RESET,"MODULE NAME","VARIABLE NAME","START/END","IS DYNAMIC","RANGE","TYPE");
	printf("===============================================================================================\n");

	for(int i=0;i<SIZE_FT_;i++){
		if(strcmp(FT->ftable[i]->funcName,"")!=0){

			FuncTableNode * curr=FT->ftable[i];
			if(curr->parent==NULL){
				getArrayVariables(curr,curr);
				//printf("%10s\t%10d\t\n",curr->funcName,size);
			}
		}
	}
	printf("===============================================================================================\n");
}
