//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//ENUM FOR TERMINALS AND NON-TERMINALS
typedef enum {program,moduleDeclarations,otherModules,driverModule,moduleDeclaration,module,moduleDef,input_plist,ret,dataType,N1,output_plist,type,N2,range_arrays,index1,statements,statement,ioStmt,simpleStmt,declareStmt,conditionalStmt,iterativeStmt,var,var_id_num,whichId,boolConstt,assignmentStmt,moduleReuseStmt,whichStmt,lvalueIDStmt,lvalueARRStmt,expression,optional,idList,N3,arithmeticOrBooleanExpr,U,new_NT,arithmeticExpr,unary_op,AnyTerm,N7,logicalOp,N8,relationalOp,op1,op2,term,N4,factor,N5,caseStmts,default1,N9,value,range}NonTerminal;

typedef enum {EPS,DECLARE,MODULE,ID,SEMICOL,DRIVERDEF,DRIVER,PROGRAM,DRIVERENDDEF,DEF,ENDDEF,TAKES,INPUT,SQBO,SQBC,RETURNS,COLON,COMMA,INTEGER,REAL,BOOLEAN,ARRAY,OF,RANGEOP,START,END,GET_VALUE,BO,BC,PRINT,NUM,RNUM,TRUE,FALSE,ASSIGNOP,USE,WITH,PARAMETERS,PLUS,MINUS,MUL,DIV,AND,OR,LT,LE,GT,GE,NE,EQ,SWITCH,DEFAULT,BREAK,FOR,WHILE,CASE,IN,DOLLAR}Terminal;

// CHAR ARRAY REPRESENTING TERMINAL AND NON-TERMINALS NAME'S
char *nterms[]={"program","moduleDeclarations","otherModules","driverModule","moduleDeclaration","module","moduleDef","input_plist","ret","dataType","N1","output_plist","type","N2","range_arrays","index1","statements","statement","ioStmt","simpleStmt","declareStmt","conditionalStmt","iterativeStmt","var","var_id_num","whichId","boolConstt","assignmentStmt","moduleReuseStmt","whichStmt","lvalueIDStmt","lvalueARRStmt","expression","optional","idList","N3","arithmeticOrBooleanExpr","U","new_NT","arithmeticExpr","unary_op","AnyTerm","N7","logicalOp","N8","relationalOp","op1","op2","term","N4","factor","N5","caseStmts","default1","N9","value","range"};

char *terms[]={"EPS","DECLARE","MODULE","ID","SEMICOL","DRIVERDEF","DRIVER","PROGRAM","DRIVERENDDEF","DEF","ENDDEF","TAKES","INPUT","SQBO","SQBC","RETURNS","COLON","COMMA","INTEGER","REAL","BOOLEAN","ARRAY","OF","RANGEOP","START","END","GET_VALUE","BO","BC","PRINT","NUM","RNUM","TRUE","FALSE","ASSIGNOP","USE","WITH","PARAMETERS","PLUS","MINUS","MUL","DIV","AND","OR","LT","LE","GT","GE","NE","EQ","SWITCH","DEFAULT","BREAK","FOR","WHILE","CASE", "IN", "DOLLAR"};


typedef union {
    Terminal T;
    NonTerminal NT;
}SymType;

//NODE STRUCTURE FOR THE LINKED LIST REPRESENTING RHS OF THE GRAMMAR
struct rhsNode{
    SymType S;
    int flag;
    struct rhsNode* nextRhs;
};
typedef struct rhsNode RhsNode;

//ARRAY CELL REPRESENTING EACH RULE IN THE ARRAY OF RULES(GRAMMAR)
struct grammarCell{
    NonTerminal lhs;
    RhsNode * headRhs;
};
typedef struct grammarCell Grammar[120];

// RECORD STRUCTURE FOR EACH ENTRY OF MAPPING TABLE: MAPS STRINGS TO TERMINALS/NON-TERMINALS
typedef struct{
    SymType sym;
    char token[100];
    int TorNT;
}mapRecord;

//typedef struct treenode TreeNode;
// NODE OF PARSE TREE
typedef struct treeNode{
    int value;
    int flag;
    char tokenName[30];
	char tokenLexeme[30];
	int lineNumber;
    int isArray;
    int called;
    struct treeNode* right;
    struct treeNode* child;
    struct treeNode* ASTChild;//to be initialized with NULL
    struct treeNode* ASTRight;//to be initialized with NULL
    struct treeNode* ASTParent;
	struct treeNode* parent;
}TreeNode;

typedef struct {
    int num;
    TreeNode* root;
}Tree;

// NODE OF STACK REQUIRED DURING PARSING
typedef struct stackNode{
    int val;
    int flag;
    TreeNode* address; // stores address of the treenode corresponding to that particular symbol
    struct stackNode* next;
}StackNode;

typedef struct stack{
    int num;
    StackNode* head;
}Stack;
