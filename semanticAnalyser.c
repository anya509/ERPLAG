//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
//#include "typeChecker.c"

#include "semanticAnalyser.h"

void checkRecursion(FuncTableNode* ftnode, char* presscope, int lineNumber){
    int index_out=getFuncTableIndex(presscope);
    if(index_out==-1) { //printf("SA 3 invalid scope searched, no st corrs"); exit(0);
        return;
    }
    FuncTableNode* presftnode = FT->ftable[index_out];
    while(presftnode){
        if(presftnode==ftnode){
            semanticerr=1;
            printf("Line(%d). Recusrive call to module '%s'. \n", lineNumber, ftnode->funcName);
        }
        presftnode = presftnode->parent;
    }
}

void checkForStmts(int lineNumber, char* tofind, TreeNode* stmtsNode, char* scope);
void checkForStmt(int lineNumber, char* tofind, TreeNode* stmtNode, char* scope){
    // DECLARE , PRINT,
    // ASSMT, MODULE RESUSE, GET VALUE
    // WHILE STMT
    if(stmtNode->ASTChild && stmtNode->ASTChild->flag==1 && stmtNode->ASTChild->value==WHILE){
        char newscope[50];
        sprintf(newscope,"WHILE_%d",stmtNode->ASTChild->lineNumber);
        //if(getSymTableNode1(tofind, newscope)==NULL) // if NOT NULL, found a redeclaration of variable, diff var goes by the same name
            checkForStmts(lineNumber, tofind, stmtNode->ASTChild->ASTRight->ASTRight, newscope);
    }
    // FOR STMT
    else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==1 && stmtNode->ASTChild->value==FOR){
        char newscope[50];
        sprintf(newscope,"FOR_%d",stmtNode->ASTChild->lineNumber);
        //if(getSymTableNode1(tofind, newscope)==NULL)
            checkForStmts(lineNumber, tofind, stmtNode->ASTChild->ASTRight->ASTRight->ASTRight, newscope);
    }
    // SWITCH STMT
    else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==1 && stmtNode->ASTChild->value==SWITCH){
        char newscope[50];
        sprintf(newscope,"SWITCH_%d",stmtNode->ASTChild->lineNumber);
        //if(getSymTableNode1(tofind, newscope)==NULL){
            //checkFor(tofind, stmtNode->ASTChild->ASTRight->ASTRight->ASTRight);
            TreeNode* nodeCase = stmtNode->ASTChild->ASTRight->ASTRight->ASTChild;
            while(nodeCase && nodeCase->flag==1 && nodeCase->value==CASE){
                if(nodeCase->ASTRight && nodeCase->ASTRight->ASTRight && nodeCase->ASTRight->ASTRight->flag==0 && nodeCase->ASTRight->ASTRight->value==statements){
                    checkForStmts(lineNumber, tofind, nodeCase->ASTRight->ASTRight, newscope);
                }
                nodeCase = nodeCase->ASTRight->ASTRight->ASTRight;
            }
            TreeNode* nodeDef = stmtNode->ASTChild->ASTRight->ASTRight->ASTRight;
            if(nodeDef && nodeDef->flag==0 && nodeDef->value==default1){
                checkForStmts(lineNumber, tofind, nodeDef->ASTChild, newscope);
            }
        //}
    }
    // ASSIGNMENT STMT
    else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==0 && stmtNode->ASTChild->value==assignmentStmt){
        TreeNode* idnode = stmtNode->ASTChild->ASTChild;
        if(idnode->ASTRight->flag==0 && idnode->ASTRight->value==lvalueIDStmt){
            if(strcmp(idnode->tokenLexeme, tofind)==0){
                char scopenew[50]; strcpy(scopenew, scope);
                SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                if(stnode && stnode->lineNumber == lineNumber){
                    semanticerr=1;
                    printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
                }
            }
        }
    }
    // GET VALUE
    else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==1 && stmtNode->ASTChild->value==GET_VALUE){
        TreeNode* idnode = stmtNode->ASTChild->ASTRight;
        if(strcmp(idnode->tokenLexeme, tofind)==0){
            char scopenew[50]; strcpy(scopenew, scope); 
            SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopenew); strcpy(scopenew, scope);
            if(stnode && stnode->lineNumber == lineNumber){
                semanticerr=1;
                printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
            }
        }
    }
    // MODULE RESUSE
     else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==0 && stmtNode->ASTChild->value==moduleReuseStmt){
        if(stmtNode->ASTChild->ASTChild && stmtNode->ASTChild->ASTChild->ASTChild){
            TreeNode* idnode = stmtNode->ASTChild->ASTChild->ASTChild->ASTChild;
            while(idnode){
                if(strcmp(idnode->tokenLexeme, tofind)==0){
                    char scopenew[50]; strcpy(scopenew, scope);
                    SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                    if(stnode && stnode->lineNumber == lineNumber){
                        semanticerr=1;
                        printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
                    }
                }
                idnode = idnode->ASTRight;
            }
        }
     }
}

void checkForStmts(int lineNumber, char* tofind, TreeNode* stmtsNode, char* scope){
    // ITERATING INDEX OF FOR IS VARIABLE(ID) OF TYPE NUM ONLY
    if(stmtsNode==NULL) return;
    TreeNode* stmtNode = stmtsNode->ASTChild;
    while(stmtNode){
        checkForStmt(lineNumber, tofind, stmtNode, scope);
        stmtNode= stmtNode->ASTRight;
    }
}

int compareArrTypes(char* t1, char* t2, int erline, char* ername){
    // if not both arr... return 2...print prim mismatch err
    if(strcmp(t1,"INTEGER")==0 || strcmp(t1,"REAL")==0 || strcmp(t1,"BOOLEAN")==0 || strcmp(t2,"INTEGER")==0 || strcmp(t2,"REAL")==0 || strcmp(t2,"BOOLEAN")==0)
        return 1;
    // if both arr check static and compare
    if(strcmp(t1,"INTEGER")==0 || strcmp(t1,"REAL")==0 || strcmp(t1,"BOOLEAN")==0)
        return 0;
    if(strcmp(t2,"INTEGER")==0 || strcmp(t2,"REAL")==0 || strcmp(t2,"BOOLEAN")==0)
        return 0;

    char type1[50]; strcpy(type1, t1);
    char token1Type[50]; strcpy(token1Type,strtok(type1, "("));
    strcpy(token1Type,strtok(NULL, ","));
    char token1Ind1[50];
    strcpy(token1Ind1, strtok(NULL, ","));
    char token1Ind2[50];
    strcpy(token1Ind2, strtok(NULL, ")"));

    char type2[50]; strcpy(type2, t2);
    char token2Type[50]; strcpy(token2Type,strtok(type2, "("));
    strcpy(token2Type,strtok(NULL, ","));
    char token2Ind1[50];
    strcpy(token2Ind1, strtok(NULL, ","));
    char token2Ind2[50];
    strcpy(token2Ind2, strtok(NULL, ")"));

    if(strcmp(token1Type, token2Type)!=0){
        semanticerr=1;
        printf("Line(%d). Type of Input Argument Array: '%s'(%s) did not match with expected type of Array: '%s'. \n", erline, ername, token2Type, token1Type);
        return 0;
    }

    if(!((atoi(token1Ind1)==0&&strcmp(token1Ind1,"0")!=0)||(atoi(token1Ind2)==0&&strcmp(token1Ind2,"0")!=0)))
        if(!((atoi(token2Ind1)==0&&strcmp(token2Ind1,"0")!=0)||(atoi(token2Ind2)==0&&strcmp(token2Ind2,"0")!=0)))
            if((atoi(token1Ind2)!=(atoi(token2Ind2)) || (atoi(token1Ind1))!=atoi(token2Ind1))){
                semanticerr=1;
                printf("Line(%d). Input Argument Array: '%s' does not have same range as compared to expected Array of range: %d..%d \n", erline, ername, atoi(token1Ind1), atoi(token1Ind2));
            }
    return 0;
}

void compareListOp(TreeNode* idlnode, FuncTableNode* ftnode, char* scope, TreeNode* curr){
    // Output List by defn does not contain param with Array Type
    char scopetemp[50];
    strcpy(scopetemp, scope);
    TreeNode* idnode = idlnode->ASTChild;
    int match=1, isfew=0, i;
    int numio = ftnode->numOutputParam;
    for(i=0; i<numio; i++){
        if(idnode==NULL){
            isfew=1;
            semanticerr=1;
            printf("Line(%d). Too Few Return Arguments for used(called) module: '%s'. Expected number: %d.\n", curr->lineNumber, curr->tokenLexeme, numio);
            break;
        }
        SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopetemp);
        strcpy(scopetemp, scope);
        if(stnode==NULL){
            semanticerr=1;
            printf("Line(%d). Undeclared Variable: '%s' in Return Arguments of used(called) module: '%s'. \n", idnode->lineNumber, idnode->tokenLexeme, curr->tokenLexeme); 
            idnode = idnode->ASTRight; match=0; continue;
        } // if undeclared var, error already reported, check for others
        if(strcmp(ftnode->outputType[i], stnode->type)!=0){
            semanticerr=1;
            printf("Line(%d). Type of Return Argument: '%s'(%s) did not match with expected type: '%s'.\n", idnode->lineNumber, stnode->tokenName, stnode->type, ftnode->outputType[i]);
            match=0;
        }
        else if(strcmp(ftnode->outputType[i], stnode->type)==0){
            stnode->isAssigned=1;
            //printf("SA 71. %s assigned value.\n", stnode->tokenName);
        }
        idnode = idnode->ASTRight;
    }
    if(isfew==0 && i==numio && idnode!=NULL){
        semanticerr=1;
        printf("Line(%d). Too Many Return Arguments for used(called) module: '%s'. Expected number: %d. \n", curr->lineNumber, curr->tokenLexeme, numio);
    }
}

void compareListIp(TreeNode* idlnode, FuncTableNode* ftnode, char* scope, TreeNode* curr){
    // Output List by defn does not contain param with Array Type
    char scopetemp[50];
    strcpy(scopetemp, scope);
    TreeNode* idnode = idlnode->ASTChild;
    int match=1, isfew=0, i;
    int numio = ftnode->numInputParam;
    for(i=0; i<numio; i++){
        if(idnode==NULL){
            isfew=1;
            semanticerr=1;
            printf("Line(%d). Too Few Input Arguments for used(called) module: '%s'. Expected number: %d.\n", curr->lineNumber, curr->tokenLexeme, numio);
            break;
        }
        SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopetemp);
        strcpy(scopetemp, scope);
        if(stnode==NULL){ 
            semanticerr=1;
            printf("Line(%d). Undeclared Variable: '%s' in Input Arguments of used(called) module: '%s'. \n", idnode->lineNumber, idnode->tokenLexeme, curr->tokenLexeme); 
            idnode = idnode->ASTRight; match=0; continue;
        } // if undeclared var, error already reported, check for others
        if(strcmp(ftnode->inputType[i], stnode->type)!=0){
            int perrr = compareArrTypes(ftnode->inputType[i], stnode->type, idnode->lineNumber, stnode->tokenName);
            if(perrr){
                semanticerr=1;
                printf("Line(%d). Type of Input Argument: '%s'(%s) did not match with expected type: '%s'. \n", idnode->lineNumber, stnode->tokenName, stnode->type, ftnode->inputType[i]);
            }
            match=0;
        }
        idnode = idnode->ASTRight;
    }
    if(isfew==0 && i==numio && idnode!=NULL){
        semanticerr=1;
        printf("Line(%d). Too Many Input Arguments for used(called) module: '%s'. Expected number: %d. \n", curr->lineNumber, curr->tokenLexeme, numio);
    }
}


void semanticAnalyser(TreeNode* curr, char* scope){
    if(curr==NULL) return;

    // scope change
    if(curr->flag==0 && strcmp(nterms[curr->value],"driverModule")==0)
        strcpy(scope,"driver");

    char scopenew[50]; strcpy(scopenew, scope);
    semanticAnalyser(curr->ASTChild,scope);

    if(curr->flag==1 && curr->value==ID && curr->ASTParent && curr->ASTParent->flag==0 && curr->ASTParent->value==module){
        int index_out=getFuncTableIndex(curr->tokenLexeme);
        if(index_out==-1) { //printf("invalid scope searched, no st corrs"); exit(0);
            return;
        }
        if(curr->lineNumber > FT->ftable[index_out]->endScope){
            //printf("SORRY\n"); 
            return;
        }
        strcpy(scope,curr->tokenLexeme);
    }
    strcpy(scopenew, scope);

    // semantic analysis
    if(curr->flag==1 && curr->value==ID){
        TreeNode* parent = curr->ASTParent;

        // CASE : MODULE REUSE STMT
        if(parent->flag==0 && parent->value==moduleReuseStmt){
            // curr is ID: module name
            int index_out=getFuncTableIndex(curr->tokenLexeme);
            if(index_out==-1){
                semanticerr=1;
                printf("Line(%d). Use of unknown module: '%s'. \n", curr->lineNumber, curr->tokenLexeme);
            }
            else{
            // module found in function table
                FuncTableNode* ftnode = FT->ftable[index_out];
            // CHECK DIRECT RECURSION
                if(ftnode){
                    checkRecursion(ftnode, scopenew, curr->lineNumber);
                    strcpy(scopenew, scope);
                }

            // 1. check for output plist and left of assignment
                if(ftnode->numOutputParam==0 && parent->ASTChild->ASTChild!=NULL){
                    semanticerr=1;
                    printf("Line(%d). Invalid assignment during module reuse: '%s' which does not return anything. \n", curr->lineNumber, curr->tokenLexeme);
                }
                else if(ftnode->numOutputParam>0){
                    if(parent->ASTChild->ASTChild==NULL){
                        semanticerr=1;
                        printf("Line(%d). Invalid module reuse: '%s'. Returned values need to be assigned properly. \n", curr->lineNumber, curr->tokenLexeme);
                    }
                    // match type and number
                    else{
                        TreeNode* idlnode = parent->ASTChild->ASTChild;
                        compareListOp(idlnode, ftnode, scopenew, curr);// 0 for oplist
                        strcpy(scopenew, scope);
                    }
                }

            // 2. check for input plist and passed arguments
                // match type and number
                TreeNode* idlnode = curr->ASTRight;
                compareListIp(idlnode, ftnode, scopenew, curr);// 1 for iplist
                strcpy(scopenew, scope);
            }
        }
        // CASE : FOR LOOP
        else if(curr->ASTParent->ASTChild->flag==1 && curr->ASTParent->ASTChild->value==FOR){
            // curr is loop variable
            char scope1[50];
            int index_out=getFuncTableIndex(scopenew);
            strcpy(scopenew, scope);
                if(index_out==-1) { //printf("SA 169 invalid scope searched, no st corrs"); exit(0);
                    return;
                }
            strcpy(scope1, FT->ftable[index_out]->parent->funcName);
            //printf("Finding STNODE for FOR(%s) in scope: %s \n",curr->tokenLexeme, scope1);
            SymTableNode* stiter = getAnyNode(curr->lineNumber, curr->tokenLexeme, scope1);
            //SymTableNode* stiter = getAnyNode(curr->tokenLexeme, scope);
            if(stiter&&strcmp(stiter->type,"INTEGER")!=0){
                semanticerr=1;
                printf("Line(%d). Expected INTEGER Type Variable in FOR Loop. Found: '%s' of Type '%s'. \n", curr->lineNumber, curr->tokenLexeme, stiter->type);
            }
            else if(stiter){
                // make sure that curr/stiter is not redeclared anywhere in this loop
                TreeNode* stmtsNode = curr->ASTRight->ASTRight;
                //TO DO :: For LOOP semantics
                checkForStmts(stiter->lineNumber, curr->tokenLexeme, stmtsNode, scopenew);
                strcpy(scopenew, scope);
            }
        }

    }

    if(curr->flag==1 && (curr->value==FOR||curr->value==WHILE||curr->value==SWITCH)){
        char newscope[50];
        sprintf(newscope,"%s_%d",terms[curr->value],curr->lineNumber);
        semanticAnalyser(curr->ASTRight,newscope);
        return;
    }
    semanticAnalyser(curr->ASTRight,scope);
}


void semanticAnalysis(TreeNode* curr, char* scope){
    //printf("ANOTHER PASS\n");
    // AST passing
    printf(BOLDCYAN "%15s\n" RESET,"During semantic analysis:\n");
    semanticAnalyser(curr, scope);

    if(semanticerr==0)
        printf(BOLDGREEN "%15s\n" RESET,"No error found.\n");
    printf(BOLDCYAN "%15s\n" RESET,"During remaining checks:\n");
    // traverse all modules and check if all their output params are assigned a value or not
    for(int i=0;i<SIZE_FT_;i++){
        int allopdecl = 1;
        int j;
        if(strcmp(FT->ftable[i]->funcName,"")!=0){
            if(FT->ftable[i]->isModule==1){
                for(j=0;j<SIZE_IN_;j++){
                    if(strcmp(ST->out_table[i]->in_table[j]->tokenName,"")!=0 && ST->out_table[i]->in_table[j]->in_out_var==2){
                        if(ST->out_table[i]->in_table[j]->isAssigned==0){
                            allopdecl=0; break;
                        }
                    }

                }
                if(allopdecl==0){
                    othererr=1;
                    printf("Line(%d). Output Parameter: '%s' is never assigned a value in Module: '%s'.\n", FT->ftable[i]->endScope, ST->out_table[i]->in_table[j]->tokenName, FT->ftable[i]->funcName);
                }
            }
        }
    }
}
