#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
/*Ian De Leon 1946676
This program reads text from a file, it then generates and sorts the text into a doubly linked list. 
After, the program prints the linked list either in ascending or descending alphabetical order depending on the -d flag.
The -o flag can be used to choose an output file location.
I affirm that this is 100% my work!
*/

//Handles input into an array.
char** CreateWordArray(FILE* inputFile, char** wordArray, int* size){
    //Expects word lenght to not exceed 50 characters
    int capacity = sizeof(char**) * 50;
    int usedSpace = 0;
    int stringSize =0;
    int fileRead = 0;

    char* tempString = (char*)malloc(capacity);
    wordArray = (char**)calloc(capacity, sizeof(char*));
    fileRead = fscanf(inputFile, "%s", tempString);
    //Reads the file while it is not empty
    //!feof(inputFile)
    while(!feof(inputFile)){
        //Takes in word input
        stringSize = strlen(tempString);
        //turns string to lowercase and checks for non letters
        for(int j = 0; j < stringSize; j++){
            if(isalpha(tempString[j])){
                tempString[j] = tolower(tempString[j]);
            }
            else{
                tempString[j] = ' ';
            }
        }
        //Allocates memory for the word within the array/ 
        wordArray[*size] = (char*)malloc(stringSize+1);
        //places word into array   
        strcpy(wordArray[*size], tempString);
        usedSpace += stringSize+1;
        (*size)++;  
        
        if(wordArray == NULL){
            printf("%s\n", "Something else went wrong, ran out of space");
        }
        //Makes sure the array has enough memory to keep adding characters
        if(usedSpace >= capacity/2){
            capacity = capacity * 2;
            wordArray = (char**)realloc(wordArray, capacity * sizeof(char*));
        }
        fileRead = fscanf(inputFile, "%s", tempString);
    }

    free(tempString);
    return wordArray;
    
}
//Doubly linked list code starts here
 //Creates a struct that will be the template for all nodes.
typedef struct StringNode_struct {
    char* storedString;
    struct StringNode_struct* nextNodePointer;
    struct StringNode_struct* previousNodePointer;
    } StringNode;

//Node constructor
void CreateNode(StringNode* newNode, char* inputString, StringNode* nextNode, StringNode* previousNode){
    newNode->storedString = inputString;
    newNode->nextNodePointer = nextNode;
    newNode->previousNodePointer = previousNode;
}
//Creates a new node at the front, and assigns the last start node as the 'new' front node's next node.
void InsertStart(StringNode* currentNode, char* inputString, StringNode* newStartNode){
    currentNode->previousNodePointer = newStartNode;
    newStartNode->storedString = inputString;
    newStartNode->nextNodePointer = currentNode;
    newStartNode->previousNodePointer = NULL;
}
//Creates a new node at the end, and assigns the next start node as the 'new' front node's next node
void InsertEnd(StringNode* currentNode, char* inputString, StringNode* newLastNode){
    currentNode->nextNodePointer = newLastNode;
    newLastNode->storedString = inputString;
    newLastNode->previousNodePointer = currentNode;
    newLastNode->nextNodePointer = NULL;
}
//Frees up the Linked list
void ReclaimList(StringNode* currentNode){
    while(currentNode->nextNodePointer != NULL){
        currentNode = currentNode->nextNodePointer;
        //Useful debug print statement
        //printf("deleting %s: \n", currentNode->storedString);
        if(currentNode->previousNodePointer!= NULL){
            free(currentNode->previousNodePointer);
        }
    }
    
}
//Prints a node
int PrintNode(StringNode* currentNode){
    if(currentNode == NULL){
        printf("Error, node is empty and cannot be printed");
        return 1;
    }
    if(currentNode->storedString != NULL){
        printf("%s\n", currentNode->storedString);
    }
    return 0;
}
//Prints the LL in ascending alphabetical order
void PrintList(StringNode* currentNode, int oflag, FILE* outputFile){
    
    StringNode* dummyNode = NULL;
    dummyNode = currentNode;
    while(dummyNode->nextNodePointer != NULL){ 
        if(oflag == 0){    
            PrintNode(dummyNode);
        }
        else{
            fprintf(outputFile, "%s\n", dummyNode->storedString);
        }
        dummyNode = dummyNode->nextNodePointer;       
    }
    if(oflag==0){
        PrintNode(dummyNode);
    }
    else{
        fprintf(outputFile, "%s\n", dummyNode->storedString);
    }
    //Reclaims the memory from the list after printing.    
    ReclaimList(currentNode);
}
//Prints the LL in descending alphabetical order
void PrintListBackwards(StringNode* currentNode, int oflag, FILE* outputFile){
    StringNode* dummyNode = NULL;
    dummyNode = currentNode;
    //Useful debug print statement
    //printf("Debug print current node: %s, %p\n", currentNode->storedString, currentNode->previousNodePointer);
    while(dummyNode->previousNodePointer != NULL){  
        if(oflag == 0){ 
            PrintNode(dummyNode);
        }
        else{
            fprintf(outputFile, "%s\n", dummyNode->storedString);
        }
        dummyNode = dummyNode->previousNodePointer;      
    }
    if(oflag == 0){    
        PrintNode(dummyNode);
    }
    else{
        fprintf(outputFile, "%s\n", dummyNode->storedString);
    }
    //Reclaims the memory from the list after printing.
    ReclaimList(dummyNode);
}

//Handles the creation and printing of the linked list and sorting.
StringNode* CreateSortedLinkedList(char** wordArray, int* size, int dflag){
    StringNode* firstNode;
    StringNode* currentNode;
    StringNode* lastNode;
    StringNode* tempNode;
    StringNode* tempNextNode;
    char* tempString = NULL;
    int compareResult = 1;
    
    firstNode = (StringNode*)malloc(sizeof(StringNode));
    currentNode = (StringNode*)malloc(sizeof(StringNode));
    
    //Create the first node
    tempString = wordArray[0];
    
    CreateNode(firstNode, tempString, NULL, NULL);
    currentNode = firstNode;
    
    //Iterates through word array to create the sorted linked list
    for(int i = 1; i < *size;i++ ){             
        currentNode = firstNode;
        tempString = wordArray[i];
        tempNode = (StringNode*)malloc(sizeof(StringNode));
        compareResult = strcmp(tempString, currentNode->storedString);
        //Traverses to the right if stored string is greater than current node.    
        if (compareResult > 0){
            //If compare is 0 both loops should skip
            while(compareResult > 0 ){
                compareResult = strcmp(tempString, currentNode->storedString);
                //creates new node if there is no next node             
                if(currentNode->nextNodePointer == NULL){
                    InsertEnd(currentNode, tempString, tempNode);
                    compareResult = 0;
                }
                //creates a new node in between two nodes if there is no NULL
                else if (compareResult < 0){
                    CreateNode(tempNode, tempString,currentNode ,currentNode->previousNodePointer );
                    //Changes the pointer of the next node to be the newly added node
                    tempNextNode =currentNode->previousNodePointer;
                    tempNextNode->nextNodePointer= tempNode;
                    //Changes the pointer of the current node to the newly created node.
                    currentNode->previousNodePointer = tempNode;
                }
                if(compareResult != 0){       
                    currentNode = currentNode->nextNodePointer;
                }   
            }
        }
        //traverses left if the stored string is less than current node.
        //Always start from first node is this required? Yes what if its less than first node.
        else if(compareResult < 0){
            
            InsertStart(currentNode, tempString, tempNode);
            firstNode = tempNode;
            compareResult = 0;           
        }
        lastNode = currentNode->nextNodePointer;     
    }
    //PrintNode(lastNode);
    currentNode = firstNode;
    //Prints the list,uses oflag to pick a direction
    if(dflag == 0){
        return firstNode;
    }
    else{
        return lastNode;
    }
}

int main(int argc , char **argv){
    extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int dflag=0, oflag=0;
    int size = 0;
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;
    StringNode* toPrintNode = NULL;
    static char **wordArray = NULL;
    
	static char usage[] = "usage: %s [-d] [-o output_file_name] input_file_name \n";
	//Deals with the different command line arguments via getopt
    //-p is the percent, -h is hours worked, -b is hourly rate
    while ((c = getopt(argc, argv, "do:")) != -1)
		switch (c) {
		case 'd':
            //If d flag is on program will print from last to first.
			dflag =1;
			break;	
		case 'o':
            //o flag is used to print to an optional output file
			oflag =1;
            outputFile = fopen(optarg, "w");
			break;	                        
		case '?':
			err = 1;
			break;
		}
    
    if ((optind + 1) > argc) {	
		/* need at least one argument (change +1 to +2 for two, etc. as needeed) */

		printf("optind = %d, argc=%d\n", optind, argc);
		fprintf(stderr, "%s: missing value\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 
    else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
    //Takes the input file from argv
    inputFile = fopen(argv[optind], "r");
    if(inputFile == NULL){
        printf("File %s failed to open\n", argv[optind]);
        return -1;
    }
    //Creates an array that will be used to create the list
    wordArray = CreateWordArray(inputFile, wordArray, &size);
    //Sends the array to be turned into a sorted linked list, put its into a node that will be used as a pointer for printing.
    toPrintNode = CreateSortedLinkedList(wordArray, &size, dflag);
    //Selects from D flag to choose which direction to print
    if(dflag == 0){
        PrintList(toPrintNode, oflag, outputFile);
    }
    else{
        PrintListBackwards(toPrintNode, oflag, outputFile);
    }
    
    fclose(inputFile);
    //Closes outputfile only if one was used
    if(oflag ==1){
        fclose(outputFile);
    }
    return 0;
}
