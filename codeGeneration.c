//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "semanticAnalyser.c"
#include "codeGeneration.h"

CodeAssembyL * code;
int numLoops;
int localVar;
char scope[100];
char instrTemp[400];
int fornum,whilenum,relnum;
int isFloat,labelnum;
//char lastDataSection[100];
//int lastWidth;


void initalizeAssembyCode(){
    numLoops=0;
    localVar=0;
    fornum=0;
    whilenum=0;
    isFloat=0;
    relnum=0;
    labelnum=0;
    //lastWidth=0;
    code = (CodeAssembyL*)malloc(sizeof(CodeAssembyL));
    code->startLine=NULL;
    code->lastLine=NULL;
    code->numLines=0;
}

void displayAssemblyCode(){
    if(code==NULL || code->startLine==NULL)
        return;
    //printf("Number of lines %d\n",code->numLines);
    LineAssembly * curr=code->startLine;
    printf("\nASSEMBLY LANGUAGE CODE:\n");
    for(int i=0;i<code->numLines;i++){
        printf("%s",curr->instruction);
        //puts(curr->instruction);
        curr=curr->nextInstruction;
    }
    printf("\n");
}

void saveAssemblyCodeToFile(FILE * fp){
    // FILE * fp=fopen(name,"w");
    if(fp==NULL)
        return;
    if(code==NULL || code->startLine==NULL)
        return;
    //printf("Number of lines %d\n",code->numLines);
    LineAssembly * curr=code->startLine;
    for(int i=0;i<code->numLines;i++){
        fprintf(fp,"%s",curr->instruction);
        //puts(curr->instruction);
        curr=curr->nextInstruction;
    }
    //fclose(fp);
}

void insertInstructionAtEnd(char * instr){
    if(instr==NULL)
        return;

    LineAssembly * line=(LineAssembly*)malloc(sizeof(LineAssembly));
    if(instr!=NULL){
        char *temp=(char *)malloc(1+sizeof(char)*strlen(instr));
        strcpy(temp,instr);
        line->instruction=temp;
        line->nextInstruction=NULL;
    }
    if(code->lastLine==NULL & code->startLine==NULL){
        code->lastLine=line;
        code->startLine=line;
    }
    else{
        code->lastLine->nextInstruction=line;
        code->lastLine=line;
    }
    code->numLines++;
}

void pushonstack(TreeNode* head,char * scope){
    if(head==NULL)
        return;
    //char instrTemp[200];
    pushonstack(head->ASTRight,scope);
    SymTableNode * stn=getSNode(head->tokenLexeme,scope);
    if(stn!=NULL){
        snprintf(instrTemp,sizeof(instrTemp), "        push         qword[%s_%s_%d]\n",stn->tokenName,stn->scope,stn->in_out_var);
        insertInstructionAtEnd(instrTemp);
        puts(instrTemp);
    }
 }

 void popfromstack(TreeNode* head,char * scope){

     if(head==NULL)
         return;
     //char instrTemp[200];
     SymTableNode * stn=getSNode(head->tokenLexeme,scope);
     if(stn!=NULL){
         snprintf(instrTemp,sizeof(instrTemp), "        pop         rax\n");
         insertInstructionAtEnd(instrTemp);
         if(head->ASTParent->ASTParent!=NULL && !head->ASTParent->ASTParent->flag && head->ASTParent->ASTParent->value == optional) {
             snprintf(instrTemp,sizeof(instrTemp), "        mov         qword[%s_%s_%d],rax\n",stn->tokenName,stn->scope,stn->in_out_var);
             insertInstructionAtEnd(instrTemp);
         }
     }
     popfromstack(head->ASTRight,scope);

}

 int getlabel(){
 	static int lab = 0;
 	return lab++;
 }



void generateHeaderCode(){
    // declaring the variables
    insertInstructionAtEnd("section .data        ; This section is for initialized variables\n\n");
    insertInstructionAtEnd("        readformat: db \"INPUT: Enter an integer value\",10,0 \n");
    insertInstructionAtEnd("        writeformat: db \"OUTPUT: \",0 \n");
    insertInstructionAtEnd("        newLine db 0AH,0DH,0  ;for new line\n");
    insertInstructionAtEnd("        format: db \"%ld\",0\n");
    insertInstructionAtEnd("        formatArray: db \"%ld \",0\n");
    insertInstructionAtEnd("        truem db \"true\",10,0\n");
    insertInstructionAtEnd("        falsem db \"false\",10,0\n");
    insertInstructionAtEnd("        enterint db \"INPUT: Enter an integer value\",10,0\n");
    insertInstructionAtEnd("        enterbool db \"INPUT: Enter a boolean (0 or 1) value\",10,0\n");
    insertInstructionAtEnd("        entereal db \"INPUT: Enter a real value\",10,0\n");

    insertInstructionAtEnd("        enterintArr db \"Enter %d elements of integer type for range %d and %d\",10,0\n");
    insertInstructionAtEnd("        enterboolArr db \"Enter %d elements of boolean (0 or 1) type for range %d and %d\",10,0\n");
    insertInstructionAtEnd("        enterrealArr db \"Enter %d elements of real type for range %d and %d\",10,0\n");
    insertInstructionAtEnd("        bounderror db \"Run time error: Array index out of bounds\",10,0\n");

    insertInstructionAtEnd("\nsection .bss        ; This section is for uninitialized locally declared variables\n\n");
    for(int k=0;k<SIZE_FT_;k++){
        if(strcmp(FT->ftable[k]->funcName,"")!=0 && FT->ftable[k]->parent==NULL){
            int index=k;      //getFuncTableIndex("driver");
            int findex=index;
            if(index!= -1){
                //printf("YES");
                int count=-1;
                do {
                    if(count!= -1)
                        findex=getFuncTableIndex(FT->ftable[index]->children[count]->funcName);
                    for(int j=0;j<SIZE_IN_;j++){
                        if(strcmp(ST->out_table[findex]->in_table[j]->tokenName,"")!=0){
                            char type[100],instr[200],name[200];
                            strcpy(type,ST->out_table[findex]->in_table[j]->type);
                            // if(!ST->out_table[findex]->in_table[j]->in_out_var)
                            //     sprintf(name,"%s_%s",ST->out_table[findex]->in_table[j]->tokenName,ST->out_table[findex]->in_table[j]->scope);
                            // else
                            sprintf(name,"%s_%s_%d",ST->out_table[findex]->in_table[j]->tokenName,ST->out_table[findex]->in_table[j]->scope,ST->out_table[findex]->in_table[j]->in_out_var);

                            //printf("%s \n",type);
                            if(strcmp(type,"INTEGER")==0){
                                //printf("YES");
                                snprintf(instr,sizeof(instr),"        %s:         resq  %d\n",name,1);
                                insertInstructionAtEnd(instr);
                            }
                            else if(strcmp(type,"REAL")==0){
                                snprintf(instr,sizeof(instr),"        %s:         resq  %d\n",name,1);
                                insertInstructionAtEnd(instr);
                            }
                            else if(strcmp(type,"BOOLEAN")==0){
                                snprintf(instr,sizeof(instr),"        %s:         resq  %d\n",name,1);
                                insertInstructionAtEnd(instr);
                            }
                            else if(strncmp(type,"ARRAY",5)==0){
                                //Extract Array Type And Index Range
                                //char type1[50]; strcpy(type1, stnode1->type);
                                snprintf(instr,sizeof(instr),"        %s_address:         resq  %d\n",name,1);
                                insertInstructionAtEnd(instr);
                                snprintf(instr,sizeof(instr),"        %s_in:         resq  %d\n",name,1);
                                insertInstructionAtEnd(instr);
                                snprintf(instr,sizeof(instr),"        %s_out:         resq  %d\n",name,1);
                                insertInstructionAtEnd(instr);
                                if(!ST->out_table[findex]->in_table[j]->ifArrayDynamic && !ST->out_table[findex]->in_table[j]->in_out_var){
                                    char tokenType[50]; strcpy(tokenType,strtok(type, "("));
                                    strcpy(tokenType,strtok(NULL, ","));
                                    char tokenInd1[50];
                                    strcpy(tokenInd1, strtok(NULL, ","));
                                    char tokenInd2[50];
                                    strcpy(tokenInd2, strtok(NULL, ")"));
                                    int bound_in,bound_out,range;
                                    bound_out=atoi(tokenInd2);
                                    bound_in=atoi(tokenInd1);
                                    range=bound_out-bound_in+1;
                                    int size_array;
                                    if(strcmp(tokenType,"INTEGER")==0){
                                        size_array=range*2;
                                        snprintf(instr,sizeof(instr),"        %s:         resq  %d\n",name,range);
                                        insertInstructionAtEnd(instr);
                                    }
                                    else if(strcmp(tokenType,"REAL")==0){
                                        size_array=range*4;
                                        snprintf(instr,sizeof(instr),"        %s:         resq  %d\n",name,range);
                                        insertInstructionAtEnd(instr);
                                    }
                                    else if(strcmp(tokenType,"BOOLEAN")==0){
                                        size_array=range*1;
                                        snprintf(instr,sizeof(instr),"        %s:         resq  %d\n",name,range);
                                        insertInstructionAtEnd(instr);
                                    }
                                }
                                // else{
                                //
                                // }

                            }
                        }

                    }
                    //printf("%d \n",FT->ftable[index]->numChild);
                    if(FT->ftable[index]->numChild==0)
                        break;
                    count++;

                }while(count < FT->ftable[index]->numChild);
            }
        }
    }

    //exit(0);
    insertInstructionAtEnd("        loopVar:        resq  1\n");
    insertInstructionAtEnd("        nextAddress:        resq  1\n");
    insertInstructionAtEnd("\nsection .text        ; Code Section\n\n        ");
    insertInstructionAtEnd("global main\n        ");
    insertInstructionAtEnd("extern scanf\n        ");
    insertInstructionAtEnd("extern printf\n        ");
    insertInstructionAtEnd("extern exit\n        ");
    insertInstructionAtEnd("default rel\n\n");
    insertInstructionAtEnd("main :\n");
    insertInstructionAtEnd("        push        rbx\n");
    insertInstructionAtEnd("        mov         rax,nextAddress\n");
    insertInstructionAtEnd("        add         rax,8\n");
    insertInstructionAtEnd("        mov         qword[nextAddress],rax\n");
}

void generateExpressionCode(TreeNode * astNode, char * scope){

    if(astNode==NULL)
        return;

    // if(astNode->flag)
    //     printf("\n %s ", terms[astNode->value]);
    // else
    //     printf("\n %s ", nterms[astNode->value]);

    generateExpressionCode(astNode->ASTChild,scope);

    TreeNode * curr=astNode;
    if(!curr->flag && curr->value==arithmeticOrBooleanExpr){
        generateExpressionCode(curr->ASTRight,scope);
        return;
    }

    if(curr->flag){
        switch(curr->value){
            case PLUS:{

                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        add         rax, rbx\n");
                    insertInstructionAtEnd("        push         rax\n");
                    //printf("PLUS");
                }
                else
                    insertInstructionAtEnd("fadd rax, rbx\n");

                break;
            }
            case MINUS:{

                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        sub         rax, rbx\n");
                    insertInstructionAtEnd("        push         rax\n");
                }
                else
                    insertInstructionAtEnd("fsub rax, rbx\n");


                break;
            }
            case MUL:{

                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        imul         rbx\n");
                    insertInstructionAtEnd("        push         rax\n");
                }
                else
                    insertInstructionAtEnd("imul rbx\n");

                break;
            }
            case DIV:{

                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         edx\n");
                    insertInstructionAtEnd("        mov         edx,00000000h\n");
                    insertInstructionAtEnd("        idiv         rbx\n");
                    insertInstructionAtEnd("        pop         edx\n");
                    insertInstructionAtEnd("        push         rax\n");
                }
                else
                    insertInstructionAtEnd("idiv rbx\n");


                break;
            }
            case LE:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         0fffffffh\n");
                    insertInstructionAtEnd("        cmp         rax,rbx\n");
                    snprintf(instrTemp,sizeof(instrTemp),"        jle         .rel%d\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         00000000h\n");
                    snprintf(instrTemp,sizeof(instrTemp),".rel%d:\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    relnum++;
                }
                else{

                }
                    //insertInstructionAtEnd("idiv rbx\n");

                break;
            }
            case LT:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         0fffffffh\n");
                    insertInstructionAtEnd("        cmp         rax,rbx\n");
                    snprintf(instrTemp,sizeof(instrTemp),"        jl         .rel%d\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         00000000h\n");
                    snprintf(instrTemp,sizeof(instrTemp),".rel%d:\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    relnum++;
                }
                else{

                }
                break;
            }
            case GT:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         0fffffffh\n");
                    insertInstructionAtEnd("        cmp         rax,rbx\n");
                    snprintf(instrTemp,sizeof(instrTemp),"        jg         .rel%d\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         00000000h\n");
                    snprintf(instrTemp,sizeof(instrTemp),".rel%d:\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    relnum++;
                }
                else{

                }
                break;
            }
            case GE:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         0fffffffh\n");
                    insertInstructionAtEnd("        cmp         rax,rbx\n");
                    snprintf(instrTemp,sizeof(instrTemp),"        jge         .rel%d\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         00000000h\n");
                    snprintf(instrTemp,sizeof(instrTemp),".rel%d:\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    relnum++;
                }
                else{

                }
                break;
            }
            case NE:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         0fffffffh\n");
                    insertInstructionAtEnd("        cmp         rax,rbx\n");
                    snprintf(instrTemp,sizeof(instrTemp),"        jne         .rel%d\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         00000000h\n");
                    snprintf(instrTemp,sizeof(instrTemp),".rel%d:\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    relnum++;
                }
                else{

                }
                break;
            }
            case EQ:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         0fffffffh\n");
                    insertInstructionAtEnd("        cmp         rax,rbx\n");
                    snprintf(instrTemp,sizeof(instrTemp),"        je         .rel%d\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        push         00000000h\n");
                    snprintf(instrTemp,sizeof(instrTemp),".rel%d:\n",relnum);
                    insertInstructionAtEnd(instrTemp);
                    relnum++;
                }
                else{

                }
                break;
            }
            case AND:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        and         rax, rbx\n");
                    insertInstructionAtEnd("        push         rax\n");
                }
                else
                    insertInstructionAtEnd("fadd rax, rbx\n");


                break;
            }
            case OR:{
                if(!isFloat){
                    insertInstructionAtEnd("        pop         rbx\n");
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("        or         rax, rbx\n");
                    insertInstructionAtEnd("        push         rax\n");
                }
                else
                    insertInstructionAtEnd("fadd rax, rbx\n");

                break;
            }
            case ID:{
                // add type checking
                if(!curr->ASTParent->flag && curr->ASTParent->value==whichId)
                    break;
                char type[100],scopeTemp[100],name[100];
                SymTableNode * temp=getSNode(curr->tokenLexeme,scope);

                strcpy(type,temp->type);
                strcpy(scopeTemp,temp->scope);

                sprintf(name,"%s_%s_%d",temp->tokenName,scopeTemp,temp->in_out_var);

                if(curr->ASTChild->ASTChild==NULL){
                    if(strcmp(type,"INTEGER")==0 || strcmp(type,"BOOLEAN")==0)
                        isFloat=0;
                    else if(strcmp(type,"REAL")==0 )
                        isFloat=1;
                    if(strncmp(type,"ARRAY",5)!=0){
                        snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s]\n",name);
                    }
                    else{
                        snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s_address]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s_in]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s_out]\n",name);

                    }

                    //printf("        push         qword[%s]\n",curr->tokenLexeme);
                }
                else{

                    if(strncmp(type,"ARRAY",5)==0 && (type[6]=='I' || type[6]=='B'))
                        isFloat=0;
                    else if(strncmp(type,"ARRAY",5)==0 && type[6]=='R')
                        isFloat=1;

                    char tokenType[50]; strcpy(tokenType,strtok(type, "("));
                    strcpy(tokenType,strtok(NULL, ","));
                    char tokenInd1[50];
                    strcpy(tokenInd1, strtok(NULL, ","));
                    char tokenInd2[50];
                    strcpy(tokenInd2, strtok(NULL, ")"));


                    char scopeTemp2[100],name2[100];

                    if(curr->ASTChild->ASTChild->value==NUM){
                        sprintf(name2,"%s",curr->ASTChild->ASTChild->tokenLexeme);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,%s\n",name2);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx,qword[%s_in]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        cmp         rax,rbx\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        jl        .boundError\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rcx,qword[%s_out]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        cmp         rax,rcx\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        jg        .boundError\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx,%s\n",name2);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        sub         rbx,qword[%s_in]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,qword[%s_address]\n",name);
                        insertInstructionAtEnd(instrTemp);

                        snprintf(instrTemp,sizeof(instrTemp),"        push         qword [rax + 8*rbx]\n");
                    }
                    else{
                        SymTableNode * temp2=getSNode(curr->ASTChild->ASTChild->tokenLexeme,scope);
                        sprintf(name2,"%s_%s_%d",temp2->tokenName,temp2->scope,temp2->in_out_var);

                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,qword[%s]\n",name2);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx,qword[%s_in]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        cmp         rax,rbx\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        jl        .boundError\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rcx,qword[%s_out]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        cmp         rax,rcx\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        jg        .boundError\n");
                        insertInstructionAtEnd(instrTemp);

                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rax, qword [%s]\n",name2);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rcx, qword[%s_address]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        //sprintf(name,"%s_%s",curr->tokenLexeme,scope);
                        insertInstructionAtEnd("        sub         rax,rbx\n");
                        snprintf(instrTemp,sizeof(instrTemp),"        push         qword [rcx + 8*rax]\n");
                    }

                }
                insertInstructionAtEnd(instrTemp);
                break;
            }
            case NUM:{
                if(!curr->ASTParent->flag && curr->ASTParent->value==whichId)
                    break;
                isFloat=0;
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax, %s\n",curr->tokenLexeme);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        push         rax\n");
                break;
            }
            case RNUM:{
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax, %s\n",curr->tokenLexeme);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        push         rax\n");
                isFloat=1;
                break;
            }
            case TRUE:{
                isFloat=0;
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax, 1\n");
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        push         rax\n");
                break;
            }

            case FALSE:{
                isFloat=0;
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax, 0\n");
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        push         rax\n");
                break;
            }
        }
    }

    // add cases
    generateExpressionCode(curr->ASTRight,scope);

}

void generateInstructions(TreeNode * astNode, char * scope){

    if(astNode==NULL)
        return;


        // if(astNode->flag)
        //     printf("\n %s ", terms[astNode->value]);
        // else
        //     printf("\n %s ", nterms[astNode->value]);
        //
    	// printf("Scope %s %d\n",scope,astNode->lineNumber);

    if(!astNode->flag && astNode->value==driverModule){
        strcpy(scope,"driver");
        astNode->called=1;
    }

    if(!astNode->flag && astNode->value==otherModules){
        //head=head->ASTRight;
        //astNode=astNode->ASTRight;
        generateInstructions(astNode->ASTRight,scope);
        return;
    }

    generateInstructions(astNode->ASTChild,scope);

    TreeNode * head=astNode;


    // if(astNode->flag)
    //     printf("\n %s ", terms[astNode->value]);
    // else
    //     printf("\n %s ", nterms[astNode->value]);
    //
    // printf("Scope %s\n",scope);
    //DRIVER MODULE CASE



    if(head->flag && head->value==ID && head->ASTParent->flag==0 && head->ASTParent->value==idList && head->called==0){

        if(head->ASTParent->ASTParent->ASTChild!=NULL && head->ASTParent->ASTParent->ASTChild->value==DECLARE){

            SymTableNode *stn=getSNode(head->tokenLexeme,scope);
            char name[100],type[50];
            sprintf(name,"%s_%s_%d",stn->tokenName,stn->scope,stn->in_out_var);
            strcpy(type,stn->type);

            if(stn->in_out_var==0 && stn->isArray && !stn->ifArrayDynamic){

                char tokenType[50]; strcpy(tokenType,strtok(type, "("));
                strcpy(tokenType,strtok(NULL, ","));
                char tokenInd1[50];
                strcpy(tokenInd1, strtok(NULL, ","));
                char tokenInd2[50];
                strcpy(tokenInd2, strtok(NULL, ")"));

                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,%s\n",name);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_address],rax\n",name);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,%s\n",tokenInd1);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_in],rax\n",name);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,%s\n",tokenInd2);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_out],rax\n",name);
                insertInstructionAtEnd(instrTemp);

            }
            else if(stn->in_out_var==0 && stn->isArray && stn->ifArrayDynamic){
                //printf("YES\n");
                //SymTableNode * temp=getSNode(head->tokenLexeme,scope);
                char tokenType[50]; strcpy(tokenType,strtok(type, "("));
                strcpy(tokenType,strtok(NULL, ","));
                char tokenInd1[50];
                strcpy(tokenInd1, strtok(NULL, ","));
                char tokenInd2[50];
                strcpy(tokenInd2, strtok(NULL, ")"));

                // puts(tokenInd1);
                // puts(tokenInd2); exit(0);
                SymTableNode * ind1=getSNode(tokenInd1,scope);
                SymTableNode * ind2=getSNode(tokenInd2,scope);

                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,qword[%s_%s_%d]\n",ind1->tokenName,ind1->scope,ind1->in_out_var);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_in],rax\n",name);
                insertInstructionAtEnd(instrTemp);

                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,qword[%s_%s_%d]\n",ind2->tokenName,ind2->scope,ind2->in_out_var);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_out],rax\n",name);
                insertInstructionAtEnd(instrTemp);

                snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,qword[nextAddress]\n");
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_address],rax\n",name);
                insertInstructionAtEnd(instrTemp);

                // snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx,nextAddress\n");
                // insertInstructionAtEnd(instrTemp);
                // snprintf(instrTemp,sizeof(instrTemp),"        add         rax,rbx\n");
                // insertInstructionAtEnd(instrTemp);
                // snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_address],rax\n",name);
                // insertInstructionAtEnd(instrTemp);

                snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s_out]\n",name);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s_in]\n",name);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        pop         rbx\n");
                insertInstructionAtEnd("        pop         rax\n");
                insertInstructionAtEnd("        sub         rax,rbx\n");
                insertInstructionAtEnd("        add         rax,1\n");
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx,8\n");
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        imul         rbx\n");

                snprintf(instrTemp,sizeof(instrTemp),"        add         rax,qword[%s_address]\n",name);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        mov         qword[nextAddress],rax\n");

            }
        }
        head->called=1;
    }

    // MODULE CASE
    if(head->flag && strcmp(terms[head->value],"ID")==0 && !head->ASTParent->flag && strcmp(nterms[head->ASTParent->value],"module")==0 && head->called==0){
        //strcpy(scope,head->tokenLexeme);//scopechange
        //head->called=1;
    }

    // FOR CASE
    if(head->flag && strcmp(terms[head->value],"FOR")==0 && !head->called){

        int numf=fornum;
        fornum++;
        SymTableNode * temp=getSNode(head->ASTRight->tokenLexeme,scope);
        char name[100],type[50];
        sprintf(name,"%s_%s_%d",temp->tokenName,temp->scope,temp->in_out_var);
        strcpy(type,temp->type);

        char num1[50],num2[50];
        strcpy(num1,head->ASTRight->ASTRight->ASTChild->tokenLexeme);
        strcpy(num2,head->ASTRight->ASTRight->ASTChild->ASTRight->tokenLexeme);
        int end,start;
        sscanf(num1,"%d",&start);
        sscanf(num2,"%d",&end);
        //puts(num2);

        //char instr[200];
        snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s],%s\n",name,num1);
        insertInstructionAtEnd(instrTemp);
        // snprintf(instrTemp,sizeof(instrTemp),".for%d:\n",fornum);
        // insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp),".for%d:\n",numf);
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp),"        push         qword[%s]\n",name);
        insertInstructionAtEnd(instrTemp);
        strcpy(instrTemp,"        pop         rax\n");
        insertInstructionAtEnd(instrTemp);

        snprintf(instrTemp,sizeof(instrTemp), "        mov         rcx, %d\n", start);
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp), "        cmp         rax, rcx\n");
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp), "         jl          .endfor%d\n", numf);
        insertInstructionAtEnd(instrTemp);

        snprintf(instrTemp,sizeof(instrTemp), "        mov         rcx, %d\n", end);
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp), "        cmp         rax, rcx\n");
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp), "        jg      .endfor%d\n", numf);
        insertInstructionAtEnd(instrTemp);
        //if id is between num1 and num2 --code for stmts
        char newscope[50];
        sprintf(newscope,"FOR_%d",head->lineNumber);//scopechange
        //printf("%s\n",newscope);

        generateInstructions(head->ASTRight->ASTRight->ASTRight,newscope);
        //return;
        snprintf(instrTemp,sizeof(instrTemp),"          inc        qword[%s]\n",name);
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp),"          jmp         .for%d\n", numf);
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp),".endfor%d:\n", numf);
        insertInstructionAtEnd(instrTemp);
        //fornum++;
        head->called=1;
    }

    // WHILE STATEMENTS
    if(head->flag==1 && strcmp(terms[head->value],"WHILE")==0 && head->called==0){
        //char instr[200];
        int numw=whilenum;
        whilenum++;
        snprintf(instrTemp,sizeof(instrTemp),".while%d:\n",numw);
        insertInstructionAtEnd(instrTemp);
        char newscope[50];
        snprintf(newscope,sizeof(newscope),"WHILE_%d",head->lineNumber);//scopechange
        generateExpressionCode(head->ASTRight,newscope);
        snprintf(instrTemp,sizeof(instrTemp),"        pop         rax\ncmp rax,0000h\nje .endwhile%d\n", numw);
        insertInstructionAtEnd(instrTemp);
        generateInstructions(head->ASTRight->ASTRight,newscope);

        snprintf(instrTemp,sizeof(instrTemp),"jmp .while%d\n", numw);
        insertInstructionAtEnd(instrTemp);
        snprintf(instrTemp,sizeof(instrTemp),".endwhile%d:\n", numw);
        insertInstructionAtEnd(instrTemp);
        //whilenum++;
        head->called=1;
    }

    //PRINT STATEMENTS
    if(head->flag==1 && strcmp(terms[head->value],"PRINT")==0 && head->called==0){
        //char instr[200];
        TreeNode * temp=head->ASTRight->ASTChild;

        insertInstructionAtEnd("        lea        rdi,[writeformat]\n");
        insertInstructionAtEnd("        mov        rsi,0\n");
        insertInstructionAtEnd("        xor        rax,rax\n");
        insertInstructionAtEnd("        call        printf\n");



        if(temp->flag==1 && strcmp(terms[temp->value],"NUM")==0){

            snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, %s\n",temp->tokenLexeme );
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
            insertInstructionAtEnd(instrTemp);
        }
        else if(temp->flag==1 && strcmp(terms[temp->value],"RNUM")==0){

            snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, %s\n", temp->tokenLexeme);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
            insertInstructionAtEnd(instrTemp);
        }
        else if(temp->flag==1 && strcmp(terms[temp->value],"TRUE")==0){

            snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [truem]\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, 0\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
            insertInstructionAtEnd(instrTemp);
        }
        else if(temp->flag==1 && strcmp(terms[temp->value],"FALSE")==0){

            snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [falsem]\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, 0\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
            insertInstructionAtEnd(instrTemp);
        }
        else {
            //id
            //printf("YES\n");
            SymTableNode * temp2=getSNode(temp->tokenLexeme,scope);
            char name[100],type[50];
            sprintf(name,"%s_%s_%d",temp2->tokenName,temp2->scope,temp2->in_out_var);
            strcpy(type,temp2->type);

            if(temp->ASTChild->ASTChild==NULL){

                if(strncmp(type,"ARRAY",5)!=0 ){  // NOT ARRAY TYPE

                    if(strcmp(type,"BOOLEAN")==0){

                        snprintf(instrTemp,sizeof(instrTemp),"        mov         rax,qword[%s]\n",name);
                        insertInstructionAtEnd(instrTemp);
                        insertInstructionAtEnd("        cmp         rax,1\n");
                        snprintf(instrTemp,sizeof(instrTemp),"        je         .boolOut%d\n",labelnum);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [falsem]\n");
                		insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        jmp         .boolOutEnd%d\n",labelnum);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), ".boolOut%d:\n",labelnum);
                        insertInstructionAtEnd(instrTemp);
                		snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [truem]\n");
                		insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), ".boolOutEnd%d:\n",labelnum);
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, 0\n");
                        insertInstructionAtEnd(instrTemp);
                        labelnum++;
                    }
                    else{
                        snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
                        insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, [%s]\n", name);
                        insertInstructionAtEnd(instrTemp);
                    }

                    snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
                    insertInstructionAtEnd(instrTemp);
                }
                // PRINTING AN ARRAY
                else{

                    snprintf(instrTemp,sizeof(instrTemp),"        mov        rax,qword[%s_in]\n",name);
                    insertInstructionAtEnd(instrTemp);
                    insertInstructionAtEnd("        mov qword[loopVar],rax\n");
                    snprintf(instrTemp,sizeof(instrTemp),".for%d:\n",fornum);
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp),"        push         qword[loopVar]\n");
                    insertInstructionAtEnd(instrTemp);
                    strcpy(instrTemp,"        pop         rax\n");
                    insertInstructionAtEnd(instrTemp);

                    snprintf(instrTemp,sizeof(instrTemp), "        mov         rcx, qword[%s_in]\n", name); //start index
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        cmp         rax, rcx\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "         jl          .endfor%d\n", fornum);
                    insertInstructionAtEnd(instrTemp);

                    snprintf(instrTemp,sizeof(instrTemp), "        mov         rcx, qword[%s_out]\n",name); //out index
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        cmp         rax, rcx\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        jg      .endfor%d\n", fornum);
                    insertInstructionAtEnd(instrTemp);

                    // instructions
                    snprintf(instrTemp,sizeof(instrTemp), "        mov         rbx, qword[%s_in]\n",name); //start ind
                    insertInstructionAtEnd(instrTemp);

                    snprintf(instrTemp,sizeof(instrTemp), "        mov         rax, qword[loopVar]\n"); // rax->current index
                    insertInstructionAtEnd(instrTemp);

                    insertInstructionAtEnd("        sub         rax,rbx\n");
                    insertInstructionAtEnd("        mov         rbx,rax\n");

                    snprintf(instrTemp,sizeof(instrTemp), "        mov         rax, qword[%s_address]\n",name); //dynmaiv array address
                    insertInstructionAtEnd(instrTemp);


                    snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [formatArray]\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, [rax+rbx*8]\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
                    insertInstructionAtEnd(instrTemp);

                    snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
                    insertInstructionAtEnd(instrTemp);

                    // instrcutions end

                    snprintf(instrTemp,sizeof(instrTemp),"          inc        qword[loopVar]\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp),"          jmp         .for%d\n", fornum);
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp),".endfor%d:\n", fornum);
                    insertInstructionAtEnd(instrTemp);
                    fornum++;

                }

            }
            //mov rbx, [array+1*4]
            else if(temp->ASTChild->ASTChild->flag==1 && strcmp(terms[temp->ASTChild->ASTChild->value],"NUM")==0){
                //printf("YESSSS\n");
                SymTableNode * temp3=getSNode(temp->ASTChild->ASTChild->tokenLexeme,scope);
                char name2[100];
                sprintf(name2,"%s",temp3->tokenName);

                snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        mov         rax, qword[%s_address]\n",name); //dynmaiv array address
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        mov         rbx, qword[%s_in]\n",name); //dynmaiv array address
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi, [rax+(%s-rbx)*8]\n",name2);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
                insertInstructionAtEnd(instrTemp);

            }

            else { //id[id]

                SymTableNode * temp3=getSNode(temp->ASTChild->ASTChild->tokenLexeme,scope);
                char name2[100];

                sprintf(name2,"%s_%s_%d",temp3->tokenName,temp3->scope,temp3->in_out_var);

                snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        mov         rax, qword[%s_address]\n",name); //dynmaiv array address
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        mov         rbx, qword[%s]\n",name2); //dynmaiv array address
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rdx, qword [%s_in]\n",name);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        sub         rbx,rdx\n");
                snprintf(instrTemp,sizeof(instrTemp),"       mov        rsi, qword [rax + 8*rbx]\n");
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "        call        printf\n");
                insertInstructionAtEnd(instrTemp);

            }
        }

        insertInstructionAtEnd("        lea        rdi,[newLine]\n");
        insertInstructionAtEnd("        mov        rsi,0\n");
        insertInstructionAtEnd("        xor        rax,rax\n");
        insertInstructionAtEnd("        call        printf\n");

        head->called=1;
    }

    // GET VALUE CASE
    if(head->flag==1 && strcmp(terms[head->value],"GET_VALUE")==0 && head->called==0){
        //char instrTemp[200];
        //printf("%s,\n", scope);
        TreeNode* temp=head->ASTRight;

        SymTableNode * temp2=getSNode(temp->tokenLexeme,scope);
        char name[100],type[50];
        sprintf(name,"%s_%s_%d",temp2->tokenName,temp2->scope,temp2->in_out_var);
        strcpy(type,temp2->type);

        if(strncmp(type,"ARRAY",5)!=0 ){

        	if(strcmp(type,"INTEGER")==0){
        		snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [enterint]\n");
        		insertInstructionAtEnd(instrTemp);}

        	else if(strcmp(type,"REAL")==0){

        		snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [entereal]\n");
        		insertInstructionAtEnd(instrTemp);
        	}
        	else {
        		snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [enterbool]\n");
        		insertInstructionAtEnd(instrTemp);
        	}

            insertInstructionAtEnd("        mov        rsi,0\n");
            insertInstructionAtEnd("        xor        rax,rax\n");
            insertInstructionAtEnd("        call        printf\n");

            snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
            insertInstructionAtEnd(instrTemp);

            snprintf(instrTemp,sizeof(instrTemp), "        lea        rsi, [%s]\n", name);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
            insertInstructionAtEnd(instrTemp);
        	snprintf(instrTemp,sizeof(instrTemp), "        call        scanf\n");
            insertInstructionAtEnd(instrTemp);

        }
        else { //id is an array

            snprintf(instrTemp,sizeof(instrTemp), "        mov        rax,qword[%s_out]\n",name);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rbx,qword[%s_in]\n",name);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        add        rax,1\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        sub        rax,rbx\n");
            insertInstructionAtEnd(instrTemp);

            if(type[6]=='I'){
                //snprintf(instrTemp,sizeof(instrTemp), "        mov        rdi,\"Enter %d integer values from %d to %d\" \n",range,bound_in,bound_out);
                snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [enterintArr]\n");
                insertInstructionAtEnd(instrTemp);
            }
            else if(type[6]=='R'){
                //snprintf(instrTemp,sizeof(instrTemp), "        mov        rdi,\"Enter %d real values from %d to %d\" \n",range,bound_in,bound_out);
                snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [enterrealArr]\n");
                insertInstructionAtEnd(instrTemp);
            }
            else {
                //snprintf(instrTemp,sizeof(instrTemp), "        mov        rdi,\"Enter %d boolean values from %d to %d\" \n",range,bound_in,bound_out);
                snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [enterboolArr]\n");
                insertInstructionAtEnd(instrTemp);

            }

            snprintf(instrTemp,sizeof(instrTemp), "        mov        rsi,rax\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rdx,qword[%s_in]\n",name);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        mov        rcx,qword[%s_out]\n",name); // CHANGE HERE
            insertInstructionAtEnd(instrTemp);

            insertInstructionAtEnd("        xor        rax,rax\n");
            insertInstructionAtEnd("        call        printf\n");

            // int ind=0; //doubt?? how to specify name for successive array elements
            // while(range){
            //     snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
            //     insertInstructionAtEnd(instrTemp);
            // 	snprintf(instrTemp,sizeof(instrTemp), "        lea        rsi, [%s+%d*8]\n", name,ind);
        	// 	insertInstructionAtEnd(instrTemp);
        	// 	snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
        	// 	insertInstructionAtEnd(instrTemp);
            //
    		// 	snprintf(instrTemp,sizeof(instrTemp), "        call        scanf\n");
        	// 	insertInstructionAtEnd(instrTemp);
        	// 	range--;ind++;
            // }

            snprintf(instrTemp,sizeof(instrTemp),"        mov        rax,qword[%s_in]\n",name);
            insertInstructionAtEnd(instrTemp);
            insertInstructionAtEnd("        mov qword[loopVar],rax\n");
            snprintf(instrTemp,sizeof(instrTemp),".for%d:\n",fornum);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp),"        push         qword[loopVar]\n");
            insertInstructionAtEnd(instrTemp);
            strcpy(instrTemp,"        pop         rax\n");
            insertInstructionAtEnd(instrTemp);

            snprintf(instrTemp,sizeof(instrTemp), "        mov         rcx, qword[%s_in]\n", name); //start index
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        cmp         rax, rcx\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "         jl          .endfor%d\n", fornum);
            insertInstructionAtEnd(instrTemp);

            snprintf(instrTemp,sizeof(instrTemp), "        mov         rcx, qword[%s_out]\n",name); //out index
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        cmp         rax, rcx\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        jg      .endfor%d\n", fornum);
            insertInstructionAtEnd(instrTemp);

            // instructions
            snprintf(instrTemp,sizeof(instrTemp), "        mov         rbx, qword[%s_in]\n",name); //start ind
            insertInstructionAtEnd(instrTemp);

            snprintf(instrTemp,sizeof(instrTemp), "        mov         rax, qword[loopVar]\n"); // rax->current index
            insertInstructionAtEnd(instrTemp);

            insertInstructionAtEnd("        sub         rax,rbx\n");
            insertInstructionAtEnd("        mov         rbx,rax\n");

            snprintf(instrTemp,sizeof(instrTemp), "        mov         rax, qword[%s_address]\n",name); //dynmaiv array address
            insertInstructionAtEnd(instrTemp);


            snprintf(instrTemp,sizeof(instrTemp), "        lea        rdi, [format]\n");
            insertInstructionAtEnd(instrTemp);

        	snprintf(instrTemp,sizeof(instrTemp), "        lea        rsi, [rax+8*rbx]\n");
    		insertInstructionAtEnd(instrTemp);
    		snprintf(instrTemp,sizeof(instrTemp), "        xor        rax, rax\n");
    		insertInstructionAtEnd(instrTemp);

			snprintf(instrTemp,sizeof(instrTemp), "        call        scanf\n");
    		insertInstructionAtEnd(instrTemp);

            // instrcutions end

            snprintf(instrTemp,sizeof(instrTemp),"          inc        qword[loopVar]\n");
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp),"          jmp         .for%d\n", fornum);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp),".endfor%d:\n", fornum);
            insertInstructionAtEnd(instrTemp);
            fornum++;

        }
        head->called=1;
    }
    // make rdits here

    if(!head->flag && strcmp(nterms[head->value],"assignmentStmt")==0 && head->called==0){
        //printf("Scope qww %s\n",scope);
        //printf("aaaa\n");
        char lhs[50],type[50];
        SymTableNode * temp1=getSNode(head->ASTChild->tokenLexeme,scope);
        //printf(" %s \n",temp1->tokenName);
        sprintf(lhs,"%s_%s_%d",temp1->tokenName,temp1->scope,temp1->in_out_var);
        strcpy(type,temp1->type);
        TreeNode * temp=head->ASTChild->ASTRight;
        //printf("HELLLLOOO %s\n",scope);

        if(temp->flag==0 && strcmp(nterms[temp->value],"lvalueIDStmt")==0 && temp->called==0){
            //printf("HERE\n");
            if(!temp->ASTChild->flag && strcmp(nterms[temp->ASTChild->value],"arithmeticOrBooleanExpr")==0){
                //printf("Inside expression code\n");
                //printf("HELLLLOOO1 %s\n",scope);
                generateExpressionCode(temp->ASTChild->ASTChild,scope);
            }
            if(!temp->ASTChild->flag && strcmp(nterms[temp->ASTChild->value],"U")==0){
                //printf("Inside unary expression code\n");
                //printf("YES");
                char sign[2];
                //puts(temp->ASTChild->ASTChild->tokenLexeme);
                strcpy(sign,temp->ASTChild->ASTChild->tokenLexeme);
                generateExpressionCode(temp->ASTChild->ASTChild->ASTRight,scope);
                if(strcmp(sign,"+")==0){
                    insertInstructionAtEnd("        pop         rax\n");
                }
                else if(strcmp(sign,"-")==0){
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("neg rax\n");
                }
            }
            //insertInstructionAtEnd("        pop         rax\n");
            if(strncmp(type,"ARRAY",5)!=0 ){
                insertInstructionAtEnd("        pop         rax\n");
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s],rax\n",lhs);
                insertInstructionAtEnd(instrTemp);
            }
            else{
                insertInstructionAtEnd("        pop         rax\n");
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_out],rax\n",lhs);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        pop         rax\n");
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_in],rax\n",lhs);
                insertInstructionAtEnd(instrTemp);
                insertInstructionAtEnd("        pop         rax\n");
                snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[%s_address],rax\n",lhs);
                insertInstructionAtEnd(instrTemp);
            }

            temp->called=1;
        }

        if(temp->flag==0 && strcmp(nterms[temp->value],"lvalueARRStmt")==0 && temp->called==0){
            //printf(" HERE %s \n",temp1->tokenName);
            char ind[50];
            if(temp->ASTChild->value==ID){
                SymTableNode * temp2=getSNode(temp->ASTChild->tokenLexeme,scope);
                sprintf(ind,"%s_%s_%d",temp2->tokenName,temp2->scope,temp2->in_out_var);
            }
            else
                sprintf(ind,"%s",temp->ASTChild->tokenLexeme);
            //printf(" %s \n",temp1->tokenName);

            //printf(" %s \n",temp1->tokenName);
            // char tokenType[50]; strcpy(tokenType,strtok(type, "("));
            // strcpy(tokenType,strtok(NULL, ","));
            // char tokenInd1[50];
            // strcpy(tokenInd1, strtok(NULL, ","));
            // char tokenInd2[50];
            // strcpy(tokenInd2, strtok(NULL, ")"));
            //strcpy(ind,temp->ASTChild->tokenLexeme);
            //TreeNode* temp2=head->ASTParent->ASTChild;

            if(!temp->ASTChild->ASTRight->flag && strcmp(nterms[temp->ASTChild->ASTRight->value],"arithmeticOrBooleanExpr")==0){
                //printf("Inside expression code\n");
                generateExpressionCode(temp->ASTChild->ASTRight->ASTChild,scope);
                insertInstructionAtEnd("        pop         rax\n");

            }
            if(!temp->ASTChild->ASTRight->flag && strcmp(nterms[temp->ASTChild->ASTRight->value],"U")==0){
                //printf("Inside unary expression code\n");
                char sign[20];
                strcpy(sign,temp->ASTChild->ASTRight->ASTRight->tokenLexeme);
                generateExpressionCode(temp->ASTChild->ASTRight->ASTChild->ASTRight,scope);
                if(strcmp(sign,"PLUS")==0){
                    insertInstructionAtEnd("        pop         rax\n");
                }
                else if(strcmp(sign,"MINUS")==0){
                    insertInstructionAtEnd("        pop         rax\n");
                    insertInstructionAtEnd("neg rax\n");
                }
            }
            //insertInstructionAtEnd("        pop rax\n");
            if(temp->ASTChild->value==NUM){
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx, %s\n",ind);
                insertInstructionAtEnd(instrTemp);
            }
            else{
                snprintf(instrTemp,sizeof(instrTemp),"        mov         rbx, qword[%s]\n",ind);
                insertInstructionAtEnd(instrTemp);
            }

            snprintf(instrTemp,sizeof(instrTemp),"        mov         rcx, qword[%s_address]\n",lhs);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp),"        sub         rbx, qword[%s_in]\n",lhs);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp),"        mov         qword[rcx + 8*rbx],rax\n");
            insertInstructionAtEnd(instrTemp);
            temp->called=1;

        }
        //printf("HELLLLOOO2 %s\n",scope);
        //generateInstructions(head->ASTChild->ASTRight,scope);
        head->called=1;
    }

    // if(head->flag && head->value==CASE && head->parent->flag==0 && head->parent->value==caseStmts){
    //     printf("YESSS\n");
    //     return;
    // }

    // SWITCH CASE
    if(head->flag==1 &&  strcmp(terms[head->value],"SWITCH")==0 && head->called==0){
        //char instrTemp[200];
        TreeNode* idn=head->ASTRight;

        SymTableNode * temp= getSNode(idn->tokenLexeme,scope);
        char name[100];
        sprintf(name,"%s_%s_%d",temp->tokenName,temp->scope,temp->in_out_var);
        snprintf(instrTemp,sizeof(instrTemp), "        mov r8, [%s]\n", name);
        insertInstructionAtEnd(instrTemp);
        char newscope[50];
        snprintf(newscope,sizeof(newscope),"SWITCH_%d",head->lineNumber);//scopechange
        char typen[50];
        strcpy(typen,temp->type);

        if(strcmp(typen,"INTEGER")==0){
           //nprintf(instrTemp,sizeof(instrTemp), "        and r8, 00000000000000ffh\n");
           //insertInstructionAtEnd(instrTemp);
		   snprintf(instrTemp,sizeof(instrTemp), "        push r8\n");
           insertInstructionAtEnd(instrTemp);

           TreeNode* case_s=head->ASTRight->ASTRight->ASTChild;
           TreeNode* num_s;
           TreeNode* stmt_s;//call with newscope
           int currentLabel = getlabel();
           int endLabel = getlabel();
           int nextLabel;


            while(case_s){
                num_s=case_s->ASTRight;
                stmt_s=num_s->ASTRight;

                //printf("Scope %s\n",scope);

                nextLabel = getlabel();
                snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", currentLabel);
                insertInstructionAtEnd(instrTemp);
    			snprintf(instrTemp,sizeof(instrTemp), "        pop r8\n");
                insertInstructionAtEnd(instrTemp);
    		    snprintf(instrTemp,sizeof(instrTemp), "        push r8\n");
                insertInstructionAtEnd(instrTemp);
    			snprintf(instrTemp,sizeof(instrTemp), "        mov r9, %s\n", num_s->tokenLexeme);
                insertInstructionAtEnd(instrTemp);
    		    snprintf(instrTemp,sizeof(instrTemp), "        cmp r8,r9\n");
                insertInstructionAtEnd(instrTemp);
    			snprintf(instrTemp,sizeof(instrTemp), "        jne .ConditionalLabel%d\n", nextLabel);
                insertInstructionAtEnd(instrTemp);

                generateInstructions(stmt_s->ASTChild,newscope);

                snprintf(instrTemp,sizeof(instrTemp), "        jmp .ConditionalLabel%d\n", endLabel);
                insertInstructionAtEnd(instrTemp);//taking care of BREAK
                currentLabel=nextLabel;
                case_s=case_s->ASTRight->ASTRight->ASTRight;

            }

            TreeNode* def_stmt=idn->ASTRight->ASTRight->ASTChild;
            snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", currentLabel);
            insertInstructionAtEnd(instrTemp);
            generateInstructions(def_stmt,newscope);
            snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", endLabel);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        pop r8\n");
            insertInstructionAtEnd(instrTemp);
        }

        else{//index is BOOLEAN
            //snprintf(instrTemp,sizeof(instrTemp), "        and r8, 0000000000000001h\n");
            //insertInstructionAtEnd(instrTemp); //true in r8
    		snprintf(instrTemp,sizeof(instrTemp), "        push r8\n");
            insertInstructionAtEnd(instrTemp);

            TreeNode* case_s=head->ASTRight->ASTRight->ASTChild;
            TreeNode* bool_s;
            TreeNode* stmt_s;//call with newscope
            int currentLabel = getlabel();
            int endLabel = getlabel();
            int nextLabel;

            while(case_s){
                bool_s=case_s->ASTRight;
                stmt_s=bool_s->ASTRight;
                nextLabel = getlabel();
                if(strcmp(terms[bool_s->value],"TRUE")==0){
                    snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", currentLabel);
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        pop r8\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        push r8\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov r9, 0000000000000001h\n");
                    insertInstructionAtEnd(instrTemp);
                }
                else{
                    snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", currentLabel);
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        pop r8\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        push r8\n");
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov r9, 0000000000000000h\n");
                    insertInstructionAtEnd(instrTemp);
                }

                snprintf(instrTemp,sizeof(instrTemp), "        cmp r8,r9\n");
                insertInstructionAtEnd(instrTemp);
    			snprintf(instrTemp,sizeof(instrTemp), "        jne .ConditionalLabel%d\n", nextLabel);
                insertInstructionAtEnd(instrTemp);

                generateInstructions(stmt_s,newscope);

                snprintf(instrTemp,sizeof(instrTemp), "        jmp .ConditionalLabel%d\n",endLabel);
                insertInstructionAtEnd(instrTemp);
                currentLabel=nextLabel;
                case_s=case_s->ASTRight->ASTRight->ASTRight;

            }

            snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", nextLabel);
            insertInstructionAtEnd(instrTemp);
    		snprintf(instrTemp,sizeof(instrTemp), ".ConditionalLabel%d:\n", endLabel);
            insertInstructionAtEnd(instrTemp);
            snprintf(instrTemp,sizeof(instrTemp), "        pop r8\n");
            insertInstructionAtEnd(instrTemp);


        }

        head->called=1;
    }//switch ends

    // MODULE REUSE STATEMENT
    if(head->flag==0 && strcmp(nterms[head->value],"moduleReuseStmt")==0 && head->called==0){

        TreeNode* id_m=head->ASTChild->ASTRight;
        //printf("%s\n",id_m->tokenLexeme);
        FuncTableNode * function=getFuncTableEntry(id_m->tokenLexeme);
        //if(function==NULL)
            //printf("YYYY\n");
        insertInstructionAtEnd("        push        rdi\n");
        insertInstructionAtEnd("        push        rax\n");

        TreeNode * id_list=head->ASTChild->ASTRight->ASTRight->ASTChild;
        TreeNode * out_list=head->ASTChild->ASTChild->ASTChild;


        for(int i=0;i<function->numInputParam;i++){
            SymTableNode * stn=getSNode(id_list->tokenLexeme,scope);
            if(stn!=NULL){
                if(!stn->isArray){
                    snprintf(instrTemp,sizeof(instrTemp), "         mov         rax,qword[%s_%s_%d]\n",stn->tokenName,stn->scope,stn->in_out_var);
                    insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "         mov         qword[%s_%s_1],rax\n",function->inputParam[i],id_m->tokenLexeme);
                    insertInstructionAtEnd(instrTemp);
                }
                else{
                    char name[100],type[100];
                    sprintf(name,"%s_%s_%d",stn->tokenName,stn->scope,stn->in_out_var);
                    SymTableNode * modP=getSNode(function->inputParam[i],id_m->tokenLexeme);
                    strcpy(type,modP->type);

                    if(stn->ifArrayDynamic &&  !modP->ifArrayDynamic){
                        char tokenType[50]; strcpy(tokenType,strtok(type, "("));
                        strcpy(tokenType,strtok(NULL, ","));
                        char tokenInd1[50];
                        strcpy(tokenInd1, strtok(NULL, ","));
                        char tokenInd2[50];
                        strcpy(tokenInd2, strtok(NULL, ")"));

                        snprintf(instrTemp,sizeof(instrTemp), "        mov        rax, qword[%s_in]\n",name);
                		insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        mov        rbx, %s\n",tokenInd1);
                		insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        cmp        rax, rbx\n");
                		insertInstructionAtEnd(instrTemp);
                        insertInstructionAtEnd("        jne     .boundError\n");
                        snprintf(instrTemp,sizeof(instrTemp), "        mov        rax, qword[%s_out]\n",name);
                		insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        mov        rbx, %s\n",tokenInd2);
                		insertInstructionAtEnd(instrTemp);
                        snprintf(instrTemp,sizeof(instrTemp), "        cmp        rax, rbx\n");
                		insertInstructionAtEnd(instrTemp);
                        insertInstructionAtEnd("        jne     .boundError\n");

                    }

                    snprintf(instrTemp,sizeof(instrTemp), "        mov        rax, qword[%s_address]\n",name);
            		insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov        [%s_%s_1_address],rax\n",function->inputParam[i],id_m->tokenLexeme);
            		insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov        rax, qword[%s_in]\n",name);
            		insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov        [%s_%s_1_in],rax\n",function->inputParam[i],id_m->tokenLexeme);
            		insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov        rax, qword[%s_out]\n",name);
            		insertInstructionAtEnd(instrTemp);
                    snprintf(instrTemp,sizeof(instrTemp), "        mov        [%s_%s_1_out],rax\n",function->inputParam[i],id_m->tokenLexeme);
            		insertInstructionAtEnd(instrTemp);



                }
            }

            id_list=id_list->ASTRight;
        }
        //exit(0);
        snprintf(instrTemp,sizeof(instrTemp), "        call %s\n", id_m->tokenLexeme);
        insertInstructionAtEnd(instrTemp);

        for(int i=0;i<function->numOutputParam;i++){
            SymTableNode * stn=getSNode(out_list->tokenLexeme,scope);
            if(stn!=NULL){

                snprintf(instrTemp,sizeof(instrTemp), "         mov         rax,qword[%s_%s_2]\n",function->outputParam[i],id_m->tokenLexeme);
                insertInstructionAtEnd(instrTemp);
                snprintf(instrTemp,sizeof(instrTemp), "         mov         qword[%s_%s_%d],rax\n",stn->tokenName,stn->scope,stn->in_out_var);
                insertInstructionAtEnd(instrTemp);
            }

            out_list=out_list->ASTRight;
        }


        insertInstructionAtEnd("        pop       rax\n");

        insertInstructionAtEnd("        pop        rdi\n");
        //output list??
        head->called=1;

    }

    // if(head->flag==0 && strcmp(nterms[head->value],"statements")==0 && head->ASTParent->flag==0 && strcmp(nterms[head->ASTParent->value],"module")==0 && head->called==0){
    //
    //     char newscope[50];
    //     strcpy(newscope,head->ASTParent->ASTChild->tokenLexeme);
    //     char label[50];
    //     strcpy(label,newscope);
    //     snprintf(instrTemp,sizeof(instrTemp), "%s:\n",label);
    //     insertInstructionAtEnd(instrTemp);//label of the functions
    //     snprintf(instrTemp,sizeof(instrTemp), "        push rbp\n");
    //     insertInstructionAtEnd(instrTemp);
    //     snprintf(instrTemp,sizeof(instrTemp), "        mov rbp, rsp\n");
    //     insertInstructionAtEnd(instrTemp);
    //     //loading parameters from caller's stack?? the actual parameters
    //     generateInstructions(head,newscope);
    //     snprintf(instrTemp,sizeof(instrTemp), "        pop rbp\n");
    //     insertInstructionAtEnd(instrTemp);
    //     snprintf(instrTemp,sizeof(instrTemp), "        mov rsp, rbp\n");
    //     insertInstructionAtEnd(instrTemp);
    //     snprintf(instrTemp,sizeof(instrTemp), "        ret\n");
    //     insertInstructionAtEnd(instrTemp);
    //
    //     head->called=1;
    // }



    generateInstructions(astNode->ASTRight,scope);
}

void addProcedures(TreeNode * astNode, char * scope){

    if(astNode==NULL)
        return;
    addProcedures(astNode->ASTChild,scope);

    TreeNode * head= astNode;

    if(head->flag && strcmp(terms[head->value],"ID")==0 && !head->ASTParent->flag && strcmp(nterms[head->ASTParent->value],"module")==0 && head->called==0){

        strcpy(scope,head->tokenLexeme);//scopechange

        snprintf(instrTemp,sizeof(instrTemp), "%s:\n",head->tokenLexeme);
        insertInstructionAtEnd(instrTemp);
        //int count=8;

        insertInstructionAtEnd("        push         rbx\n");
        //insertInstructionAtEnd("        push        rbp\n");

        generateInstructions(head->ASTRight->ASTRight->ASTRight,scope);
        //pushonstack(head->ASTRight->ASTRight->ASTChild,scope);

        //insertInstructionAtEnd("        pop         rbp\n");
        insertInstructionAtEnd("        pop        rbx\n");

        insertInstructionAtEnd("        ret\n");
        insertInstructionAtEnd(".boundError:\n");
        insertInstructionAtEnd("        lea        rdi,[bounderror]\n");
        insertInstructionAtEnd("        mov        rsi,0\n");
        insertInstructionAtEnd("        xor        rax,rax\n");
        insertInstructionAtEnd("        call        printf\n");
        insertInstructionAtEnd("        mov         rax,60\n");
        insertInstructionAtEnd("        xor         rdi,rdi\n");
        insertInstructionAtEnd("        call         exit\n");
        insertInstructionAtEnd("        pop rbx\n");

        head->called=1;
    }

    addProcedures(astNode->ASTRight,scope);
    return;
}

void generateAssemblyCode(TreeNode * astNode, char * scope){
    generateHeaderCode();
    generateInstructions(astNode,scope);
    insertInstructionAtEnd("        pop rbx\n        ret\n");
    insertInstructionAtEnd(".boundError:\n");
    insertInstructionAtEnd("        lea        rdi,[bounderror]\n");
    insertInstructionAtEnd("        mov        rsi,0\n");
    insertInstructionAtEnd("        xor        rax,rax\n");
    insertInstructionAtEnd("        call        printf\n");
    insertInstructionAtEnd("        mov         rax,60\n");
    insertInstructionAtEnd("        xor         rdi,rdi\n");
    insertInstructionAtEnd("        call         exit\n");
    insertInstructionAtEnd("        pop rbx\n");
    addProcedures(astNode,scope);

}
