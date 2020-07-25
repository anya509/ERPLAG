//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
#include "parser.h"
#include "lexer.c"


#define mapSize 199 //199
#define z EPS
#define z1 DOLLAR
#define numRules 105//101;//changed
#define numNonTerminals 57//54;//changed
#define numTerminals 58//60;//ap


//----FORMATTING UTILS------
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"
//------------------------


mapRecord MappingTable[mapSize];

int flags[numNonTerminals];
int flag[numNonTerminals];
int first[numNonTerminals][numTerminals];
int firstRHS[numRules][numTerminals];
int follow[numNonTerminals][numTerminals];
int hash[numNonTerminals][numRules];

Grammar G;
int ParseTable[numNonTerminals][numTerminals];

Stack* st;
Tree* pt;

//MODULES TO HASH TERMINALS AND NON-TERMINAL NAMES TO ENUM VALUES

void insertIntoMap(char* token, int TorNT, SymType S){

    int index=hashFunction(token,mapSize);
    //printf("Index: %d\n",index);
    int i=index;
    //printf("%d \n",index);
    while(strcmp(MappingTable[i].token,"")!=0){
        i=(i+1)%mapSize;
    }
    //printf("Final Index: %d Token %s\n",i,token);
    strcpy(MappingTable[i].token,token);
    MappingTable[i].TorNT=TorNT;
    MappingTable[i].sym=S;
}

mapRecord getRecordFromMap(char *token){

    int index,i;
    index=hashFunction(token,mapSize);
    i=index;

    while(strcmp(MappingTable[i].token,token)!=0){
        i++;
        i=i%mapSize;
        if(i==index){
            printf("%d",index);
            puts(token);
            printf("Record not in mapping table. First insert the token %s\n",token);
            exit(0);
        }
    }
    return MappingTable[i];
}

void buildMappingTable(){

    for(int i=0;i<numTerminals;i++){
        SymType S;
        S.T=i;
        insertIntoMap(terms[i], 1, S);
    }

    for(int i=0;i<numNonTerminals;i++){
        SymType S;
        S.NT=i;
        insertIntoMap(nterms[i], 0 ,S);
    }
}

void displayMappingTable(){

	    for(int i=0;i<mapSize;i++){
	        printf("Index: %-5d ",i);
	        if(MappingTable[i].TorNT){
	            printf("Name: %-30s Sym: %-5d ", MappingTable[i].token, MappingTable[i].sym.T);
	            printf("%-20s","Terminal");
	        }
	        else{
	        	if(strcmp(MappingTable[i].token,"")!=0){
	            	printf("Name: %-30s Sym: %-5d ",MappingTable[i].token,MappingTable[i].sym.NT);
	            	printf("%-20s","Non Terminal");
	            }
	            else{
	            	printf("Name: %-30s Sym: %-5s ","-------","---");
	            	printf("%-20s","------");
	            }
	        }
	        printf("\n");
	    }
	}


// MODULE TO SET UP AN ARRAR OF LINKED LISTS REPRESENTING THE GRAMMAR

void buildGrammar(FILE *grammarFile){

    if(grammarFile==NULL)
        return;

    char lineBuff[1000];
    int headFlag=0;
    int ruleno=0;

    while(!feof(grammarFile)){
        // reads the grammar file line by line
        fscanf(grammarFile, "%[^\n]\n",lineBuff);
        lineBuff[strlen(lineBuff)-1]='\0';
        char* token=strtok(lineBuff, ":"); // delimiters are : and , to separate rhs and lhs
        RhsNode * curr=NULL;
        while(token!=NULL){
        	//printf(" %d ", ruleno+1);
            mapRecord M=getRecordFromMap(token);
            if(headFlag==0){
                G[ruleno].lhs=M.sym.NT;
                headFlag=1;
            }
            else{
                RhsNode * node=(RhsNode*)malloc(sizeof(RhsNode));
                node->S=M.sym;
                node->flag=M.TorNT;
                if(headFlag==1){
                    curr=node;
                    G[ruleno].headRhs=curr;
                    headFlag=2;
                }
                else{
                    curr->nextRhs=node;
                    curr=curr->nextRhs;
                }
            }

            token=strtok(NULL, ",");
        }
        headFlag=0;
        curr->nextRhs=NULL;
        ruleno++;
    }
}

void displayGrammar(){
    for(int i=0;i<numRules;i++){
        RhsNode * curr;
        printf("Head: Flag: %d ENUM: %d ",0,G[i].lhs);
        curr=G[i].headRhs;
        while(curr!=NULL){
            printf("-> FLAG: %d ENUM: %d ",curr->flag,(curr->flag ? curr->S.T : curr->S.NT));
            curr=curr->nextRhs;
        }
        printf("\n");
    }
}

// MODULES TO CALCULATE THE FIRST AND FOLLOW SETS

//--------------FIRST SETS---------------------------------------------------------------------------------------
//Initialize the first, follow nd hash arrays
void populate(){
    int y=0;
    int r;
    for(y=0;y<numNonTerminals;y++){
        for(r=0;r<numTerminals;r++){
            first[y][r]=0;
            follow[y][r]=0;
        }
    }
    for(y=0;y<numRules;y++){
        for(r=0;r<numTerminals;r++){
            firstRHS[y][r]=0;
        }
    }
    for(y=0;y<numNonTerminals;y++){
        for(r=0;r<numRules;r++){
            hash[y][r]=0;
        }
    }
}

// Calculate the first set of each Non terminal
void fillFirst(int nt){ // input is the enum value of the non-terminal

    //printf("%d\n", nt);
    int temp[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int index=0;
    int i=0;

    for(;i<numRules;i++){
        if(G[i].lhs==nt)
            temp[index++]=i;
    }
    flags[nt]=1;
    //printf("%d\n", nt);
    for(i=0;i<index;i++){
        //printf("index %d\n", temp[i]);
        RhsNode* start=G[temp[i]].headRhs;
        //printf("%d\n",start->S.T );
        //printf("oops"); //first[nt][start->S.T]=1
        while(start!=NULL){

            if(start->flag==1) {
                //printf("T check");
                first[nt][start->S.T]=1;
                break;
            }
            else{
                if(flags[start->S.NT]!=1){
                    fillFirst(start->S.NT);
                }

                int q;
                for(q=0;q<numTerminals;q++){
                    if(q==z)
                        continue;
                    first[nt][q]|=first[start->S.NT][q];
                }
                if(first[start->S.NT][z]!=1)
                    break;
                if(first[start->S.NT][z]==1 && start->nextRhs==NULL)
                    first[nt][z]=1;

                start=start->nextRhs;
            }
        }
    }

}

void findFirst(){
    int i=0;
    for(;i<numNonTerminals;i++){
        if(flags[i]!=1){
            fillFirst(i);
            //printf("%d done\n", i);
        }
    }
}

// Calculate the first of the RHS of each rule
void fillRHS(int rule){

    RhsNode* curr=G[rule].headRhs;
    int i;
    while(curr!=NULL){
        if(curr->flag==1){
            firstRHS[rule][curr->S.T]=1;
            break;
        }
        for(i=0;i<numTerminals;i++){
            if(i==z){
                continue;
            }
            firstRHS[rule][i]|=first[curr->S.NT][i];
        }

        if(first[curr->S.NT][z]!=1){
            break;
        }
        if(first[curr->S.NT][z]==1 && curr->nextRhs==NULL){
            firstRHS[rule][z]=1;
        }
        curr=curr->nextRhs;
    }
}

 void fillfirstRHS(){
    int i=0;
    for(i=0;i<numRules;i++){
        fillRHS(i);
    }
 }

//--------------------FOLLOW SETS------------------------------------------------------------------------------

// checks is a non terminal is contained in the rhs of any rule
void populate_hash(){

    int i=0;
    for(i=0;i<numRules;i++){
        //printf("%d ", i);
        RhsNode* curr=G[i].headRhs;
        while(curr!=NULL){
            //printf("%d ", curr->flag);
            if(curr->flag==0)
                hash[curr->S.NT][i]=1;
            curr=curr->nextRhs;
        }
    }
}


void fillfollow(int nt){
    //printf("nt: %s", nterms[nt]);
    int i;
    for(i=0;i<numRules;i++){

        if(hash[nt][i]==1){
            int null_occ=0;

            RhsNode* start=G[i].headRhs;
            while(start!=NULL){
                while(start->flag==1 || start->S.NT!=nt){
                    start=start->nextRhs;
                    if(start==NULL){
                        null_occ=1;
                        break;
                    }
                }

                if(null_occ==1){
                    break;
                }

                RhsNode* nex=start->nextRhs;

                if(nex!=NULL && nex->flag){
                    follow[nt][nex->S.T]=1;
                    start=start->nextRhs;
                    continue;
                }
                else if(nex==NULL && G[i].lhs!=nt){
                    if(flag[G[i].lhs]!=1){
                        fillfollow(G[i].lhs);
                    }
                    int y=0;
                    for(y=0;y<numTerminals;y++){
                        follow[nt][y]|=follow[G[i].lhs][y];
                    }
                    break;//goto next rule
                }
                else {//NT succeeds
                    //printf("%d %d", i, G[i].lhs); if(nex==NULL) puts("nll");//exit(0);
                    int q=0;
                    while(nex!=NULL){

                        if(nex->flag){
                            follow[nt][nex->S.T]=1;
                            break;
                        }
                        for(q=0;q<numTerminals;q++){
                            if(q==z){
                                continue;
                            }
                            follow[nt][q]|=first[nex->S.NT][q];
                        }
                        if(nex->nextRhs==NULL && first[nex->S.NT][z]==1 && G[i].lhs!=nt){
                            if(flag[G[i].lhs]!=1){
                                fillfollow(G[i].lhs);
                            }
                            int y=0;
                            for(y=0;y<numTerminals;y++){
                                follow[nt][y]|=follow[G[i].lhs][y];
                            }
                        }

                        if(first[nex->S.NT][z]==1){
                            nex=nex->nextRhs;
                            continue;
                        }
                        else
                            break;
                    }
                }
                start=start->nextRhs;
            }
        }
    }

    flag[nt]=1;
    //printf("nt%d\n", nt);
}


void findfollow(){
    follow[0][z1]=1; // 0 is the start symbol and z1 is dollar
    int i=0;
    for(i=0;i<numNonTerminals;i++){
        if(flag[i]!=1)
            fillfollow(i);
    }
}

//--------------DISPLAY FIRST AND FOLLOW SETS----------------------------------------
void displayFirst(){
    //populate();
    //findFirst();
    for(int i=0;i<numNonTerminals;i++){
        printf("FIRST %s:  ",nterms[i]);
        for(int j=0;j<numTerminals;j++){
            if(first[i][j])
            	printf("%s ", terms[j]);
        }
        printf("\n");
    }
}

void displayFirstRhs(){
    //populate();
    //findFirst();
    //fillfirstRHS();
    for(int i=0;i<numRules;i++){
        printf("FIRST (IN CORRS. RULE %d) for %s:  ",i+1, nterms[i]);
        for(int j=0;j<numTerminals;j++){
            if(firstRHS[i][j])
                printf("%s ", terms[j]);
        }
        printf("\n");
    }
}

void displayFollow(){
    // populate();
    // findFirst();
    // fillfirstRHS();
    // populate_hash();
    // findfollow();

    // UNCOMMENT TO DISPLAY THE HASH TABLE
    // for(int i=0;i<numNonTerminals;i++){
    //     printf("HASH %s:",nterms[i]);
    //     for(int j=0;j<numRules;j++){
    //         //printf(" %d ",hash[i][j]);
    //         if(hash[i][j])
    //         	printf("%d ", j);
    //     }
    //     printf("\n");
    // }

    //DISPLAYS THE FOLLOW SETS FOR EACH NT
    for(int i=0;i<numNonTerminals;i++){
        printf("FOLLOW %s:",nterms[i]);
        for(int j=0;j<numTerminals;j++){
            //printf(" %d ",follow[i][j]);
            if(follow[i][j]==1)
            	printf("%s ", terms[j]);
        }
        printf("\n");
    }
}


//------------------MODULE FOR CONSTRUCTING THE PARSE TABLE--------------------------------------------------
void makeParseTable(){
    for(int ntIdx=0; ntIdx<numNonTerminals; ntIdx++)
        for(int termIdx=0; termIdx<numTerminals; termIdx++)
            ParseTable[ntIdx][termIdx]=-1;

    for(int ruleIdx=0; ruleIdx<numRules; ruleIdx++){
        int ntIdx = G[ruleIdx].lhs;

        for(int termIdx=1; termIdx<numTerminals-1; termIdx++){
            if(firstRHS[ruleIdx][termIdx])
                ParseTable[ntIdx][termIdx] = ruleIdx;
        }

        if(firstRHS[ruleIdx][0]){//epsilon in first
            //each T in follow set, and $
            for(int termIdx=1; termIdx<numTerminals; termIdx++)
                if(follow[ntIdx][termIdx])
		//&& ParseTable[ntIdx][termIdx]==-1
                    ParseTable[ntIdx][termIdx] = ruleIdx;
        }
    }
}

void displayParseTable(){
    printf("\nNOTE");
    printf("\n\nBase Index of Rules 1, printing parse table valid entries only with format :");
    printf("\nNon Terminal : (Terminal, Index of Rule)\n\n");
    for(int ntIdx=0; ntIdx<numNonTerminals; ntIdx++){
        printf("%s : ", nterms[ntIdx]);
        for(int termIdx=0; termIdx<numTerminals; termIdx++)
            if(ParseTable[ntIdx][termIdx]!=-1)
                printf("(%s, %d) ", terms[termIdx], ParseTable[ntIdx][termIdx]+1);
            //printf("%d, ", ParseTable[ntIdx][termIdx]);
        printf("\n");
    }
}

//-----------------MODULES FOR PARSING ALGORITHM AND MAKING PARSE TREE-----------------------------------------

void pop(Stack* st){
    if(st->head==NULL)
        return;
    st->head=(st->head)->next;
}

void push(Stack*st, StackNode* node ){
    if(st->head==NULL){
        st->head=node;
        return;
    }
    node->next=st->head;
    st->head=node;
}

StackNode* createNode(RhsNode* rnode,TreeNode * tnode){
    StackNode* temp = (StackNode*)malloc(sizeof(StackNode));
    temp->flag=rnode->flag;
    temp->address=tnode;
    if(rnode->flag==1)
        temp->val=rnode->S.T;
    else
        temp->val=rnode->S.NT;
    return temp;
}

//Initialize the stack with dollar and start symbol and tree with the start node
void initializeStackTree(){
    st = (Stack*) malloc(sizeof(Stack));
    pt = (Tree*) malloc(sizeof(Tree));
    StackNode* s1=(StackNode*)malloc(sizeof(StackNode));
    s1->val=DOLLAR;
    s1->flag=1;
	s1->next=NULL;
    //s1->address=NULL;
    push(st,s1);
    StackNode* s2=(StackNode*)malloc(sizeof(StackNode));
    s2->val=0; // to be defined in enum
    s2->flag=0;
    //push(st,s2);
    TreeNode* t1=(TreeNode*)malloc(sizeof(TreeNode));
    t1->value=0;
    t1->flag=0;
	t1->parent=NULL;
    t1->ASTChild=NULL;
    t1->ASTRight=NULL;
    t1->ASTParent=NULL;
    t1->child=NULL;
    pt->root=t1;
	s2->address=t1;
push(st,s2);
}

// Recursive module to insert a grammar node to both the parse tree and stack
TreeNode* insertST(RhsNode * snode){

    if(snode->nextRhs==NULL){

        TreeNode* tnode=(TreeNode*)malloc(sizeof(TreeNode));
        tnode->value= snode->flag ? snode->S.T : snode->S.NT;
        tnode->flag = snode->flag;
        tnode->ASTChild=NULL;
        tnode->ASTRight=NULL;
        tnode->ASTParent=NULL;
        tnode->child=NULL;
	    StackNode* temp=createNode(snode,tnode);
        push(st, temp);
        return tnode;
    }

    TreeNode* tnodenext = insertST(snode->nextRhs);
    TreeNode* tnode=(TreeNode*)malloc(sizeof(TreeNode));
    tnode->value= snode->flag ? snode->S.T : snode->S.NT;
    tnode->flag = snode->flag;
    tnode->right = tnodenext;
    tnode->ASTChild=NULL;
    tnode->ASTRight=NULL;
    tnode->ASTParent=NULL;
    tnode->child=NULL;
    StackNode* temp1=createNode(snode,tnode);
    push(st, temp1);

    return tnode;
}

// Main module to parse the input code
void parseInput(FILE* code){ // Takes as input the source code file

    initializeStackTree();
    int error_rule=0;
	//int er = 0;
    code=getStream(code);

    while(st->head!=NULL){
       	//printf("\niter\n");
		if(token!=NULL){
	        //printf("token : %s %s \n", token->tokenName, token->tokenLexeme);

	        if(token && strcmp(token->tokenName,"COMMENT")==0){			// No need to parse the comment. Ignore the COMMENT token
	            //printf("reject commment in parser. \n ");
	            code=getStream(code);
	            continue;
	        }

	        mapRecord tokenInf=getRecordFromMap(token->tokenName);

	        if((st->head)->flag==1){ // when the stack head is a terminal

	            if(tokenInf.sym.T==st->head->val){
					if(error_rule==1) error_rule=0;
	            	//puts(token->tokenLexeme);
	            	//puts(terms[st->head->val]);
						if(tokenInf.sym.T==DOLLAR){ // Reached end of the code
							pop(st);
							break;
						}

						TreeNode * temp=(st->head)->address;
	            		//puts(terms[temp->value]);
	            		if(temp!=NULL){
	            			strcpy(temp->tokenName,token->tokenName);
	            			strcpy(temp->tokenLexeme,token->tokenLexeme);
	            			temp->lineNumber=token->lineNumber;
	            		}



			    		code=getStream(code);
		            	pop(st);
		            	continue;
	      		 }
	         	else{ // top of stack terminal and input token don't match.
					if(strcmp(token->tokenName,"DOLLAR")==0){
							//while(st->head!=NULL) pop(st);
							break;
					}
					if(error_rule==0){
                        syntaxerr=1; // for CODEGEN
						printf("Parser.c.%d(SYNTACTICAL ERROR): The input terminal and the terminal on the top of the stack do not match\n",token->lineNumber);
                    }
					error_rule=1;
	                code=getStream(code);
	                continue;
	            }
	        }
	        else{ // when stack head is a  nonterminal

	        	int rule_no= ParseTable[st->head->val][tokenInf.sym.T]; // get rule number from parse table

	            if(error_rule==0 && rule_no==-1){ // Begin of error
                    syntaxerr=1; //for CODEGEN
	            	error_rule = 1;
					printf("Parser.c.%d(SYNTACTICAL ERROR): Terminal does not occur in both the first and follow sets of the Non terminal present on the top of the stack\n",token->lineNumber);
	                code=getStream(code);
	                continue;
	            }
	            if(error_rule==0 && rule_no!=-1){

	            	RhsNode * start=G[rule_no].headRhs;
	                TreeNode * topStackAdd = st->head->address;
	                if(start->flag==1 && start->S.T==EPS){ // in case of null production don't add anything to the stack

	                    TreeNode* t=(TreeNode*)malloc(sizeof(TreeNode));
	                	t->value=EPS;
	                	t->flag=1;
						t->parent=topStackAdd;
                        t->ASTRight=NULL;
                        t->ASTChild=NULL;
                        t->ASTParent=NULL;
                        t->child=NULL;
                        t->right=NULL;
                        t->called=0;
	                	topStackAdd->child=t;
	                	pop(st);
	                	continue;
	                }
	                else{
	                	pop(st);
						TreeNode* temp=insertST(start);
	                	topStackAdd->child=temp;
						while(temp!=NULL){
							temp->parent=topStackAdd;
							temp=temp->right;
						}
	                	continue;
	                }

	            }
	            else if(first[st->head->val][tokenInf.sym.T]==1){ // found in the first set. Error recovery stops here
					error_rule=0;
	                continue;
	            }
	            else if(first[st->head->val][tokenInf.sym.T]!=1){ // still recovering from error
					if(strcmp(token->tokenName,"DOLLAR")==0){
					//while(st->head!=NULL) pop(st);
					break;
				}
	            code=getStream(code);
	            continue;
	            }
        	}
        }
    }
   // displayStack();
    // puts("");
    // displayParseTree();
    // puts("");
	fseek(fp, 0, SEEK_SET);
	endInd=0;
	fileBufInd=0;
	strcpy(fileBuf,"");


}

int countParseTreeNodes(TreeNode * root, int count){
    if(root==NULL){
        return count;
    }
    count+=countParseTreeNodes(root->child,0)+1;

    TreeNode* sib = root->child;
    if(sib) sib = sib->right;
    while(sib!=NULL){
        count+=countParseTreeNodes(sib,0);
        sib = sib->right;
    }
    return count;
}

	//----------------------MODULES FOR DISPLAYING STACK CONFIGURATION AND PARSE TREE---------------------------------
void display(TreeNode* root){
    if(root==NULL)
        return;
    numPTnodes++;
	display(root->child);
	//int isLeaf;
    //if(root->child==NULL;
    if(root->flag==1){
    	char num[50]="";
    	TreeNode * pr=root->parent;
    	if(strcmp(terms[root->value],"RNUM")==0 || strcmp(terms[root->value],"NUM")==0)
    		strcpy(num,root->tokenLexeme);
    	else
    		strcpy(num,"N/A");
    	if(strcmp(terms[root->value],"EPS")==0)
    			printf("%-10s %-25s %-15s %-15s %-15s %-25s %-15s %-15s\n ", "----",terms[root->value],"N/A","----",num,nterms[pr->value],"YES","TERMINAL");
    	else
        	  printf("%-10s %-25s %-15d %-15s %-15s %-25s %-15s %-15s\n ", root->tokenLexeme,terms[root->value],root->lineNumber,root->tokenName,num,nterms[pr->value],"YES","TERMINAL");
    }
    else{
    	  char lexeme[15]="-------";

		if(root->parent==NULL){

		    printf("%-10s %-25s %-15s %-15s %-15s %-25s %-15s %-15s\n ", "----",nterms[root->value],"N/A","N/A","N/A","ROOT","NO","NON-TERMINAL");
    	}
    	else{
    		TreeNode * pr=root->parent;

		    printf("%-10s %-25s %-15s %-15s %-15s %-25s %-15s %-15s\n ", "----",nterms[root->value],"N/A","N/A","N/A",nterms[pr->value],"NO","NON-TERMINAL");
    	}
	// puts(nterms[root->value]);
 //    	//printf("%s ",nterms[root->value]);

 //        //printf("%s ", nterms[root->value]);
        //printf("NT ");
    }
	TreeNode* sib = root->child;
	if(sib) sib = sib->right;
	while(sib!=NULL){
		display(sib);
		sib = sib->right;
	}

}

	// void displayParseTree(char * outfile){
    //         numPTnodes=0;
	// 		FILE * ot=fopen(outfile,"w+");
	// 		if(ot==NULL){
	// 			printf("Unable to open outfile\n");
	// 			return;
	// 		}
	// 		outf=ot;
    //
	// 		fprintf(outf,"%-10s %-25s %-15s %-15s %-15s %-25s %-15s %-15s\n\n ", "Lexeme","Current Node","Line Number","Token Name","ValueIfNumber","ParentNode","isLeafNode","Node Symbol");
	// 		//fprintf(outf,"Lexeme\t\t\tCurrent Node\t\tLine Number\t\tTokenName\t\tValueIfNumber\t\tParentNode Symbol\t\tisLeafNode\t\tNode Symbol\t\n");
	// 		//printf("HELLO");
	// 		TreeNode * root=pt->root;
	// 				//exit(0);
	//         display(root);
	//         printf("\n");
	//         printf("\nParse Tree(inorder traversal) has been saved to the file %s\n",outfile);
	//         //fclose(ot);
	//         //if(outf)fclose(outf);
    //
    // }

void displayParseTree(){
        numPTnodes=0;
        printf("=============================================================================================================================================\n");
    	printf(BOLDWHITE "%70s\n" RESET,"PARSE TREE");
        printf("=============================================================================================================================================\n");

        //fprintf(outf,"%-10s %-25s %-15s %-15s %-15s %-25s %-15s %-15s\n\n ", "Lexeme","Current Node","Line Number","Token Name","ValueIfNumber","ParentNode","isLeafNode","Node Symbol");
        printf(BOLDGREEN "%-10s %-25s %-15s %-15s %-15s %-25s %-15s %-15s\n\n " RESET, "Lexeme","Current Node","Line Number","Token Name","ValueIfNumber","ParentNode","isLeafNode","Node Symbol");
        printf("=============================================================================================================================================\n");

        TreeNode * root=pt->root;
                //exit(0);
        display(root);
        printf("=============================================================================================================================================\n");

        printf("\n");
        //printf("\nParse Tree(inorder traversal) has been saved to the file %s\n",outfile);
        //fclose(ot);
        //if(outf)fclose(outf);

}
void displayStack(){

    StackNode* temp = st->head;
	printf("stack: ");
    while(temp!=NULL){
    	if(temp->flag==1)
		    printf("%s ", terms[temp->val]);
		else
		    printf("%s ", nterms[temp->val]);
		temp = temp->next;
    }
}
