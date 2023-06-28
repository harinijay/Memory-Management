#include <stdio.h>
#include <stdlib.h>

struct node {
	//Size of the node
	int data;
	//Total of data in the linked list
	int totalData;
	//Availability of memory in a node of the linked list
	int avail;
	//Process number
	int processNo;

	struct node *prev;
	struct node *next;
};

//Total memory of the nodes which are not free
int totalSize = 0;
//Count of the process
int n = 0;
//Total memory (given by user)
int totalMemory;
//Count of the option
int count = 1;
//Size of the new node
int sizeOfNew;
//Position of the relevant node
int tPosition;

//Taking the start as null
struct node *start = NULL;

//Functions called in this program
void allocate();
void terminate();
void selection();
void display();

//Allocating memory blocks
void allocate(){
	//Taking the memory size from the user
	printf("Input the memory size(kb) : ");
	scanf("%d", &sizeOfNew);

    //Using 2 pointers
	struct node *tempNode;
	struct node *previousNode;
	tempNode = start;
	previousNode = start;

    //Creating a new node to represent the allocation of a new memory block
	struct node *newNode = (struct node*)malloc(sizeof(struct node));
	newNode -> next = NULL;
	newNode -> prev = NULL;
  
    //Incrementing the allocating term
	n++;

    //If the creation of the new node fails, inform the user
	if(newNode == NULL){
		printf("No enough space.");
	}
	else{
		//Incrementing the memory size which is not free
		totalSize = totalSize + sizeOfNew;

        //Traversing through the linked list
		while(tempNode -> next != NULL){
			previousNode = tempNode;
			tempNode = tempNode -> next;

            /*If there is a free memory block where the size of it is greater
			than or equal to the newly entering memory, traversing stops*/
			if(tempNode -> avail == 0 && tempNode -> data >= sizeOfNew){
				break;
			}
		}

        
		/*Check whether the total size(existing memory size) is less than
		the total memory(maximum memory given by the user)*/
		if(totalSize < totalMemory){
			/*Check whether the size of the selected free memory block is equal to the
			size of the new memory block*/
			if(tempNode -> data == sizeOfNew){
				/*If the above condition is true, assigning the process num of that free
				node with n*/
				tempNode -> processNo = n;
				//Recording the availability of a memory block
				tempNode -> avail = 1;	
			}
			/*If the size of the selected free memory block is greater than the size of 
				the new memory block, allocating the new memory to it and keeping the remaining
				space as a free block*/
			else{
				//Inserting a new node in between selected node and the node before it
				previousNode -> next = newNode;
				newNode -> next = tempNode;
				newNode -> prev = previousNode;
				tempNode -> prev = newNode;

                //Assigning the process number of the new node
				newNode -> processNo = n;
				//Recording the availability of a memory block
				newNode -> avail = 1;
				//Data in the new node is, the user given memory size
				newNode -> data = sizeOfNew;
				//Increment of total size of memory available until now
				newNode -> totalData = previousNode -> totalData + sizeOfNew;
			}
		}
		//If the given memory is greater than the maximum memory
		else{
			printf("No space.");
			totalMemory = totalMemory - sizeOfNew;
		}
	}
}

//Terminating memory blocks
void terminate(){
	int mark = 0;

	struct node *tempNode = start;
	struct node *previousNode = start;

    //Taking the number of the memory block which should be terminate
	printf("\nInput the memory partition which should be terminate : P ");
	scanf("%d", &tPosition);

    //Traversing through the linked list to find the relevant node
	while(tempNode -> next != NULL){
		previousNode = tempNode;
		tempNode = tempNode ->next;

        //Finding the relevant node
		if(tempNode -> processNo == tPosition){
			//After finding this position recording it as 1
			mark = 1;
			//Exit from the loop
			break;
		}
	}

    //If the mark is '0' (The given position is not found)
	if(mark ==0){
		printf("No relevant memory partition.");
	}
	//If the given position is found
	else{
		/*When the two nodes which are previous and next of the relevant node are also free of memory,
		we delete the first two nodes of them and updating the last node of them*/
		if(tempNode -> avail == 1 && previousNode -> avail == 0 && tempNode -> next -> avail == 0){
			previousNode -> prev -> next = tempNode -> next;
			tempNode -> next -> prev = previousNode -> prev;

			tempNode -> next -> data = (tempNode -> next -> data) + (tempNode -> data) + (previousNode -> data);
			totalSize = totalSize - (tempNode -> data);

			free(previousNode);
			free(tempNode);
		}
		/*When the next node of the relevent node is free of memory, we delete the relevant node 
		and updates the next node*/
		else if(tempNode -> next -> avail == 0){
			tempNode ->  next -> prev = previousNode;
			previousNode -> next = tempNode -> next;

			totalSize = totalSize - (tempNode -> data);

			free(tempNode);
		}
		/*When the previous node of the relevent node is free of memory, we delete the previous node 
		and updates the relevant node*/
		else if(previousNode -> avail == 0){
			previousNode -> prev -> next = tempNode;
			tempNode -> prev = previousNode -> prev;

			tempNode -> data = (tempNode -> data) + (previousNode -> data);

			totalSize = totalSize - (tempNode -> data);

			free(previousNode);
		}
		//If only the relevant node is free, just updating it
		else{
			tempNode -> avail = 0;
			tempNode -> processNo = 0;

			totalSize  = totalSize - (tempNode -> data);
		}
	}
}


//Selecting an option
void selection(){
	int option;
	count++;

    //Options..
	printf("\n\n1.Allocate memory ");
	printf("\n2.Terminate memory");
	printf("\n3.End");

	printf("\n\nInsert the option : ");
	scanf("%d", &option);

    
	switch(option){
		//If the option is '1'
		case 1: 
		allocate();
		display();
		selection();
		break;

        //If the option is '2'
		case 2:
		terminate();
		display();
		selection();
		break;

        //If the option is '3'
		case 3:
		exit(0);

        //If the user mistakenly gives another option
		default:
		printf("Try again....");
		selection();
	}
}


//Displaying the snapshot of memory representation
void display(){
	struct node *tempNode;

    //If the start is null, informing the user as no data is available
	if(start == NULL){
		printf("No data available");
	}

    //If data is available in the linked list....
	else{

		tempNode = start;

		//When the tempory node is not null,
		while(tempNode != NULL){
			/*Start node contains the size of the operating system, when the
			temp node is equal to start */
			if(start == tempNode){
				printf("\n0k\t-----------------------------");
				printf("\n\t|                           |");
				printf("\n\t|     Operating System      |");
				printf("\n\t|                           |");
				printf("\n%dk\t-----------------------------", tempNode -> data);

                //Temp node pointer travels to the next node
				tempNode = tempNode -> next;
			}
			//When the availability of memory in the tempnode is '0', builds a free block
			else if(tempNode -> avail == 0){
				printf("\n\t|                           |");
				printf("\n\t|           free            |");
				printf("\n\t|                           |");
				printf("\n%dk\t-----------------------------", tempNode -> totalData);

                //Temp node pointer travels to the next node
				tempNode = tempNode -> next;
			}
			//When memory is available in the tempnode, shows the memory with its number
			else if(tempNode -> avail == 1){
				printf("\n\t|                           |");
				printf("\n\t|           P%d              |", tempNode -> processNo);
				printf("\n\t|                           |");
				printf("\n%dk\t-----------------------------", tempNode -> totalData);

                //Temp node pointer travels to the next node
				tempNode = tempNode -> next;
			}
		}
	}
}

//Main function
void main(){
	//Creating two nodes for the os and the remaining free memory
	struct node *operatingSystem = (struct node*)malloc(sizeof(struct node));

	operatingSystem -> next = NULL;
	operatingSystem -> prev = NULL;

	struct node *end = (struct node*)malloc(sizeof(struct node));

	end -> next = NULL;
	end -> prev = NULL;

	start = operatingSystem;

	printf("\nInsert the size of the operating system(kb) : ");
	scanf("%d", &operatingSystem -> data);

	printf("\nInsert the maximum memory(kb) : ");
	scanf("%d", &end -> data);

    //Assigning the process num of os as '0'
	operatingSystem -> processNo = 0;
	//Updating the size of the os with user given data
	operatingSystem -> totalData = operatingSystem -> data;
	//Marking as memory is available in the os
	operatingSystem -> avail = 1;

	//Joining the two nodes
	operatingSystem ->  next = end;
	end -> prev = operatingSystem;

    //Assigning the process number of the end node as '0'
	end -> processNo = 0;
	//Calculating the free memory size
	end -> totalData = (operatingSystem -> data) + (end -> data);
	//Marking as no memory is available in this node
	end -> avail = 0;
	//Total memory is given as the free memory
	totalMemory = end -> totalData;

                /*Printing the memory which is available before the process(allocating
				and terminating) begin*/
	            printf("\n\t-----------------------------");
				printf("\n\t|                           |");
				printf("\n\t|     Operating System      |");
				printf("\n\t|                           |");
				printf("\n%dk\t-----------------------------", operatingSystem -> totalData);

				printf("\n\t|                           |");
				printf("\n\t|           free            |");
				printf("\n\t|                           |");
				printf("\n%dk\t-----------------------------", end -> totalData);
    
	//Choosing an option
    selection();
}