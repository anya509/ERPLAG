//Group Number 4
//2016B1A70280P -- Akshina Jindal
//2017A7PS0276P -- Kavya Gupta
//2016B4A70322P -- Satyavrat Sharma
//2016B4A70509P -- Nikita N. Singh
int numPTnodes=0;
int numASTnodes=0;
int syntaxerr=0; // lexer.c
int sterr=0;     // symboltable.c
int tcerr=0;     // typechecker.c
int othererr=0;  // others in SA
int semanticerr=0; // SA
#include <stdio.h>
#include <time.h>
//#include "parser.c"
//#include "symbolTable.c"
//#include "typeChecker.c"
#include "codeGeneration.c"
//#include "semanticAnalyser.c"
void displayMenu(){
    printf("\nFUNCTIONALITIES: \n");
    printf("- PRESS 0 TO EXIT\n");
    //printf("- PRESS 1 TO REMOVE COMMENTS FROM SOURCE FILE AND CREATE A CLEAN FILE\n");
    printf("- PRESS 1 FOR LEXER\n");
    printf("- PRESS 2 FOR PARSER\n");
    //printf("- PRESS 4 FOR DISPLAYING THE TIME TAKEN BY PROGRAM\n");
	//printf("- PRESS 5 FOR DISPLAYING THE MAPPING TABLE\n");
    //printf("- PRESS 6 FOR DISPLAYING THE GRAMMAR\n");
    //printf("- PRESS 7 FOR DISPLAYING THE FIRST SETS\n");
    //printf("- PRESS 8 FOR DISPLAYING THE FOLLOW SETS\n");
    //printf("- PRESS 9 FOR DISPLAYING THE PARSE TABLE\n");
    printf("- PRESS 3 FOR AST\n");
    printf("- PRESS 4 FOR MEMORY\n");
    printf("- PRESS 5 FOR SYMBOL TABLE\n");
    printf("- PRESS 6 FOR ACTIVATION RECORD SIZE\n");
    printf("- PRESS 7 FOR STATIC AND DYNAMIC ARRAYS\n");
    printf("- PRESS 8 FOR ERROR REPORTING AND TOTAL COMPILE TIME\n");
    printf("- PRESS 9 FOR CODE GENERATION\n");
    //printf("- PRESS 11 FOR BUILDING THE SYMBOL TABLE\n");
}

int main(int argc, char * argv[]) {

    int option;
    char * file[50];
    FILE * gram=fopen("grammar_latest.txt", "r");
    //setting up the maps and first and follow sets

    clock_t    start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();


    buildKeywordTable();
    buildMappingTable();
    buildGrammar(gram);
    populate();
    findFirst();
    fillfirstRHS();
    populate_hash();
    findfollow();
    makeParseTable();

    int isdone[10] = {0,0,0,0,0,0,0,0,0,0};
    int isast=0; int isst = 0;

    end_time = clock();
    total_CPU_time  =  (double) (end_time - start_time);
    total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
    //printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
    //printf("=====================================\n");


    printf("=====================================\n");
    printf(BOLDCYAN "%15s\n" RESET,"COMPILER PROJECT: GROUP_4");
    printf("=====================================\n");
    printf("=====================================\n");
    printf(BOLDCYAN "%15s\n" RESET,"LEVEL 4: (Everything Specified) Symbol table/type Checking/ Semantic rules / static and dynamic arrays in type checking and code generation");
    printf("=====================================\n");

    printf("=====================================\n");
    printf(BOLDCYAN "%15s\n" RESET,"STATUS OF IMPLEMENTATION");
    printf("=====================================\n");
    printf("1. All Files provided as test cases work completely in accordance with expected output.\n");
    printf("2. All Expected 'ERROR' are printed, No expected 'not an error' is printed\n");
    printf("3. The Test Files are: t1.txt to t10.txt, c1.txt to c11.txt, ST.txt (given for symbol table expected output).\n");
    printf("4. Extra Test Files : t11.txt (for loop construct), t12.txt (while loop construct) \n");
    
    printf("=====================================\n");
    printf(BOLDCYAN "%15s\n" RESET,"NOTE");
    printf("=====================================\n");
    printf("1. Compile generated code.asm file using following command using NASM: \n");
    printf("\tnasm -f elf64 code.asm && gcc -no-pie code.o && ./a.out\n");
    printf(BOLDCYAN "%15s\n" RESET,"\tOR as: \n");
    printf("\tnasm -f elf64 code.asm\n");
    printf("\tgcc -no-pie code.o\n");
    printf("\t./a.out\n");
    printf("2. In t8.txt, line number 91, expected error should be different because of typo in line 68 and 91 with use of var4 and Var4 respectively.\n");
    printf("3. In c4.txt, Expected output should be different.\n");

    printf("Total Preprocessing in time  : %f\n", total_CPU_time);
    printf("Total Preprocessing in time in seconds : %f\n", total_CPU_time_in_seconds);
    printf("=====================================\n");
    //printf("Enter the name of the source code file: ");
    //scanf("%s",file);
    addDollar(argv[1]);
    FILE * code1=fopen(argv[1],"r");
    while(1){

        displayMenu();
        printf("Enter the option[0-8]. Press 0 to exit:");
        scanf("%d",&option);
        printf("\n");
        switch(option){
            case 1:{
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

                // get stream and create token list
				//fseek(code1, 0, SEEK_SET);
                printf("=====================================\n");
                printf(BOLDWHITE "%15s\n" RESET,"LEXER");
                printf("=====================================\n");
                lineno=1;
				lexicalAnalysis(code1);

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("=====================================\n");
				printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");
                isdone[1]=1;
				break;
            }
            case 4:{
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

                lineno=1;
                parseInput(code1);
                if(syntaxerr==1){
                    printf(BOLDRED "%15s\n" RESET,"Syntactically incorrect code. Can't create AST. Hence no memory for AST.\n");
                    break;
                }
                makeAst(pt->root);
                numASTnodes=0;
                assignAstParent(pt->root); // before every call make numastnode=0
                isast=1;

                printf("=====================================\n");
                printf(BOLDWHITE "%15s\n" RESET,"MEMORY ANALYSIS");
                printf("=====================================\n");
                printf("Parse Tree: \n");
                int numP=countParseTreeNodes(pt->root,0);
                //printf("Number Of Nodes  =  %d\n", numPTnodes);
                
                printf("Number Of Nodes  =  %d\n", numP);
                printf("Allocated Memory =  %lu\n", numP*sizeof(TreeNode));
                printf("Abstract Syntax Tree: \n");
                printf("Number Of Nodes  =  %d\n", numASTnodes);
                printf("Allocated Memory =  %lu\n", numASTnodes*sizeof(TreeNode));
                printf("\nCompression Percentage =  %f\n", (float)(numP-numASTnodes)/numP);
                printf("=====================================\n");
                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");
                isdone[4]=1;
                break;
            }
            case 2:{
		        lineno=1;
        		//lexicalAnalysis();
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

                printf("=====================================\n");
                printf(BOLDWHITE "%15s\n" RESET,"PARSER");
                printf("=====================================\n");
                if(code1==NULL){
                    printf(BOLDRED "%15s" RESET,"Unable to open source code file\n");
                    exit(0);
                }
			    //fseek(code1, 0, SEEK_SET);
                lineno=1;
                parseInput(code1);

                displayParseTree();
                //printf("%d\n", numPTnodes);
                printf(BOLDGREEN "%15s\n" RESET,"Parse Done\n");
                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("=====================================\n");
                printf("Total CPU time  : %f\n", total_CPU_time);
				printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");
                isdone[2]=1;
                break;
            }
            case 3:{
                if(syntaxerr==1){
                    printf(BOLDRED "%15s\n" RESET,"Syntactically incorrect code. Can't create AST. \n");
                    break;
                }
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

                // printf("=====================================\n");
                // printf(BOLDWHITE "%15s\n" RESET,"ABSTRACT SYNTAX TREE");
                // printf("=====================================\n");
                lineno=1;
                parseInput(code1);
                if(syntaxerr==1){
                    printf(BOLDRED "%15s\n" RESET,"Syntactically incorrect code. Can't create AST. \n");
                    break;
                }
                makeAst(pt->root);
                numASTnodes=0;
                assignAstParent(pt->root); // before every call make numastnode=0
                isast=1;
                displayAST(pt->root);
                printf("\n");
                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("=====================================\n");
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");
                isdone[3]=1;
                break;

            }
            case 5:{
        
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

                lineno=1;
                parseInput(code1);
                if(syntaxerr==1){
                    printf(BOLDRED "%15s\n" RESET,"Syntactically incorrect code. Can't create AST. Hence no Symbol Table. \n");
                    break;
                }
                makeAst(pt->root);
                numASTnodes=0;
                assignAstParent(pt->root); // before every call make numastnode=0
                isast=1;

                printf(BOLDCYAN "%15s\n" RESET,"Populating the symbol table\n");
                int nestL=1;
                char * scope=(char *)malloc(50*sizeof(char));
                strcpy(scope,"");
                initializeTables();
                buildSymbolTable(pt->root, scope, nestL);
                isst=1;
                printf(BOLDCYAN "%15s\n" RESET,"Displaying the symbol table\n");
                strcpy(scope,"");
                printSymbolTable(pt->root, scope);

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");
                isdone[5]=1;
                break;

            }
            case 6:{
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();


                if(syntaxerr==1){
                    printf(BOLDRED "%15s\n" RESET,"Can not create AST due to Syntactically Incorrect Code. Hence no Activation Record.\n" );
                    break;
                }
                if(isst==0){
                    printf(BOLDCYAN "%15s\n" RESET,"Create Symbol Table using 5.\n");
                    break;
                }

                displayActivationRecord();

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");

                isdone[6]=1;
                break;
            }
            case 7:{
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();

                if(syntaxerr==1){
                    printf(BOLDRED "%15s\n" RESET,"Can not create AST due to Syntactically Incorrect Code. Hence can't display array elements.\n" );
                    break;
                }
                if(isst==0){
                    printf(BOLDCYAN "%15s\n" RESET,"Create Symbol Table using 5.\n");
                    break;
                }

                displayArrayElements();

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");

                isdone[7]=1;
                break;
            }
            case 8:{
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();


                printf("=====================================\n");
                printf(BOLDWHITE "%15s\n" RESET,"SYNTAX CHECK");
                printf("=====================================\n");
                if(code1==NULL){
                    printf(BOLDRED "%15s\n" RESET,"Unable to open source code file\n");
                    exit(0);
                }
                //fseek(code1, 0, SEEK_SET);
                lineno=1;
                parseInput(code1);
                //printf("\n-------------Parse Done------------\n");
                //printf("%d \n", syntaxerr);
                if(syntaxerr){
                    printf(BOLDRED "%15s\n" RESET,"Syntactically Incorrect Code. No further semantic & type error reporting. \n");
                    break;
                }
                printf(BOLDGREEN "%15s\n" RESET,"No syntax errors encountered.\n");
                makeAst(pt->root);
                assignAstParent(pt->root);
                isast=1;
                char * scope=(char *)malloc(50*sizeof(char));
                strcpy(scope,"");
                initializeTables();

                printf("=====================================\n");
                printf(BOLDWHITE "%15s\n" RESET,"TYPE AND SEMANTIC CHECK");
                printf("=====================================\n");
                printf(BOLDCYAN "%15s\n" RESET,"During symbol table creation:\n");
                strcpy(scope,"");
                buildSymbolTable(pt->root, scope, 1);
                isst=1;
                if(sterr==0)
                    printf(BOLDGREEN "%15s\n" RESET,"No error found.\n");
                strcpy(scope,"");
                printf(BOLDCYAN "%15s\n" RESET,"During type checking:\n");
                typeChecker(pt->root, scope);
                if(tcerr==0)
                    printf(BOLDGREEN "%15s\n" RESET,"No error found.\n");
                strcpy(scope,"");
                semanticAnalysis(pt->root, scope);
                if(othererr==0)
                    printf(BOLDGREEN "%15s\n" RESET,"No error found.\n");
                if(sterr==0 && tcerr==0 && othererr==0 && semanticerr==0)
                    printf(BOLDGREEN "%15s\n" RESET,"No error found during complete process. Hence code compiles successfully. \n");
                printf("=====================================\n");

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");

                isdone[8]=1;
                break;
            }
            case 9:{
                clock_t    start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
                //printf("%d", isdone[9]);
                if(isdone[9]==1) {printf(BOLDCYAN "%15s" RESET,"Generated code already saved to file : "); printf("%s.\n", argv[2]); break;}
                printf("=====================================\n");
                printf(BOLDWHITE "%15s\n" RESET,"CODE GENERATION");
                printf("=====================================\n");
                printf(BOLDCYAN "%15s\n" RESET,"ASSUMPTION: There is no syntactic, semantic or type mismatch error in the test case. \n");
                //lineno=1;
                //lexicalAnalysis(code1);
                lineno=1;
                parseInput(code1);
                makeAst(pt->root);
                numASTnodes=0;
                assignAstParent(pt->root);
                isast=1;
                char * scope=(char *)malloc(50*sizeof(char));
                strcpy(scope,"");
                initializeTables();
                strcpy(scope,"");
                buildSymbolTable(pt->root, scope, 1);
                isst=1;
                //typeChecker(pt->root, scope);//ornot?
                //semanticAnalysis(pt->root, scope);//ornot?
                initalizeAssembyCode();
                strcpy(scope,"");
                generateAssemblyCode(pt->root,scope);
                //displayAssemblyCode(); // Comment me later
                //printf(BOLDRED "%15s" RESET,"Comment me later.\n");
                if(argc>=3){
                    FILE * asmfp=fopen(argv[2], "w+");
                    if(asmfp==NULL){
                        printf(BOLDRED "%15s" RESET,"Unable to open file given as argument.\n"); break;
                    }
                    saveAssemblyCodeToFile(asmfp);
                    printf(BOLDCYAN "%15s" RESET,"Generated code saved to file : "); printf("%s.\n", argv[2]); 
                    //fclose(asmfp);
                }
                else{
                    printf(BOLDRED "%15s" RESET,"Please specify asm file as argument.\n");
                }

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time  : %f\n", total_CPU_time);
                printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
                printf("=====================================\n");

                isdone[9]=1;
                break;
            }
            // case 11:{
            //     lineno=1;
            //     lexicalAnalysis(code1);
            //     lineno=1;
            //     parseInput(code1);
            //     makeAst(pt->root);
            //     numASTnodes=0;
            //     assignAstParent(pt->root);

            //     printAST(pt->root);
            //     printf("\n");
            //     clock_t    start_time, end_time;
            //     double total_CPU_time, total_CPU_time_in_seconds;
            //     start_time = clock();
            //     printf("\n---Populating the symbol table-------\n\n");
            //     int nestL=1;
            //     char * scope=(char *)malloc(50*sizeof(char));
            //     strcpy(scope,"");
            //     initializeTables();
            //     buildSymbolTable(pt->root, scope, nestL);
            //     isst=1;

            //     printf("\n---Type & Semantics Check-------\n");
            //     strcpy(scope,"");
            //     typeChecker(pt->root, scope);
            //     semanticAnalysis(pt->root, scope);

            //     printf("\n---Displaying the symbol table-------\n");
            //     //displaySymbolTable();
            //     //printf("\nthis oen\n");
            //     printSymbolTable(pt->root, scope);

            //     initalizeAssembyCode();
            //     printf("\n---Generating Assembly language code-------\n");
            //     generateAssemblyCode(pt->root,scope);
            //     displayAssemblyCode();
            //     end_time = clock();
            //     total_CPU_time  =  (double) (end_time - start_time);
            //     total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
            //     printf("Total CPU time  : %f\n", total_CPU_time);
            //     printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
            //     printf("=====================================\n");
            //     break;

            // }
            case 0:{
                printf("Thank you!\n");
                exit(0);
            }
//         case 1: {
//          //puts(argv[0]);
//             clock_t    start_time, end_time;
//             double total_CPU_time, total_CPU_time_in_seconds;
//             start_time = clock();

//             printf("---------------Removing comments-----------------\n");
//             removeComments(argv[1],"CommentFreeFile.txt");
//             printf("\n\n---Clean file saved to CommentFreeFile.txt-------\n");

//             end_time = clock();
//             total_CPU_time  =  (double) (end_time - start_time);
//             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
            // printf("Total CPU time  : %f\n", total_CPU_time);
//             printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
//             printf("=====================================\n");
//             break;
//         }
        // case 4:{
            //  printf("Total Preprocessing in time  : %f\n", total_CPU_time);
            //  printf("Total Preprocessing in time in seconds : %f\n", total_CPU_time_in_seconds);
   //           printf("Corresponding procesing time shown in respective case itself.\n");
            //  break;
            // }
//         case 5:{
//             clock_t    start_time, end_time;
//             double total_CPU_time, total_CPU_time_in_seconds;
//             start_time = clock();

//             printf("---Displaying the mapping table-----\n");
//             displayMappingTable();

//             end_time = clock();
//             total_CPU_time  =  (double) (end_time - start_time);
//             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
//             printf("Total CPU time  : %f\n", total_CPU_time);
            // printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
//             printf("=====================================\n");
//             break;
//         }
//         case 6:{
//             clock_t    start_time, end_time;
//             double total_CPU_time, total_CPU_time_in_seconds;
//             start_time = clock();

//             printf("--------Displaying the grammar------\n");
//             displayGrammar();

//             end_time = clock();
//             total_CPU_time  =  (double) (end_time - start_time);
//             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
//             printf("Total CPU time  : %f\n", total_CPU_time);
            // printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
//             printf("=====================================\n");
//             break;
//         }
//         case 7:{
//             clock_t    start_time, end_time;
//             double total_CPU_time, total_CPU_time_in_seconds;
//             start_time = clock();

//             printf("---Displaying the first sets-------\n");
//             printf("FIRST SETS: \n\n");
//             displayFirst();
//             printf("\n\nFIRST RHS SETS: \n\n");
//             displayFirstRhs();

//             end_time = clock();
//             total_CPU_time  =  (double) (end_time - start_time);
//             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
//             printf("Total CPU time  : %f\n", total_CPU_time);
            // printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
//             printf("=====================================\n");
//             break;
//         }
//         case 8:{
//             clock_t    start_time, end_time;
//             double total_CPU_time, total_CPU_time_in_seconds;
//             start_time = clock();

//             printf("---Displaying the follow sets------\n");
//             printf("FOLLOW SETS: \n\n");
//             displayFollow();

//             end_time = clock();
//             total_CPU_time  =  (double) (end_time - start_time);
//             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
//             printf("Total CPU time  : %f\n", total_CPU_time);
            // printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
//             printf("=====================================\n");
//             break;
//         }
//         case 9:{
//             clock_t    start_time, end_time;
//             double total_CPU_time, total_CPU_time_in_seconds;
//             start_time = clock();

//             printf("--------Display Parse Table--------\n");
//             displayParseTable();

//             end_time = clock();
//             total_CPU_time  =  (double) (end_time - start_time);
//             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
//             printf("Total CPU time  : %f\n", total_CPU_time);
            // printf("Total CPU time in seconds : %f\n", total_CPU_time_in_seconds);
//             printf("=====================================\n");
//             break;
//         }
            default: break;
        }
    }

}
