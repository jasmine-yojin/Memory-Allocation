/*===============================================================
gcc JKim_mem.c -o JKim_mem.exe					                                                  
./JKim_mem.exe 1048576                                                                                                                                                                 
================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

//Each node represents a block of memory space. Using LinkedList structure.
struct Node {
	int availableSpace;
	int start;
	int end;
	struct Node *next;
	char processID[50];
};

struct Node *head;
struct Node *initialBlock;
struct Node *temp;

//These variables store the user's input
char request[3];// RQ or RL
char process[3];//ProcessID, process name
int spaceRequested;//Amount of memory requested
char algoType[3];// W represting the worst fit algorithm

//Prints out the state of the memory spaces
void stat (){
	temp = head;
	while (temp->next!=NULL){
		printf("Addresses [%d : %d] %s\n", temp->next->start, temp->next->end, temp->next->processID, temp->next->availableSpace);
		temp= temp->next;
	}
	
	printf("\n");
}

//This function performs the worst-fit memory allocation
int requestMemory(char newProcess[3], int spaceRequested){
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	int largestHole= -9999;
	int leftOverSpace= 0;
	
	//find the largest available space
	temp = head;
	while (temp->next !=NULL){
		if ((strcmp(temp->next->processID, "Free")== 0) && (temp->next->availableSpace >= spaceRequested)){
			if(temp->next->availableSpace >=largestHole){
				largestHole = temp->next->availableSpace;
			}
		}
		
		temp = temp->next;
	}
	
	//If the largestHole variable remains unchanged, there was no free space big enough to contain the memory.End the function.
	if (largestHole == -9999){
		printf("Memory request failed. There is no free hole large enough to fit process size of %d. Try compacting first.\n", spaceRequested);
		return 1;
	}
	
	
	temp=head;
	while(temp->next!=NULL){
		//Insert the new process into the largest free space.
		if ((temp->next->availableSpace==largestHole)&& (strcmp(temp->next->processID, "Free")== 0)){
			strcpy(temp->next->processID, newProcess);
			if (temp->next->start==0) temp->next->end = temp->next->start + spaceRequested;
			else temp->next->end = temp->end + spaceRequested;
			leftOverSpace = temp->next->availableSpace - spaceRequested;
			temp->next->availableSpace = spaceRequested;
		
			//If there is leftover space, create a new node representing this space and place it right after the recently inserted node.
			if(leftOverSpace>0){
				struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
				strcpy(newNode->processID, "Free");
				newNode->availableSpace = leftOverSpace;
				newNode->start = temp->next->end + 1;
				newNode->end = temp->next->end + leftOverSpace;
				
				//Inserting the new node in the list and rearranging the addresses before and after.
				struct Node *nodeAfter = temp->next->next;
				temp->next->next = newNode;
				newNode->next = nodeAfter;
			}
			return;//the new node is inserted. end the function.
		}
		
			temp= temp->next;
	}
	
}

//This one releases the memory space. 
int release (char process[3]){
	temp=head;
	while(temp->next!=NULL){
		if(strcmp(temp->next->processID, process)==0){
			strcpy(temp->next->processID, "Free");//Replace the process name with "Free"
			return 2;
		}
		temp= temp->next;
	}
}

//Pushes all free memory spaces together.
void compact (int initialMemory){
	
	temp=head;
	//These represent the nodes to be swapped
	struct Node* one;
	struct Node* two;
	
	//If you see any free space, swap it with the next process until all free spaces are placed at the end.	
	while(temp->next!=NULL && temp->next->next!=NULL){
		if (strcmp(temp->next->processID, "Free")==0){
			one= temp->next;
			two= temp->next->next;
			swapAvailableSpace(&one->availableSpace, &two->availableSpace);//swapping memory size
			swapProcessID(one->processID, two->processID);//swapping the processID
		}		
		
		temp= temp->next;
	}
	
	//Recalculate the start and end ranges
	temp=head->next;//skip the dummy node
	while(temp->next!=NULL){
		temp->next->start = temp->end+1;
		temp->next->end = temp->end + temp->next->availableSpace;
		
		temp= temp->next;
	}
	
	//Lump the free space together to create a bigger hole.
	temp= head;
	while(temp->next!=NULL){
		if (strcmp(temp->next->processID, "Free")==0){
			temp->next->end = initialMemory;
			temp->next->availableSpace = temp->next->availableSpace+ temp->next->next->availableSpace;
			temp->next->next=NULL;//Discard the extra node now that all free space has become one space.
		}
		temp= temp->next;
	}
	
}

//Helping function- Swaps the available space variable
void swapAvailableSpace(int* a, int* b){
	int temp;
	temp = *a;
	*a= *b;
	*b=temp;
}

//Helping function- Swaps the process variables
void swapProcessID (char a[], char b[]){
	char temp [5];
	strcpy (temp, a);
	strcpy (a,b);
	strcpy (b, temp);
}


int main (int argc, char *argv[]){
	//Convert the memory space into integer
	int initialMemory = atoi(argv[1]);
	printf("Initial Memory: %d\n", initialMemory);
	
	//Creating a null head to indicate the start of the linkedlist
	head = (struct Node *)malloc(sizeof(struct Node));
	initialBlock = (struct Node *)malloc(sizeof(struct Node));
	strcpy(head->processID, "head");
	head->start= -1;
	head->end = -1;
	head->availableSpace = initialMemory;
	head->next = initialBlock;
	
	//Creating a node to represent the initial memory space.
	strcpy(initialBlock->processID, "Free");
	initialBlock->start= 0;
	initialBlock->end = initialMemory;
	initialBlock->availableSpace = initialMemory;
	initialBlock->next = NULL;
	
	char input[128];
	
	stat ();//Initial Stat

//MENU- QUIT, STAT, RQ, RL, C
	printf("allocator>");
	gets (input);
   
while(strcmp(input, "QUIT")!=0){

	if (strcmp(input, "STAT")==0) stat();
	else {
		sscanf(input, "%s %s %d %s", request, process, &spaceRequested, algoType);
		if (strcmp("RQ", request) == 0){
			requestMemory (process, spaceRequested);
			//stat ();
		}else if (strcmp("RL", request) == 0){
			release (process);
			//stat ();
		}else if (strcmp("C", request) == 0){
			compact(initialMemory);
			//stat ();
		}
	}
	
	fflush(stdout);
	printf("allocator>");
	gets (input);
		
}


/*
//TESTING Compaction function
	printf("Stat before compaction\n");
	requestMemory ("P0", 262144);
    requestMemory ("P1", 262144);
    requestMemory ("P2", 262200);
    release("P1");
    requestMemory ("P3", 200000);
    requestMemory ("P4", 200000);
    stat();
    printf("Stat after compaction\n");
    compact(initialMemory);
    stat();
*/	
	return 0;
}
