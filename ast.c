//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
//gcc 5.4.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.c"
/*
char *nterms[]={"program","moduleDeclarations","otherModules","driverModule","moduleDeclaration","module","moduleDef","input_plist","ret","dataType","N1","output_plist","type","N2","range_arrays","index1","statements","statement","ioStmt","simpleStmt","declareStmt","conditionalStmt","iterativeStmt","var","var_id_num","whichId","boolConstt","assignmentStmt","moduleReuseStmt","whichStmt","lvalueIDStmt","lvalueARRStmt","expression","optional","idList","N3","arithmeticOrBooleanExpr","U","new_NT","arithmeticExpr","unary_op","AnyTerm","N7","logicalOp","N8","relationalOp","op1","op2","term","N4","factor","N5","caseStmts","default1","N9","value","range"};
char *terms[]={"EPS","DECLARE","MODULE","ID","SEMICOL","DRIVERDEF","DRIVER","PROGRAM","DRIVERENDDEF","DEF","ENDDEF","TAKES","INPUT","SQBO","SQBC","RETURNS","COLON","COMMA","INTEGER","REAL","BOOLEAN","ARRAY","OF","RANGEOP","START","END","GET_VALUE","BO","BC","PRINT","NUM","RNUM","TRUE","FALSE","ASSIGNOP","USE","WITH","PARAMETERS","PLUS","MINUS","MUL","DIV","AND","OR","LT","LE","GT","GE","NE","EQ","SWITCH","DEFAULT","BREAK","FOR","WHILE","CASE", "IN", "DOLLAR"};
typedef struct treeNode{
    int value;
    int flag;
    char tokenName[30];
    char tokenLexeme[30];
    int lineNumber;
    struct treeNode* right;
    struct treeNode* child;
    struct treeNode* parent;
    struct treeNode* ASTChild;//to be initialized with NULL
    struct treeNode* ASTRight;//to be initialized with NULL
    //pointer to entry in ST
}TreeNode;
*/
//void makeAst(TreeNode* root);
/*
int main(){
TreeNode* test=(TreeNode*)malloc(sizeof(TreeNode));
test->flag=1;
test->value=2;
test->right=test->child=test->parent=test->ASTChild=test->ASTRight=NULL;
makeAst(test);
return 0;
}
*/


void makeAst(TreeNode* root){

    if(root==NULL){return;}
    ///*
    //printf("%d %d %d ",root->value,root->flag,root->lineNumber);
    //printf("%s %s\n",root->tokenName,root->tokenLexeme);
    //if(root->flag==0) puts(nterms[root->value]);
    TreeNode* root2 = root->parent;
    if(root2)
      //printf("parent : %d %d %s %s %d\n", root2->value,root2->flag,root2->tokenName,root2->tokenLexeme,root2->lineNumber);
    //*/
    if(root->flag==1 && strcmp(terms[root->value],"EPS")==0)return;
    if(root->flag!=1 &&root->child){//printf("%d's child \n ",root->value);
        makeAst(root->child);}
    if(root->value!=0 && root->right){//printf("%d's right \n ",root->value);
        makeAst(root->right);}

    //1
    if(root->flag==0 && strcmp(nterms[root->value],"program")==0){root->ASTChild=root->child;root->ASTRight=NULL;//dms
        root->ASTChild->ASTRight=root->child->right;//oms
        root->ASTChild->ASTRight->ASTRight=root->child->right->right; //dm
        root->ASTChild->ASTRight->ASTRight->ASTRight=root->child->right->right->right;  //oms
        return;}
    //2


    if(root->flag==0 && strcmp(nterms[root->value],"moduleDeclarations")==0){
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;return;}
        else {root->ASTChild=root->child->child->right->right;//id(so need to work for "moduleDeclaration")
              root->ASTChild->ASTRight=root->child->right->ASTChild;}//chain of ids
        root->ASTRight=root->right;//NULL or othermodules
        return;}



    //2
    if(root->flag==1 && strcmp(terms[root->value],"ARRAY")!=0){return;}

    //3
    if(root->flag==0 && strcmp(nterms[root->value],"moduleDeclaration")==0){return;}


    //4
    if(root->flag==0 && strcmp(nterms[root->value],"otherModules")==0){//astright=DM or NULL
    //astchild=id->inputplist->ret->stmts or NULL
        // astchild->....->astright=OM.astchild
        if(root->right==NULL){
         root->ASTRight=NULL;
        }
        else{
            root->ASTRight=root->right;//points to TreeNode of the original Parse Tree(DriverModule)
            }


        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){
            //otherModules->EPS
            root->ASTChild=NULL;

        }
        else{
            root->ASTChild=root->child;//points to module
            root->ASTChild->ASTRight=root->child->right->ASTChild;//link of module module-module-module...
        }
        return;}



    //5
    if(root->flag==0 && strcmp(nterms[root->value],"module")==0){
        root->ASTChild=root->child->right->right;//to id
        root->ASTChild->ASTRight=root->child->right->right->right->right->right->right->right;//points to input_plist--keeping node
        root->ASTChild->ASTRight->ASTRight=root->ASTChild->ASTRight->right->right->right;//points to ret--keeping node
        root->ASTChild->ASTRight->ASTRight->ASTRight=root->ASTChild->ASTRight->ASTRight->right->child->right;//to stmts
        return;
        }



    //6
    if(root->flag==0 && strcmp(nterms[root->value],"input_plist")==0){
    root->ASTChild=root->child;
    root->ASTChild->ASTRight=root->child->right->right->right->ASTChild;//to n1's astchild --no node for n1--directly linking chain of ids
    root->ASTChild->ASTChild=root->child->right->right->child;//ids astchild is INT/REAl/bool/array
        //will handle array separately i.e keeping  a node for ARRAY
    return;
    }

    //7
    if(root->flag==0 && strcmp(nterms[root->value],"dataType")==0){return;}
    if(root->flag==0 && strcmp(nterms[root->value],"type")==0){return;}



    //8
    if(root->flag==1 && strcmp(terms[root->value],"ARRAY")==0){//3 children are NUM/ID(index1),NUM/ID(index2),INT/REAL/BOOL
    root->ASTChild=root->right->right->child->child;
    root->ASTChild->ASTRight=root->right->right->child->right->right->child;
    root->ASTChild->ASTRight->ASTRight=root->right->right->right->right->right->child;
    return;
    }



    //9
    if(root->flag==0 && strcmp(nterms[root->value],"ret")==0){
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;return;}
        else {root->ASTChild=root->child->right->right->ASTChild;}//no node for output_plist}
        return;                                                      //ret directly point to chain of ids or NULL

    }

    //10
    if(root->flag==0 && strcmp(nterms[root->value],"range_arrays")==0){return;}


    //11
    if(root->flag==0 && strcmp(nterms[root->value],"output_plist")==0){
    root->ASTChild=root->child;
    root->ASTChild->ASTRight=root->child->right->right->right->ASTChild;//to n2's astchild --no node for n1--directly linking chain of ids
    root->ASTChild->ASTChild=root->child->right->right->child;//ids astchild is INT/REAl/bool
    return;
    }



    //12
    if(root->flag==0 && strcmp(nterms[root->value],"N1")==0){
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;}
        else {root->ASTChild=root->child->right; //to id
              root->ASTChild->ASTRight=root->child->right->right->right->right->ASTChild; //to N1
              root->ASTChild->ASTChild=root->child->right->right->right->child; //id's astchild is int/real/bool/array

             }
   return;}




    //13
    if(root->flag==0 && strcmp(nterms[root->value],"N2")==0){
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;}
        else {root->ASTChild=root->child->right; //to id
              root->ASTChild->ASTRight=root->child->right->right->right->right->ASTChild; //to N2
              root->ASTChild->ASTChild=root->child->right->right->right->child; //id's astchild is int/real/bool/array

             }
   return;}



    //14
    if(root->flag==0 && strcmp(nterms[root->value],"moduleDef")==0){return;}


    //15
    if(root->flag==0 && strcmp(nterms[root->value],"driverModule")==0){
        root->ASTChild=root->child->right->right->right->right->child->right;//points to moduleDef mein Stmts
        root->ASTRight=root->right;//points to othermodules
        return;
        }

      //16
    if(root->flag==0 && strcmp(nterms[root->value],"statements")==0){
   // root->ASTRight=NULL; TOCHECK
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;}
        else {root->ASTChild=root->child;
              root->ASTChild->ASTRight=root->child->right->ASTChild; //link of statements --a single node of statements which has all the statements chained
        } //ASTchild is either NULL or stmt

        return;
    }


    if(root->flag==0 && strcmp(nterms[root->value],"range")==0){return;}//handled in iterativestmt



  //17
    if(root->flag==0 && strcmp(nterms[root->value],"statement")==0){

    //case1 -iostmt
        if(root->child->flag==0 && strcmp(nterms[root->child->value],"ioStmt")==0){//stmt.astchild=getval/print
        //astchild.astright=id/var
            root->ASTChild=root->child->child;//points to GET_VAL/PRINT
            root->ASTChild->ASTRight=root->child->child->right->right;//points to ID/VAR in ioStmt(refer Rule 33, 34)
           }

    //case2 -iterativeStmt
        else if(root->child->flag==0 && strcmp(nterms[root->child->value],"iterativeStmt")==0){
             if(root->child->child->flag==1 && strcmp(terms[root->child->child->value],"FOR")==0){
             //for rule 103
                 root->ASTChild=root->child->child;//points to FOR
                 root->ASTChild->ASTRight=root->ASTChild->right->right;//points to ID
                 TreeNode* temp=root->child->child->right->right->right->right;//handling of "range"
                 root->ASTChild->ASTRight->ASTRight=temp;//point to range
                 root->ASTChild->ASTRight->ASTRight->ASTChild=temp->child;//range.ASTChild=NUM1
                 root->ASTChild->ASTRight->ASTRight->ASTChild->ASTRight=temp->child->right->right;//range.ASTChild.ASTRight=NUM2
                 root->ASTChild->ASTRight->ASTRight->ASTRight=temp->right->right->right;//ASTC.ASTR.ASTR.ASTR=stmts
             }
             else{
                 //for rule 104

                 root->ASTChild=root->child->child;//points to WHILE
                 root->ASTChild->ASTRight=root->ASTChild->right->right;//points to arithmeticOrBooleanExpr's TreeNode
                 root->ASTChild->ASTRight->ASTRight=root->ASTChild->ASTRight->right->right->right;//points to statement
             }
         }

      //case3--declareStmt
        else if(root->child->flag==0 && strcmp(nterms[root->child->value],"declareStmt")==0){
               root->ASTChild=root->child->child;//points to DECLARE;
               root->ASTChild->ASTRight=root->ASTChild->right;//points to idList's node in the original ParseTree
               root->ASTChild->ASTRight->ASTRight=root->ASTChild->ASTRight->right->right->child;//points to the dataType's children;
           }
      //case4--simpleStmt
        else if(root->child->flag==0 && strcmp(nterms[root->child->value],"simpleStmt")==0){
               root->ASTChild=root->child->child;}//points to labelled node of "assignmentStmt/moduleReuseStmt"



        else if(root->child->flag==0 && strcmp(nterms[root->child->value],"conditionalStmt")==0){
    root->ASTChild=root->child->child;//points to SWITCH treenode
    root->ASTChild->ASTRight=root->child->child->right->right;//points to ID
    root->ASTChild->ASTRight->ASTRight=root->child->child->right->right->right->right->right;//points to caseStmts's node
    root->ASTChild->ASTRight->ASTRight->ASTRight=root->ASTChild->ASTRight->ASTRight->right;}//points to default1's node

          return;}




    //18

   if(root->flag==0 && strcmp(nterms[root->value],"var")==0){

       root->ASTChild=root->child->ASTChild;

        return;  }



    //19
   if(root->flag==0 && strcmp(nterms[root->value],"var_id_num")==0){
   root->ASTChild=root->child;//num or rnum or id
       if(root->child->flag==1 && strcmp(terms[root->child->value],"ID")==0){root->ASTChild->ASTChild=root->child->right;}// whichid}
      // else {root->ASTChild->ASTRight=NULL;}
      return;
   }


     //20
    if(root->flag==0 && strcmp(nterms[root->value],"whichId")==0){
       if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;return;}
       else {root->ASTChild=root->child->right->child;}//num or id}
        return;}



// 21
    if(root->flag==0 && strcmp(nterms[root->value],"index1")==0) {return;}

 //22
    if(root->flag==0 && strcmp(nterms[root->value],"iterativeStmt")==0) {return;}

//23
    if(root->flag==0 && strcmp(nterms[root->value],"ioStmt")==0) {return;}

 //24
    if(root->flag==0 && (strcmp(nterms[root->value],"declareStmt")==0 || strcmp(nterms[root->value],"conditionalStmt")==0 || strcmp(nterms[root->value],"simpleStmt")==0 )){return;}



 //25
    if(root->flag==0 && strcmp(nterms[root->value],"idList")==0){
    root->ASTChild=root->child;//id
    root->ASTChild->ASTRight=root->child->right->ASTChild;
    return;
    }


  //26
    if(root->flag==0 && strcmp(nterms[root->value],"N3")==0){
    if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;}
    else {root->ASTChild=root->child->right;root->ASTChild->ASTRight=root->child->right->right->ASTChild;}
       return;
    }


  //26
    if(root->flag==0 && strcmp(nterms[root->value],"relationalOp")==0){root->ASTChild=root->child;return;}
    if(root->flag==0 && strcmp(nterms[root->value],"value")==0){root->ASTChild=root->child;return;}
    if(root->flag==0 && strcmp(nterms[root->value],"logicalOp")==0){root->ASTChild=root->child;return;}
    if(root->flag==0 && strcmp(nterms[root->value],"op1")==0){root->ASTChild=root->child;return;}
    if(root->flag==0 && strcmp(nterms[root->value],"op2")==0){root->ASTChild=root->child;return;}
    if(root->flag==0 && strcmp(nterms[root->value],"unary_op")==0){root->ASTChild=root->child;return;}
    if(root->flag==0 && strcmp(nterms[root->value],"expression")==0){root->ASTChild=root->child;return;}


  //27
    if(root->flag==0 && strcmp(nterms[root->value],"moduleReuseStmt")==0){
       root->ASTChild=root->child;//points to optional TreeNode
        /*if(root->child->child->flag==1 && strcmp(terms[root->child->child->value],"EPS")==0){
            root->ASTChild->ASTChild=NULL;
        }
        else{
            root->ASTChild->ASTChild=root->child->child->right;//points to idList's TreeNode in originalParse Tree
        }*/
        root->ASTChild->ASTRight=root->ASTChild->right->right->right;//points to the ID node in originalParse Tree
        root->ASTChild->ASTRight->ASTRight=root->ASTChild->ASTRight->right->right->right;
        return;//points to the idList's node in originalParse Tree
    }


  //28
    if(root->flag==0 && strcmp(nterms[root->value],"optional")==0){
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){
            root->ASTChild=NULL;//optional--->EPS
        }
        else{
            root->ASTChild=root->child->right;//points to idList's TreeNode in originalParse Tree
        }
        return;
    }



    //33
    if(root->flag==0 && strcmp(nterms[root->value],"assignmentStmt")==0){
    root->ASTChild=root->child;
    root->ASTChild->ASTRight=root->child->right->child;//node for lvaluearr and lvalueid
    return;
    }

    //34
    if(root->flag==0 && strcmp(nterms[root->value],"lvalueIDStmt")==0){
    root->ASTChild=root->child->right->ASTChild;return;//to astchild of exp
    }

    //35
    if(root->flag==0 && strcmp(nterms[root->value],"lvalueARRStmt")==0){
    root->ASTChild=root->child->right->child;//to num or rnum
    root->ASTChild->ASTRight=root->child->right->right->right->right->ASTChild;//to astchild of exp
    return;
    }

    //36
    if(root->flag==0 && strcmp(nterms[root->value],"default1")==0){
    //no need to keep DEFAULT since a node for default1 has been kept
       if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;}
        else {root->ASTChild=root->child->right->right;}//keeping a node for stmts
        return;

    }

    //37
    if(root->flag==0 && strcmp(nterms[root->value],"caseStmts")==0){
    root->ASTChild=root->child;//CASE
    root->ASTChild->ASTRight=root->child->right->ASTChild;//value's astchild
     root->ASTChild->ASTRight->ASTRight=root->child->right->right->right;//node for stmts
     root->ASTChild->ASTRight->ASTRight->ASTRight=root->child->right->right->right->right->right->right->ASTChild;//astchild of N9;
        return;
    }

    //38
    if(root->flag==0 && strcmp(nterms[root->value],"N9")==0){
    if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;}
        else {root->ASTChild=root->child;//CASE
              root->ASTChild->ASTRight=root->child->right->ASTChild;//value's astchild
               root->ASTChild->ASTRight->ASTRight=root->child->right->right->right;//node for stmts
              root->ASTChild->ASTRight->ASTRight->ASTRight=root->child->right->right->right->right->right->right->ASTChild;//astchild of N9;

             }
    return;

    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //39
    if(root->flag==0 && strcmp(nterms[root->value],"U")==0){ //done
        root->ASTChild=root->child->ASTChild;//U.ASTC points to unary_op.ASTC
        root->ASTChild->ASTRight=root->child->right->ASTChild;//U.AC.AR points to new_NT.AC
        return;

    }




    //40
    if(root->flag==0 && strcmp(nterms[root->value],"new_NT")==0){ //done
        if(root->child->flag==1 && strcmp(terms[root->child->value],"BO")==0){
            root->ASTChild=root->child->right->ASTChild;//points to arithmeticExpr's astchild
        }
        else
            {root->ASTChild=root->child->ASTChild;}// points to var_id_num.ASTChild
          return;
    }




    //41
    /*arithmeticExpr:term,N4,
	N4:op1,term,N4,
	N4:EPS,*/

    if(root->flag==0 && strcmp(nterms[root->value],"arithmeticExpr")==0){ //done


        if(root->child->right->child->flag==1 && strcmp(terms[root->child->right->child->value],"EPS")==0){root->ASTChild=root->child->ASTChild;return;}
        else {
        	/*
    		root->ASTChild=root->child->right->child->ASTChild;//op1's astchild
    		root->ASTChild->ASTChild=root->child->ASTChild;//terms astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->ASTChild;*/
    		TreeNode* op=root->child->right->child->ASTChild;
    		op->ASTChild=root->child->ASTChild;
    		TreeNode* semi=root->child->right->child->right->right->ASTChild;

    		if(semi==NULL){root->ASTChild=op;op->ASTRight=NULL;op->ASTChild->ASTRight=root->child->right->child->right->ASTChild;op->ASTChild->ASTRight->ASTRight=NULL;return;}
    		if(semi->flag==1 && (strcmp(terms[semi->value],"PLUS")==0 ||strcmp(terms[semi->value],"MINUS")==0 )){
    		root->ASTChild=semi;semi->ASTRight=NULL;
    		op->ASTChild->ASTRight=semi->ASTChild;
    		op->ASTRight=semi->ASTChild->ASTRight;op->ASTChild->ASTRight->ASTRight=NULL;op->ASTRight->ASTRight=NULL;
    		semi->ASTChild=op;}
    		else {

    			root->ASTChild=root->child->right->child->right->right->child->ASTChild;
    			root->ASTChild->ASTRight=NULL;
    			root->ASTChild->ASTChild=op;
    			op->ASTChild->ASTRight=root->child->right->child->right->ASTChild;
    			op->ASTRight=semi;semi->ASTRight=NULL;
    			op->ASTChild->ASTRight->ASTRight=NULL;op->ASTRight->ASTRight=NULL;


    		}


    	}

        return;
    }



    //42
    	/*N4:op1,term,N4,
			N4:EPS,*/
    if(root->flag==0 && strcmp(nterms[root->value],"N4")==0){  //done
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){
            root->ASTChild=NULL;return;//for the rule N4->EPS
        }

        if(root->child->right->right->child->flag==1 && strcmp(terms[root->child->right->right->child->value],"EPS")==0){root->ASTChild=root->child->right->ASTChild;return;}
    else {
    		/*root->ASTChild=root->child->right->right->child->ASTChild;//op1's astchild
    		root->ASTChild->ASTChild=root->child->right->ASTChild;//terms astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->right->ASTChild;*/
    	TreeNode* op=root->child->right->right->child->ASTChild;
    	op->ASTChild=root->child->right->ASTChild;
    	TreeNode* semi=root->child->right->right->child->right->right->ASTChild;
    	if(semi==NULL){root->ASTChild=op;op->ASTRight=NULL;op->ASTChild->ASTRight=root->child->right->right->child->right->ASTChild;op->ASTChild->ASTRight->ASTRight=NULL;return;}

    	if(semi->flag==1 && (strcmp(terms[semi->value],"PLUS")==0 ||strcmp(terms[semi->value],"MINUS")==0 )){

    		root->ASTChild=semi;root->ASTChild->ASTRight=NULL;
    		op->ASTChild->ASTRight=semi->ASTChild;
    		op->ASTRight=semi->ASTChild->ASTRight;op->ASTChild->ASTRight->ASTRight=NULL;op->ASTRight->ASTRight=NULL;
    		
    		semi->ASTChild=op;



    	}
    	else {
    		root->ASTChild=root->child->right->right->child->right->right->child->ASTChild;root->ASTChild->ASTRight=NULL;
    		root->ASTChild->ASTChild=op;
    		op->ASTChild->ASTRight=root->child->right->right->child->right->ASTChild;
    		op->ASTRight=semi;semi->ASTRight=NULL;
    		op->ASTChild->ASTRight->ASTRight=NULL;op->ASTRight->ASTRight=NULL;


    	}



    	}

        return;
    }




    //43

    /*term:factor,N5,
		N5:op2,factor,N5,
		N5:EPS,*/
    if(root->flag==0 && strcmp(nterms[root->value],"term")==0){ //done
        if(root->child->right->child->flag==1 && strcmp(terms[root->child->right->child->value],"EPS")==0){root->ASTChild=root->child->ASTChild;return;}
        else {
    		root->ASTChild=root->child->right->child->ASTChild;//op2's astchild
    		root->ASTChild->ASTChild=root->child->ASTChild;//factor astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->ASTChild;

    	}
        return;
    }




    //44
    if(root->flag==0 && strcmp(nterms[root->value],"N5")==0){ //done
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){
            root->ASTChild=NULL;return;//for the rule N4->EPS
        }

        if(root->child->right->right->child->flag==1 && strcmp(terms[root->child->right->right->child->value],"EPS")==0){root->ASTChild=root->child->right->ASTChild;return;}
    else {
    		root->ASTChild=root->child->right->right->child->ASTChild;//op2's astchild
    		root->ASTChild->ASTChild=root->child->right->ASTChild;//factor astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->right->ASTChild;

    	}

        return;
    }




    //45
    if(root->flag==0 && strcmp(nterms[root->value],"factor")==0){ //done
        if(root->child->flag==1 && strcmp(terms[root->child->value],"BO")==0){
            root->ASTChild=root->child->right;//points to arithmeticOrBooleanExpr's node in original ParseTree
        }
        else{
            root->ASTChild=root->child->ASTChild;//points to var_id_num.ASTChild
        }

        return;
    }





    //46
    if(root->flag==0 && strcmp(nterms[root->value],"arithmeticOrBooleanExpr")==0){ //done
        /*arithmeticOrBooleanExpr:AnyTerm,N7,
		N7:logicalOp,AnyTerm,N7,
		N7:EPS,*/

    	if(root->child->right->child->flag==1 && strcmp(terms[root->child->right->child->value],"EPS")==0){root->ASTChild=root->child->ASTChild;return;}
    	else {
    		root->ASTChild=root->child->right->child->ASTChild;//logicalop's astchild
    		root->ASTChild->ASTChild=root->child->ASTChild;//anyterms astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->ASTChild;

    	}

        return;
    }



    //47
    if(root->flag==0 && strcmp(nterms[root->value],"N7")==0){  //done
     if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){root->ASTChild=NULL;return;}
    if(root->child->right->right->child->flag==1 && strcmp(terms[root->child->right->right->child->value],"EPS")==0){root->ASTChild=root->child->right->ASTChild;return;}
    else {
    		root->ASTChild=root->child->right->right->child->ASTChild;//logicalop's astchild
    		root->ASTChild->ASTChild=root->child->right->ASTChild;//anyterms astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->right->ASTChild;

    	}

        return;
    }




    //48
    /*AnyTerm:arithmeticExpr,N8,
	AnyTerm:boolConstt,
	N8:relationalOp,arithmeticExpr,N8,
	N8:EPS,*/
    if(root->flag==0 && strcmp(nterms[root->value],"AnyTerm")==0){ //done
        if(root->child->flag==0 && strcmp(nterms[root->child->value],"boolConstt")==0){
            root->ASTChild=root->child->ASTChild;//points to boolConstt.ASTChild
        }
        else{

       if(root->child->right->child->flag==1 && strcmp(terms[root->child->right->child->value],"EPS")==0){root->ASTChild=root->child->ASTChild;return;}
       else {
       		root->ASTChild=root->child->right->child->ASTChild;//relationalop's astchild
    		root->ASTChild->ASTChild=root->child->ASTChild;//ae astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->ASTChild;
    		}
    	}


        return;
    }

    //48'
    if(root->flag==0 && strcmp(nterms[root->value],"boolConstt")==0){root->ASTChild=root->child;return;}//done



    //49
    /*N8:relationalOp,arithmeticExpr,N8,
	N8:EPS,*/
    if(root->flag==0 && strcmp(nterms[root->value],"N8")==0){ //done
        if(root->child->flag==1 && strcmp(terms[root->child->value],"EPS")==0){
            root->ASTChild=NULL;return;
            //for the rule N8->EPS
        }


        if(root->child->right->right->child->flag==1 && strcmp(terms[root->child->right->right->child->value],"EPS")==0){root->ASTChild=root->child->right->ASTChild;return;}
    else {
    		root->ASTChild=root->child->right->right->child->ASTChild;//relationalop's astchild
    		root->ASTChild->ASTChild=root->child->right->ASTChild;//ae astchild
    		root->ASTChild->ASTChild->ASTRight=root->child->right->right->ASTChild;

    	}
        return;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return;
  }

void assignAstParent(TreeNode* root){
if(!root){return;}
numASTnodes++;
TreeNode* c=root->ASTChild;
TreeNode* r;
if(c){c->ASTParent=root;
    r=c->ASTRight;
    while(r){r->ASTParent=root;r=r->ASTRight;}
  }

assignAstParent(root->ASTChild);
assignAstParent(root->ASTRight);
return;
}


void printAST(TreeNode*  root){

    if(root==NULL){

        return;
    }

    char parent[100]="  ";
    char child[100]="   ";
    //char parent[100],child[100];
    TreeNode * Aparent =root->ASTParent;
    TreeNode * Achild =root->ASTChild;
    //
    if(Aparent==NULL)
        strcpy(parent,"-----");
    else{
        if(Aparent->flag)
            strcpy(parent,terms[Aparent->value]);
        else
        strcpy(parent,nterms[Aparent->value]);
    }

    if(Achild==NULL)
        strcpy(child,"-----");
    else{
        if(Achild->flag)
            strcpy(child,terms[Achild->value]);
        else
            strcpy(child,nterms[Achild->value]);
    }

    if(root->flag==0){

        printf(" %-30s %-30s %-30s\n", nterms[root->value],parent, child);

    }
    else {
         printf(" %-30s %-30s %-30s\n", terms[root->value],parent,child);

    }

    printAST(root->ASTChild);
    printAST(root->ASTRight);
    return;
}

void displayAST(TreeNode * root){
    printf("TRAVERSAL ORDER: INORDER TRAVERSAL I.E GIVEN A ROOT NODE THE LEFT MOST CHILD IS TRAVERESED FOLLOWED BY ALL THE SIBLINGS\n\n");
    printf("===========================================================================================\n");
    printf(BOLDWHITE "%50s\n" RESET,"ABSTRACT SYNTAX TREE");
    printf("===========================================================================================\n");
    printf(BOLDGREEN " %-30s %-30s %-30s\n\n" RESET, "Current Node","Parent Node", "Leftmost Child Node");
    printf("===========================================================================================\n");

    printAST(root);
    printf("===========================================================================================\n");


}