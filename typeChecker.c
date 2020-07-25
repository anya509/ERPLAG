//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
//#include "symbolTable.c"
#include "typeChecker.h"
typedef enum {tinteger, treal, tboolean, terror}Types;
char* types[] = {"INTEGER", "REAL", "BOOLEAN", "ERROR"};

void compareArrTypes1(char* t1, char* t2, int erline){
    // if not both arr... return 2...print prim mismatch err
    if(strcmp(t1,"INTEGER")==0 || strcmp(t1,"REAL")==0 || strcmp(t1,"BOOLEAN")==0 || strcmp(t2,"INTEGER")==0 || strcmp(t2,"REAL")==0 || strcmp(t2,"BOOLEAN")==0)
        return;
    // if both arr check static and compare
    if(strcmp(t1,"INTEGER")==0 || strcmp(t1,"REAL")==0 || strcmp(t1,"BOOLEAN")==0)
        return;
    if(strcmp(t2,"INTEGER")==0 || strcmp(t2,"REAL")==0 || strcmp(t2,"BOOLEAN")==0)
        return;

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
        tcerr=1;
        printf("Line(%d). Type Mismatch for Array. Array Type on LHS: %s, RHS: %s. \n", erline, token1Type, token2Type);
    }

    if(!((atoi(token1Ind1)==0&&strcmp(token1Ind1,"0")!=0)||(atoi(token1Ind2)==0&&strcmp(token1Ind2,"0")!=0)))
        if(!((atoi(token2Ind1)==0&&strcmp(token2Ind1,"0")!=0)||(atoi(token2Ind2)==0&&strcmp(token2Ind2,"0")!=0)))
            if((atoi(token1Ind2)!=(atoi(token2Ind2)) || (atoi(token1Ind1))!=atoi(token2Ind1))){
                tcerr=1;
                printf("Line(%d). Type Mismatch for Array. Array Range on LHS: %d to %d, RHS: %d to %d. \n", erline, atoi(token1Ind1), atoi(token1Ind2), atoi(token2Ind1), atoi(token2Ind2));
            }

}
SymTableNode* getSymTableNode1(char * key, char * fname){
    // return variable if in scope
    // if scope is a module, checks in local variable, if not there checks in input or output param list
    int index_out=getFuncTableIndex(fname);
    if(index_out==-1) { //printf("invalid scope searched, no st corrs"); exit(0);
        return NULL;
    }
    int index_in= hashFunction(key,SIZE_IN_);

    //if not module
    if(FT->ftable[index_out]->isModule==0){
        int i=index_in;
        while(strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)!=0){
            i++;
            i=i%SIZE_IN_;
            if(i==index_in)
                return NULL;
        }
        return ST->out_table[index_out]->in_table[i];
    }
    //if module
    else{
        //printf("ST FOR MODULE\n");
        // check local first
        int i=index_in;
        while(! (strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0 && ST->out_table[index_out]->in_table[i]->in_out_var==0 )){
            i++;
            i=i%SIZE_IN_;
            if(i==index_in)
                break;
        }
        if(strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0 && ST->out_table[index_out]->in_table[i]->in_out_var==0)
            return ST->out_table[index_out]->in_table[i];
        // check ip&op next
        i=index_in;
        while(! (strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0 && ST->out_table[index_out]->in_table[i]->in_out_var>0) ){
            i++;
            i=i%SIZE_IN_;
            if(i==index_in)
                return NULL;
        }
        return ST->out_table[index_out]->in_table[i];
    }
}
SymTableNode* getSymTableNode2(int lineno, char * key, char * fname){
    // return variable if in scope
    // if scope is a module, checks in local variable, if not there checks in input or output param list
    int index_out=getFuncTableIndex(fname);
    if(index_out==-1) { //printf("invalid scope searched, no st corrs"); exit(0);
        return NULL;
    }
    int index_in= hashFunction(key,SIZE_IN_);

    //if not module
    if(FT->ftable[index_out]->isModule==0){
        int i=index_in;
        while(strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)!=0 || lineno<ST->out_table[index_out]->in_table[i]->lineNumber){
            i++;
            i=i%SIZE_IN_;
            if(i==index_in)
                return NULL;
        }
        return ST->out_table[index_out]->in_table[i];
    }
    //if module
    else{
        //printf("ST FOR MODULE\n");
        // check local first
        int i=index_in;
        while(! (strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0 && ST->out_table[index_out]->in_table[i]->in_out_var==0 && lineno>=ST->out_table[index_out]->in_table[i]->lineNumber)){
            i++;
            i=i%SIZE_IN_;
            if(i==index_in)
                break;
        }
        if(strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0 && ST->out_table[index_out]->in_table[i]->in_out_var==0)
            return ST->out_table[index_out]->in_table[i];
        // check ip&op next
        i=index_in;
        while(! (strcmp(ST->out_table[index_out]->in_table[i]->tokenName,key)==0 && ST->out_table[index_out]->in_table[i]->in_out_var>0 && lineno>=ST->out_table[index_out]->in_table[i]->lineNumber) ){
            i++;
            i=i%SIZE_IN_;
            if(i==index_in)
                return NULL;
        }
        return ST->out_table[index_out]->in_table[i];
    }
}
SymTableNode* getAnyNode(int lineno, char * key, char * fname){
    //printf("getAnyNode %s %s \n", key, fname);
    SymTableNode* find = NULL;
    while(find==NULL){
        // get node if in particular scope
        //printf("%d %s %s \n", lineno, key, fname);
        find = getSymTableNode2(lineno, key, fname);
        //printf("getAnyNode %s %s \n", key, fname);
        int index_out=getFuncTableIndex(fname);

        // stop finding if no parent
        if(index_out==-1) { //printf("invalid scope searched, no st corrs"); exit(0);
            return NULL;
        }
        if(FT->ftable[index_out]->parent)
            strcpy(fname, FT->ftable[index_out]->parent->funcName);
        else
            break;
    }
    //if(find){printf("returning not null st node\n");} else{printf("returning null st node");}
    return find;
}

int isPrimtive(SymTableNode* node){
    if(node==NULL)
        return -1; //No such node
    if(strcmp(node->type,"INTEGER")==0)
        return 1;
    if(strcmp(node->type,"REAL")==0)
        return 1;
    if(strcmp(node->type,"BOOLEAN")==0)
        return 1;
    return -1;
}
//TO DO
int isBinPlus(TreeNode* node){
    if(node && node->flag==1 && node->value==PLUS && node->ASTChild && node->ASTChild->ASTRight)
        return 1;
    return 0;
}
int isBinMinus(TreeNode* node){
    if(node && node->flag==1 && node->value==MINUS && node->ASTChild && node->ASTChild->ASTRight)
        return 1;
    return 0;
}
int isBinMul(TreeNode* node){
    if(node && node->flag==1 && node->value==MUL && node->ASTChild && node->ASTChild->ASTRight)
        return 1;
    return 0;
}
int isBinDiv(TreeNode* node){
    if(node && node->flag==1 && node->value==DIV && node->ASTChild && node->ASTChild->ASTRight)
        return 1;
    return 0;
}
int isArithmeticOp(TreeNode* node){
    if(isBinPlus(node)||isBinMinus(node)||isBinMul(node)||isBinDiv(node))
        return 1;
    return 0;
}
int isRelationalOp(TreeNode* node){
    if(node && node->flag==1 && node->ASTChild && node->ASTChild->ASTRight){
        if(node->value==LT || node->value==LE || node->value==EQ || node->value==NE || node->value==GT || node->value==GE)
            return 1;
        return 0;
    }
    return 0;
}
int isLogicalOp(TreeNode* node){
    if(node && node->flag==1 && node->ASTChild && node->ASTChild->ASTRight){
        if(node->value==AND || node->value==OR)
            return 1;
        return 0;
    }
    return 0;
}
int isUnaryOp(TreeNode* node){
    if(node && node->flag==1 && node->ASTRight){
        if(node->value==PLUS || node->value==MINUS)
            return 1;
        return 0;
    }
    return 0;
}
int iswhilechange=0;
void checkWhileIdStmts(int lineNumber, char* tofind, int lineNumber2, char* tofind2, TreeNode* stmtsNode, char* scope, int passt);
void checkWhileIdStmt(int lineNumber, char* tofind, int lineNumber2, char* tofind2, TreeNode* stmtNode, char* scope, int passt){
    // DECLARE , PRINT,
    // ASSMT, MODULE RESUSE, GET VALUE
    // WHILE STMT
    if(stmtNode->ASTChild && stmtNode->ASTChild->flag==1 && stmtNode->ASTChild->value==WHILE){
        char newscope[50];
        sprintf(newscope,"WHILE_%d",stmtNode->ASTChild->lineNumber);
        //if(getSymTableNode1(tofind, newscope)==NULL) // if NOT NULL, found a redeclaration of variable, diff var goes by the same name
            checkWhileIdStmts(lineNumber, tofind, lineNumber2, tofind2, stmtNode->ASTChild->ASTRight->ASTRight, newscope, passt);
    }
    // FOR STMT
    else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==1 && stmtNode->ASTChild->value==FOR){
        char newscope[50];
        sprintf(newscope,"FOR_%d",stmtNode->ASTChild->lineNumber);
        //if(getSymTableNode1(tofind, newscope)==NULL)
            checkWhileIdStmts(lineNumber, tofind, lineNumber2, tofind2, stmtNode->ASTChild->ASTRight->ASTRight->ASTRight, newscope, passt);
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
                    checkWhileIdStmts(lineNumber, tofind, lineNumber2, tofind2, nodeCase->ASTRight->ASTRight, newscope, passt);
                }
                nodeCase = nodeCase->ASTRight->ASTRight->ASTRight;
            }
            TreeNode* nodeDef = stmtNode->ASTChild->ASTRight->ASTRight->ASTRight;
            if(nodeDef && nodeDef->flag==0 && nodeDef->value==default1){
                checkWhileIdStmts(lineNumber, tofind, lineNumber2, tofind2, nodeDef->ASTChild, newscope, passt);
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
                    //semanticerr=1;
                    //printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
                    iswhilechange=1;
                }
            }
        }
        else if(idnode->ASTRight->flag==0 && idnode->ASTRight->value==lvalueARRStmt){
            TreeNode* indexNode = idnode->ASTRight->ASTChild;
            if(indexNode==NULL) return;
            if(strcmp(idnode->tokenLexeme, tofind)==0){
                char scopenew[50]; strcpy(scopenew, scope);
                SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                if(stnode && stnode->lineNumber == lineNumber){
                    if(passt==1 && indexNode->flag==1 && indexNode->value == NUM && strcmp(indexNode->tokenLexeme, tofind2)==0){
                        iswhilechange=1;
                    }
                    else if(passt==2 && indexNode->flag==1 && indexNode->value == ID && strcmp(indexNode->tokenLexeme, tofind2)==0){
                        char scopenew[50]; strcpy(scopenew, scope);
                        SymTableNode* instnode = getAnyNode(indexNode->lineNumber, indexNode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                        if(instnode && instnode->lineNumber == lineNumber2){
                            //semanticerr=1;
                            //printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
                            iswhilechange=1;
                        }
                    }
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
                // semanticerr=1;
                // printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
                iswhilechange=1;
            }
        }
    }
    // MODULE RESUSE
     else if(stmtNode->ASTChild && stmtNode->ASTChild->flag==0 && stmtNode->ASTChild->value==moduleReuseStmt){
       // printf("inmoduleresuse \n");
        if(stmtNode->ASTChild->ASTChild && stmtNode->ASTChild->ASTChild->ASTChild){
            TreeNode* idnode = stmtNode->ASTChild->ASTChild->ASTChild->ASTChild;
            while(idnode){
                if(strcmp(idnode->tokenLexeme, tofind)==0){
                    char scopenew[50]; strcpy(scopenew, scope);
                    SymTableNode* stnode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                    if(stnode && stnode->lineNumber == lineNumber){
                        //semanticerr=1;
                        //printf("Line(%d). Change in value of: '%s' of FOR Loop by Assignment.\n", idnode->lineNumber, idnode->tokenLexeme);
                        iswhilechange=1;
                    }
                }
                idnode = idnode->ASTRight;
            }
        }
     }
}
void checkWhileIdStmts(int lineNumber, char* tofind, int lineNumber2, char* tofind2, TreeNode* stmtsNode, char* scope, int passt){
    // ITERATING INDEX OF FOR IS VARIABLE(ID) OF TYPE NUM ONLY
    if(stmtsNode==NULL) return;
    TreeNode* stmtNode = stmtsNode->ASTChild;
    while(stmtNode){
        //printf("%s \n", stmtNode->tokenLexeme);
        checkWhileIdStmt(lineNumber, tofind, lineNumber2, tofind2, stmtNode, scope, passt);
        stmtNode= stmtNode->ASTRight;
    }
}
void checkWhile(TreeNode* node, char* scope, TreeNode* stmtsNode){
    char scopenew[50]; strcpy(scopenew, scope);
    if(node && node->flag==0 && node->value==arithmeticOrBooleanExpr){
        checkWhile(node->ASTChild, scopenew, stmtsNode); strcpy(scopenew, scope);
    }
    else if(isArithmeticOp(node) || isRelationalOp(node) || isLogicalOp(node)){
        checkWhile(node->ASTChild, scopenew, stmtsNode); strcpy(scopenew, scope);
        checkWhile(node->ASTChild->ASTRight, scopenew, stmtsNode); strcpy(scopenew, scope);
    }
    // NUM RNUM TRUE FALSE .. DO NOTHING
    // Identifier
    else if(node && node->flag==1 && node->value==ID){
        SymTableNode* stnode1 = getAnyNode(node->lineNumber, node->tokenLexeme, scopenew); strcpy(scopenew, scope); 
        if(stnode1==NULL){ return; } 

        if(node->ASTChild->flag==0 && node->ASTChild->value==whichId && node->ASTChild->ASTChild!=NULL){
            //var x[26], x[28.7], x[y]
            if(!( (strcmp(stnode1->type,"INTEGER")==0) || (strcmp(stnode1->type,"REAL")==0) || (strcmp(stnode1->type,"BOOLEAN")==0) )){
                // Array Element Here  //Array Index Cases
                if(node->ASTChild->ASTChild->flag==1 && node->ASTChild->ASTChild->value==NUM){
                    TreeNode* indexNode = node->ASTChild->ASTChild;
                    // chack A changes
                    checkWhileIdStmts(stnode1->lineNumber, stnode1->tokenName, stnode1->lineNumber, stnode1->tokenName, stmtsNode, scopenew, 0); strcpy(scopenew, scope);
                    // check A[NUM] changes
                    checkWhileIdStmts(stnode1->lineNumber, stnode1->tokenName, stnode1->lineNumber, node->ASTChild->ASTChild->tokenLexeme, stmtsNode, scopenew, 1); strcpy(scopenew, scope);
                }
                else if(node->ASTChild->ASTChild->flag==1 && node->ASTChild->ASTChild->value==ID){
                    //A[m]
                    TreeNode* indexNode = node->ASTChild->ASTChild;
                    SymTableNode* indexSTNode = getAnyNode(indexNode->lineNumber, indexNode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                    if(indexSTNode&&strcmp(indexSTNode->type,"INTEGER")==0){
                        // check if index (m) changes
                        checkWhileIdStmts(indexSTNode->lineNumber, indexSTNode->tokenName, indexSTNode->lineNumber, indexSTNode->tokenName, stmtsNode, scopenew, 0); strcpy(scopenew, scope);
                        // chack A changes
                        checkWhileIdStmts(stnode1->lineNumber, stnode1->tokenName, stnode1->lineNumber, stnode1->tokenName, stmtsNode, scopenew, 0); strcpy(scopenew, scope);
                        // check if A[m] changes
                        checkWhileIdStmts(stnode1->lineNumber, stnode1->tokenName, indexSTNode->lineNumber, indexSTNode->tokenName, stmtsNode, scopenew, 2); strcpy(scopenew, scope);
                    }
                }
            }
        }
        else{
            //var x
            if((strcmp(stnode1->type,"INTEGER")==0)||(strcmp(stnode1->type,"REAL")==0)||(strcmp(stnode1->type,"BOOLEAN")==0)){
                //printf("check while: %d %s %s \n", stnode1->lineNumber, stnode1->tokenName, scopenew);
                checkWhileIdStmts(stnode1->lineNumber, stnode1->tokenName,stnode1->lineNumber, stnode1->tokenName, stmtsNode, scopenew, 0); strcpy(scopenew, scope);
                //printf("checked while: %d %s %s \n", stnode1->lineNumber, stnode1->tokenName, scopenew);
            }
        }
    }
    //else if(node!=NULL) printf("%d %d %s %s\n", node->lineNumber, node->flag, node->tokenLexeme, nterms[node->value]);
    
}
int getExprType(TreeNode* node, char* scope){
    if(node==NULL){ return terror; }
    char scopenew[50]; strcpy(scopenew, scope);
    if(node && node->flag==0 && (node->value==arithmeticOrBooleanExpr || node->value==U) ){
        return getExprType(node->ASTChild, scopenew); strcpy(scopenew, scope);
    }
    else if(isArithmeticOp(node)){
        //printf("in arop check\n");
        int type1 = getExprType(node->ASTChild, scopenew); strcpy(scopenew, scope);
        int type2 = getExprType(node->ASTChild->ASTRight, scopenew); strcpy(scopenew, scope);
        if(type1==terror||type2==terror)
            return terror;
        if(type1==type2&&((type1==tinteger)||(type1==treal)))
            return type1;
        else{
            tcerr=1;
            printf("Line(%d). Type Mismatch Error. Operand Types: %s and %s. Operator: %s\n",node->lineNumber,types[type1],types[type2],node->tokenLexeme);
            return terror;
        }
    }
    else if(isRelationalOp(node)){
        //printf("in relop check\n");
        int type1 = getExprType(node->ASTChild, scopenew); strcpy(scopenew, scope);
        int type2 = getExprType(node->ASTChild->ASTRight, scopenew); strcpy(scopenew, scope);
        if(type1==terror||type2==terror)
            return terror;
        if(type1==type2&&((type1==tinteger)||(type1==treal)))
            return tboolean;
        else{
            tcerr=1;
            printf("Line(%d). Type Mismatch Error. Operand Types: %s and %s. Operator: %s\n",node->lineNumber,types[type1],types[type2],node->tokenLexeme);
            return terror;
        }
    }
    else if(isLogicalOp(node)){
        //printf("in logop check\n");
        int type1 = getExprType(node->ASTChild, scopenew); strcpy(scopenew, scope);
        int type2 = getExprType(node->ASTChild->ASTRight, scopenew); strcpy(scopenew, scope);
        if(type1==terror||type2==terror)
            return terror;
        if(type1==type2&&type1==tboolean)
            return tboolean;
        else{
            tcerr=1;
            printf("Line(%d). Type Mismatch Error. Operand Types: %s and %s. Operator: %s\n",node->lineNumber,types[type1],types[type2],node->tokenLexeme);
            return terror;
        }
    }
    else if(node && node->flag==1 && (strcmp(terms[node->value],"TRUE")==0 || strcmp(terms[node->value],"FALSE")==0)){
        //printf("in t/f/ check\n");
        return tboolean;
    }
    else if(node && node->flag==1 && node->value==NUM)
        return tinteger;
    else if(node && node->flag==1 && node->value==RNUM)
        return treal;
    else if(isUnaryOp(node)){
        //printf("in unaryop CHECK\n");
        int type = getExprType(node->ASTRight, scopenew); strcpy(scopenew, scope);
        if(type==terror||type==tinteger||type==treal) return type;
        else if(type==tboolean) {
            tcerr=1;
            printf("Line(%d). Type Mismatch Error. Operand Type: %s. Unary Operator: %s\n",node->lineNumber,types[type],node->tokenLexeme);
            return terror;
        }
    }

    // CASE VAR_ID_NUM THING; ID, ID[STH]
    else if(node && node->flag==1 && node->value==ID){
        //printf("in VAR_ID CHECK\n");
        SymTableNode* stnode1 = getAnyNode(node->lineNumber, node->tokenLexeme, scopenew); strcpy(scopenew, scope);
        if(stnode1==NULL){ return terror; } // Type error due to occurence of undeclared variable

        if(node->ASTChild->flag==0 && node->ASTChild->value==whichId && node->ASTChild->ASTChild!=NULL){
            //var x[26], x[28.7], x[y]
            if( (strcmp(stnode1->type,"INTEGER")==0) || (strcmp(stnode1->type,"REAL")==0) || (strcmp(stnode1->type,"BOOLEAN")==0) ){
                tcerr=1;
                printf("Line(%d). Invalid Occurence of %s Variable: '%s' in expression. Expected ARRAY variable\n", node->lineNumber, stnode1->type, node->tokenLexeme);
                return terror;
            }
            else{
                //Extract Array Type And Index Range
                char type1[50]; strcpy(type1, stnode1->type);
                char tokenType[50]; strcpy(tokenType,strtok(type1, "("));
                strcpy(tokenType,strtok(NULL, ","));
                char tokenInd1[50];
                strcpy(tokenInd1, strtok(NULL, ","));
                char tokenInd2[50];
                strcpy(tokenInd2, strtok(NULL, ")"));
                // tokenType[tokenInd1..tokenInd2]
                //Array Index Cases
                if(node->ASTChild->ASTChild->flag==1 && node->ASTChild->ASTChild->value==RNUM){
                    tcerr=1;
                    printf("Line(%d). Invalid Array Index: '%s' of type RNUM\n", node->ASTChild->ASTChild->lineNumber, node->ASTChild->ASTChild->tokenLexeme);
                }
                else if(node->ASTChild->ASTChild->flag==1 && node->ASTChild->ASTChild->value==NUM){
                    // check index in bound if static array
                    TreeNode* indexNode = node->ASTChild->ASTChild;
                    if(!((atoi(tokenInd1)==0&&strcmp(tokenInd1,"0")!=0)||(atoi(tokenInd2)==0&&strcmp(tokenInd2,"0")!=0))){
                        //static array, now check bound
                        if(atoi(indexNode->tokenLexeme)<atoi(tokenInd1)||atoi(indexNode->tokenLexeme)>atoi(tokenInd2)){
                            tcerr=1;
                            printf("Line(%d). Array('%s') Index out of Bounds. Index Value Found: %d\n", indexNode->lineNumber, node->tokenLexeme, atoi(indexNode->tokenLexeme));
                        }
                    }
                }
                else if(node->ASTChild->ASTChild->flag==1 && node->ASTChild->ASTChild->value==ID){
                    // check index is integer or not
                    TreeNode* indexNode = node->ASTChild->ASTChild;
                    SymTableNode* indexSTNode = getAnyNode(indexNode->lineNumber, indexNode->tokenLexeme, scopenew); strcpy(scopenew, scope);
                    if(indexSTNode&&strcmp(indexSTNode->type,"INTEGER")!=0){
                        tcerr=1;
                        printf("Line(%d). Expected Integer Type Array Index. Found: '%s'.\n", indexNode->lineNumber, indexNode->tokenLexeme);
                    }
                }

                //return array element type
                if(strcmp(tokenType,"INTEGER")==0)
                    return tinteger;
                else if(strcmp(tokenType,"REAL")==0)
                    return treal;
                else if(strcmp(tokenType,"BOOLEAN")==0)
                    return tboolean;
                return terror; // should not occur though
            }
        }

        //else if(node->ASTRight->flag==0 && node->ASTRight->value==whichId && node->ASTRight->ASTChild==NULL){
        else{
            //var x
            if(strcmp(stnode1->type,"INTEGER")==0)
                return tinteger;
            else if(strcmp(stnode1->type,"REAL")==0)
                return treal;
            else if(strcmp(stnode1->type,"BOOLEAN")==0)
                return tboolean;
            else{
                //printf("Line(%d). Invalid Occurence of Array Variable: '%s' in expression.\n", node->lineNumber, node->tokenLexeme);
                if(node->ASTParent && node->ASTParent->flag==1){
                    switch(node->ASTParent->value){
                        case LT: case LE: case GT: case GE: case EQ: case NE: case PLUS: case MINUS: case MUL: case DIV: case AND: case OR:
                        {tcerr=1;printf("Line(%d). Invalid Occurence of Array Variable: '%s' under operation: '%s'.\n", node->lineNumber, node->tokenLexeme, terms[node->ASTParent->value]);}
                    }
                }
                return terror;
            }
        }
    }

    //printf("END OF CHECKEXPRTYPE LOL");
    //printf("getExprType, S: %s, F:%d, T:%s, NT:%s\n", scope, node->flag, node->tokenLexeme, nterms[node->value]);
    //exit(0);
    return terror;
}

void typeChecker(TreeNode* curr, char* scope){
    //DEBUG
    // if(curr && curr->flag==1){
    //     printf("lineNumber: %d. lexeme: %s. scope: %s.  \n", curr->lineNumber, curr->tokenLexeme, scope);
    // }
    // else if(curr && curr->flag==0){
    //     printf("lineNumber: %d. nterm: %s. scope: %s.  \n", curr->lineNumber, nterms[curr->value], scope);
    // }
    if(curr==NULL) {//printf("NULL\n");
        return;
    }

    // scope change
    if(curr->flag==0 && strcmp(nterms[curr->value],"driverModule")==0)
        strcpy(scope,"driver");

    char scopenew[50]; strcpy(scopenew, scope);
    typeChecker(curr->ASTChild,scope);

    if(curr->flag==1 && curr->value==ID && curr->ASTParent && curr->ASTParent->flag==0 && curr->ASTParent->value==module){
        int index_out=getFuncTableIndex(curr->tokenLexeme);
        if(index_out==-1) { //printf("invalid scope searched, no st corrs"); 
            return;
        }
        if(curr->lineNumber > FT->ftable[index_out]->endScope){
            //printf("SORRY\n"); 
            return;
        }
        strcpy(scope,curr->tokenLexeme);
    }
    strcpy(scopenew, scope);
    // type checking
    // curr is id
    if(curr->flag==1 && curr->value==ID){
        TreeNode* parent = curr->ASTParent;


        // CASE : ASSIGNMENT STMT
        if(parent->flag==0 && parent->value==assignmentStmt){
            SymTableNode* node1 = getAnyNode(curr->lineNumber, curr->tokenLexeme, scopenew);
            strcpy(scopenew, scope);
            if(node1 == NULL){
                //printf(" Undeclared Variable: '%s' used at Line Number: %d. \n",curr->tokenLexeme,curr->lineNumber);
                // no need to match
                if(curr->ASTRight->flag==0 && curr->ASTRight->value==lvalueIDStmt){
                    int type2; type2 = getExprType(curr->ASTRight->ASTChild, scopenew); strcpy(scopenew, scope);
                }
                else if(curr->ASTRight->flag==0 && curr->ASTRight->value==lvalueARRStmt){
                    int type2; type2 = getExprType(curr->ASTRight->ASTChild->ASTRight, scopenew); strcpy(scopenew, scope);
                }
            }
            else if(node1 && node1->isArray==0){
                // x:=... or x[]:=... where x is not array variable
                //printf("isArray: %d lexeme: %s\n", node1->isArray, node1->tokenName);
                // lhs beginning part is not an array variable type: node1->type
                //rhs
                if(curr->ASTRight->flag==0 && curr->ASTRight->value==lvalueIDStmt){
                    // x:=... where x is not array variable
                    int type2 = getExprType(curr->ASTRight->ASTChild, scopenew); // passing ABE or U
                    strcpy(scopenew, scope);
                    //printf("%s\n", types[type2]);
                    if(type2!=terror&&strcmp(node1->type,types[type2])!=0){
                        tcerr=1;
                        printf("Line(%d). Type Mismatch Error. Type at Left: %s. Type at Right: %s, %s\n", curr->lineNumber, node1->type, types[type2], curr->ASTRight->ASTChild->tokenLexeme);
                    }
                    else if(type2!=terror){
                        //printf("Line(%d). Type Match. Type: %s\n", curr->lineNumber, types[type2]);
                    }
                    // mark x is declared for corrs stnode
                    node1->isAssigned=1;
                }

                else if(curr->ASTRight->flag==0 && curr->ASTRight->value==lvalueARRStmt){
                    // x[]:=... where x is not array variable
                    TreeNode* indexNode = curr->ASTRight->ASTChild; // NUM OR ID
                    tcerr=1;
                    printf("Line Number : (%d). Expected Array Variable. Found: '%s'.\n", curr->lineNumber, curr->tokenLexeme);
                    int type2 = getExprType(curr->ASTRight->ASTChild->ASTRight, scopenew); // passing ABE or U
                    strcpy(scopenew, scope);
                }
            }
            else if(node1&&node1->isArray==1){
                // x:=... or x[]:=... where x is array variable
                //printf("isArray: %d lexeme: %s\n", node1->isArray, node1->tokenName);
                // lhs beginning part is an array variable type: node1->type
                // rhs
                if(curr->ASTRight->flag==0 && curr->ASTRight->value==lvalueIDStmt){
                    // x:=...  where x is array variable
                    // use this for internal type check of expresiion only
                        //// TO DO : NOTICEEE
                    TreeNode* abenode = curr->ASTRight->ASTChild;
                    if(abenode&&abenode->flag==0&&abenode->value==arithmeticOrBooleanExpr&&abenode->ASTChild&&abenode->ASTChild->ASTRight==NULL && abenode->ASTChild->flag==1 && abenode->ASTChild->value==ID){
                        TreeNode* idnode = abenode->ASTChild;
                        SymTableNode* idSTNode = getAnyNode(idnode->lineNumber, idnode->tokenLexeme, scopenew);
                        strcpy(scopenew, scope);
                        if(idSTNode&&strcmp(idSTNode->type,"INTEGER")!=0 && strcmp(idSTNode->type,"BOOLEAN")!=0 &&strcmp(idSTNode->type,"REAL")!=0 ){
                            compareArrTypes1(idSTNode->type, node1->type, curr->lineNumber);
                        }
                        else if(idSTNode){
                            //error type mismatch primitive on right
                            tcerr=1;
                            printf("Line(%d). Type Mismatch. Expected Type on RHS of assign op: ARRAY. Found: %s. \n", curr->lineNumber,idSTNode->type);
                            int type2 = getExprType(curr->ASTRight->ASTChild, scopenew); // passing ABE or U
                            strcpy(scopenew, scope);
                        }
                    }
                    else{
                        //error
                        int type2 = getExprType(curr->ASTRight->ASTChild, scopenew); // passing ABE or U
                        strcpy(scopenew, scope);
                        if(type2!=terror){
                            tcerr=1;
                            printf("Line(%d). Type Mismatch. Expected Type on RHS of assign op: ARRAY. Found: %s. \n", curr->lineNumber, types[type2]);
                        }
                        else{
                            tcerr=1;
                            printf("Line(%d). Type Mismatch. Expected Type on RHS of assign op: ARRAY. \n", curr->lineNumber);
                        }
                    }
                }
                else if(curr->ASTRight->flag==0 && curr->ASTRight->value==lvalueARRStmt){
                    // x[]:=... where x is array variable
                    TreeNode* indexNode = curr->ASTRight->ASTChild; // NUM OR ID
                    // Find Array Type
                    char type1[50]; strcpy(type1, node1->type);
                    char tokenType[50]; strcpy(tokenType,strtok(type1, "("));
                    strcpy(tokenType,strtok(NULL, ","));
                    char tokenInd1[50];
                    strcpy(tokenInd1, strtok(NULL, ","));
                    char tokenInd2[50];
                    strcpy(tokenInd2, strtok(NULL, ")"));
                    // if ID, check INTEGER ONLY
                    if(indexNode->flag==1 && indexNode->value==ID){
                        SymTableNode* indexSTNode = getAnyNode(indexNode->lineNumber, indexNode->tokenLexeme, scopenew);
                        strcpy(scopenew, scope);
                        if(indexSTNode&&strcmp(indexSTNode->type,"INTEGER")!=0){
                            tcerr=1;
                            printf("Line(%d). Expected Integer Type Array Index. Found: '%s'.\n", indexNode->lineNumber, indexNode->tokenLexeme);
                        }
                    }
                    //if NUM, check index in bound, if static array
                    else if(indexNode->flag==1 && indexNode->value==NUM)
                        if(!((atoi(tokenInd1)==0&&strcmp(tokenInd1,"0")!=0)||(atoi(tokenInd2)==0&&strcmp(tokenInd2,"0")!=0))){
                            //static array
                            if(atoi(indexNode->tokenLexeme)<atoi(tokenInd1)||atoi(indexNode->tokenLexeme)>atoi(tokenInd2)){
                                tcerr=1;
                                printf("Line(%d). Array('%s') Index out of Bounds. Index Value Found: %d.\n", indexNode->lineNumber, curr->tokenLexeme, atoi(indexNode->tokenLexeme));
                            }
                        }

                    //
                    int type2 = getExprType(indexNode->ASTRight, scopenew); // passing ABE or U
                    strcpy(scopenew, scope);
                    if(type2!=terror&&strcmp(tokenType,types[type2])!=0){
                        tcerr=1;
                        printf("Line(%d). Type Mismatch Error. Type at Left: %s. Type at Right: %s.\n", curr->lineNumber, tokenType, types[type2]);
                    }
                    else if(type2!=terror){
                        //printf("Line(%d).Type Match. Type: %s.\n", curr->lineNumber, types[type2]);
                    }
                }
            }
        }

        // CASE : SWITCH STMT
        else if(parent->ASTChild->flag==1 && parent->ASTChild->value==SWITCH){
            // switch(index)
            ///////
            char scope1[50];
            int index_out=getFuncTableIndex(scopenew);
            strcpy(scopenew, scope);
                if(index_out==-1) { //printf("398 invalid scope searched, no st corrs"); exit(0);
                    return;
                }
            strcpy(scope1, FT->ftable[index_out]->parent->funcName);
            //printf("Finding STNODE for SWITCH(%s) in scope: %s \n",curr->tokenLexeme, scope1);
            SymTableNode* nodeInd = getAnyNode(curr->lineNumber, curr->tokenLexeme, scope1);
            strcpy(scope1, FT->ftable[index_out]->parent->funcName);
            // if(nodeInd == NULL){
            //  printf("Line(%d). Undeclared Variable: '%s'. \n",curr->lineNumber,curr->tokenLexeme);
            // }
            ///////
            TreeNode* nodeDef = curr->ASTRight->ASTRight;
            if(nodeInd&&strcmp(nodeInd->type,"INTEGER")==0){
                if(nodeDef->ASTChild && nodeDef->ASTChild->flag==0 && nodeDef->ASTChild->value==statements){
                    //okay
                }
                else{
                    tcerr=1;
                    printf("Line(%d). Error: DEFAULT Case absent in Switch Statement for INTEGER: %s\n", nodeDef->right->lineNumber, curr->tokenLexeme);
                }
                TreeNode* nodeCase = curr->ASTRight->ASTChild;
                while(nodeCase && nodeCase->flag==1 && nodeCase->value==CASE){
                    if(nodeCase->ASTRight && nodeCase->ASTRight->flag==1 && nodeCase->ASTRight->value!=NUM){
                        tcerr=1;
                        printf("Line(%d).  Error: Expected NUM for CASE statement of SWITCH for: %s. \n", nodeCase->ASTRight->lineNumber, curr->tokenLexeme);
                    }
                    nodeCase = nodeCase->ASTRight->ASTRight->ASTRight;
                }
            }
            else if(nodeInd&&strcmp(nodeInd->type,"BOOLEAN")==0){
                if(nodeDef->ASTChild && nodeDef->ASTChild->flag==0 && nodeDef->ASTChild->value==statements){
                    tcerr=1;
                    printf("Line(%d). Error: DEFAULT Case present in Switch Statement for BOOLEAN: %s\n", nodeDef->child->lineNumber, curr->tokenLexeme);
                }
                // TO DO check if true or false only
                TreeNode* nodeCase = curr->ASTRight->ASTChild;
                while(nodeCase && nodeCase->flag==1 && nodeCase->value==CASE){
                    if(nodeCase->ASTRight && nodeCase->ASTRight->flag==1 && nodeCase->ASTRight->value!=TRUE && nodeCase->ASTRight->value!=FALSE){
                        tcerr=1;
                        printf("Line(%d).  Error: Expected True or False for CASE statement of SWITCH for : %s. \n", nodeCase->ASTRight->lineNumber, curr->tokenLexeme);
                    }
                    nodeCase = nodeCase->ASTRight->ASTRight->ASTRight;
                }
            }
            else if(nodeInd&&strcmp(nodeInd->type,"REAL")==0){
                tcerr=1;
                printf("Line(%d). Error: Switch Statement for REAL : %s is not allowed\n", curr->lineNumber, curr->tokenLexeme);
                return;
            }
        }
        // CASE : GET VALUE STMT
        else if(parent->ASTChild->flag==1 && parent->ASTChild->value==GET_VALUE){
            //printf("Finding STNODE for GETVAL(ID) in scope: %s \n", scope);
            //printf("GET VALUE %s\n", scope);
            //char scopenew[50]; strcpy(scopenew,scope);
            SymTableNode* nodeInd = getAnyNode(curr->lineNumber, curr->tokenLexeme, scopenew);
            strcpy(scopenew, scope);
            //printf("GET VALUE %s\n", scope);
            if(nodeInd) nodeInd->isAssigned = 1;
        }
    }
    // curr is ABE for WHILE LOOP
    else if(curr->flag==0 && curr->value==arithmeticOrBooleanExpr){
        TreeNode* parent = curr->ASTParent;
        if(parent && parent->ASTChild && parent->ASTChild->flag==1 && parent->ASTChild->value==WHILE){
            char scope1[50];
            int index_out=getFuncTableIndex(scopenew);
            strcpy(scopenew, scope);
                if(index_out==-1) { //printf("446 invalid scope searched, no st corrs"); exit(0);
                    return;
                }
            strcpy(scope1, FT->ftable[index_out]->parent->funcName);
            //printf("BOOLEAN check for WHILE(ABE) in scope: %s \n", scope1);
            int typeABE = getExprType(curr, scope1);
            strcpy(scope1, FT->ftable[index_out]->parent->funcName);
            if(typeABE!=terror && typeABE!=tboolean){
                tcerr=1;
                printf("Line(%d). BOOLEAN conditional expression expected in WHILE loop, found: %s .\n", parent->ASTChild->lineNumber, types[typeABE]);
            }
            else if(typeABE==tboolean){
                TreeNode* abe = (TreeNode*)malloc(sizeof(TreeNode));
                TreeNode* stmtsNode = (TreeNode*)malloc(sizeof(TreeNode));
                abe = curr; stmtsNode=abe->ASTRight;
                char scope2[50]; strcpy(scope2, scopenew);
                iswhilechange=0;
                //printf("%s %s %s %s\n",scope2, scope1, scope, scopenew);
                checkWhile(abe, scope2, stmtsNode);
                if(iswhilechange==0){
                    tcerr=1;
                    printf("Line(%d). No variable inside conditional of WHILE loop changes its value. \n", parent->ASTChild->lineNumber);
                }
            }
        }
    }



    if(curr->flag==1 && (curr->value==FOR||curr->value==WHILE||curr->value==SWITCH)){
        char newscope[50];
        sprintf(newscope,"%s_%d",terms[curr->value],curr->lineNumber);
        typeChecker(curr->ASTRight,newscope);
        return;
    }
    //printf("to right. scope: %s curr: %d %s %d \n", scope, curr->flag, curr->tokenLexeme, curr->lineNumber);
    typeChecker(curr->ASTRight,scope);
}
