/* 
* Developed by:
* Jacob Gidley, Team Leader, jgidley@albany.edu
* Daniel Leo, Recorder, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains all the functions needed to allow the user to perform the nessecary actions of the program.
*/

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "flightRec.h"

// Delare linked list using typedef
typedef struct node
{
	char symbol[5];	// Stores symbol
	int count;				// Stores count value
	struct node *next;		// Node pointer to next node
} Node, *NodePtr;		// Node = struct node, *NodePtr = struct node*

/*
Function Description:
Inserts a new node into the linked list unless there exists a node with the same
string symbol as the one the user inputted. If that occurs, then the node with the same string symbol
will have it's count incremented by 1.

Parameter(s):
0: struct node** - The pointer pointing to the head node's values
1: char* - Pointer to the first character in the user's inputted string argument 
*/
void insert(NodePtr *head, char *str)
{
	NodePtr listItr = (*head), // Used to iterate through the nodes of the list
						temp;	// Used to allocate memory for linked list and to act as temporary node pointer

	if ((*head) == NULL) // Initial case for creating the first node, or the head of the list
	{
		temp = (NodePtr)malloc(sizeof(Node)); // Allocate memory
		temp->count = 1;			// Initialize the new node's count
		strcpy(temp->symbol, str);	// Initialize the new node's symbol
		temp->next = NULL;			// Point to the end of the list
		(*head) = temp;				// Initalize the head to NULL
		return;
	}

	// Not the end of the list AND the symbols don't match, then iterate through the list
	while (listItr->next != NULL && (strcmp(listItr->symbol, str) != 0))
	{
		listItr = listItr->next;
	}

	if (strcmp(listItr->symbol, str) == 0) // The iterator made it to a symbol that equals str and terminated the loop
	{
		listItr->count++;	// Increment the node's count
	}
	else // The while loop is terminated because iterator reached the end of the list, and the last node does not contain a symbol equals to str
	{
		temp = (NodePtr)malloc(sizeof(Node));		// Allocate memory
		strcpy(temp->symbol, str);		// Initialize the new node's symbol
		temp->count = 1;				// Initialize the new node's count
		temp->next = NULL;				// Point to the end of the list
		listItr->next = temp;			// Initalize the next node to NULL
	}
}

/*	
* Name: Delete List
* Parameter(s):
* 0: struct node** - The pointer pointing to the head node's value(s)
*/
void deleteList (NodePtr *head)
{
	NodePtr nextNode = (*head);		// Used to iterate through the nodes of the list
	NodePtr listItr; 			// Pointer pointing to the next node after the list iterator's current position	
	while (listItr != NULL)
	{
		listItr = nextNode->next;	// Point to the next node in the list
		free(nextNode);				// Delete the node the list iterator is currently pointing at!	
		nextNode = listItr;			// Go to next node
	}
	
	*head = NULL;	// Delete the head of the list
}

/*
Function: Sort List

Function Description:
Bubblesorts the linked list in decreasing value of each node's count.

Parameter(s):
0: struct node* - The head of the linked list
*/
void sortList(NodePtr head)
{
	int sorted,			// Used to check if list was sorted during the iteration 
		tempCnt;		// Temporarily stores the value of the smaller node count
	NodePtr listItr;	// Used to iterate through the nodes of the list
	char tempSym[5];		// Temporarily stores the string symbol of the smaller count node

	do
	{
		sorted = 0; // If sorted doesn’t change, then nothing was swapped so the list is sorted and the loop stops.
		listItr = head; // Start at the begining of the list each time

		while (listItr->next != NULL)	// Iterate through the list until the end
		{
			if (listItr->count < listItr->next->count)	// The current node's count is less than the next node's count
			{
				tempCnt = listItr->count;		 // Save count
				strcpy(tempSym, listItr->symbol); // Save symbol

				listItr->count = listItr->next->count;			// Swap the count of the 2nd node into the first
				strcpy(listItr->symbol, listItr->next->symbol); // Swap the symbol 2nd node into the first

				listItr->next->count = tempCnt;			// Store the count of the 1st node into the 2nd
				strcpy(listItr->next->symbol, tempSym); // Store the symbol of the 1st node into the 2nd

				sorted = 1; // Nodes were swapped. Keep sorting.         
			}
			listItr = listItr->next;	// Increment the iterator
		}
	} while (sorted); // Keep sorting until nothing was swapped
}

/*
* Name: insertFile
* Parameter(s):
* 0: struct - A struct of type flightRec
* 
* Description:
* Writes a flight record struct to a binary file.
*/
void insertFile(flyRec fRec)
{
	FILE *fp;
	flyRec f;

	// Create binary file
	// Open output file for reading and appending
	if ((fp = fopen(FILENAME,"a+"))== NULL)
  	{
    	fprintf(stderr, "Error: In function 'main': Failed to open the output file\n");
		exit(1);
  	}

	if (fwrite((const void *)&fRec, sizeof(fRec), 1, fp) != 1)
   	{
    	fprintf(stderr, "Error: In function 'insertFile': Failed to write record to file\n");
     	exit(1);
    }
	
	fclose(fp);
}

/*
* Name: printInfo
* Parameter(s):
* None
* 
* Description:
* Prints the info of all the flight records in the binary file.
*/
void printInfo()
{
	int i;
	FILE *fp;
	flyRec f;
	
	// Open output file for reading and appending
	if ((fp = fopen(FILENAME,"r"))== NULL)
  	{
    	fprintf(stderr, "Error: In function 'main': Failed to open the output file\n");
		exit(1);
  	}

	for(i = 0; i < (fileSize(FILENAME) / sizeof(f)); i++)
	{
		// Read data from file
		if (fread((void*)&f, sizeof(f), 1, fp) < 1)
		{
		  fprintf(stderr, "Error: In function 'printInfo': Failed to read record from file.\n");
		  return;
		}
		else
		{
			printf("Flight record #%d: %s%s %s %s %s %s %d %s %d\n", i+1, f.airCode, f.flyNum, f.orgCode, f.destCode, f.dayOfWeek, f.month, f.day, f.time, f.year);
		}
	}
	
	fclose(fp);
}

/* Name: inBound
 * Parameter(s):
 * 0: char* - string input from user, airport code
 * 
 * Description:
 * Prints number of inbound flights for a user supplied airport (d)
 * */
int inBound(char *airport){
    FILE *fp;
    flyRec f;
    int cnt=0, i; //initialize counter to zero
	
	// Open output file for reading and appending
	if ((fp = fopen(FILENAME,"r"))== NULL)
  	{
    	fprintf(stderr, "Error: In function 'main': Failed to open the output file\n");
		exit(1);
  	}
  	
  	for(i = 0; i < (fileSize(FILENAME) / sizeof(f)); i++)
	{
		// Read data from file
		if (fread((void*)&f, sizeof(f), 1, fp) < 1)
		{
		  fprintf(stderr, "Error: In function 'inBound': Failed to read record from file.\n");
		  exit(1);
		}
		else
		{
			if(strcmp(airport,f.destCode)==0) cnt++;
		}
	}
	return cnt;
}

/* Name: inBoundAll
 * Parameter(s):
 * 0: char* - string input from user, airport code
 * 
 * Description:
 * Prints number of inbound flights for a user supplied airport (e)
 * */
int inBoundAll(){
    FILE *fp;
    flyRec f;
    int i;
    //char airport[100][5];
    typedef struct inbnd{
        char airport[5];
        int cnt;
        struct inbnd *next;
    }inbnd_t, *inPtr;
	inPtr head=NULL, iter8r, temp; //create head of LL
    
    
    int repeat=0;
	// Open output file for reading and appending
	if ((fp = fopen(FILENAME,"r"))== NULL)
  	{
    	fprintf(stderr, "Error: In function 'main': Failed to open the output file\n");
		exit(1);
  	}
  	
  	//inbnd_t *iter8r=(*head);
    
  	for(i = 0; i < (fileSize(FILENAME) / sizeof(f)); i++)
	{
		iter8r=head;// set/reset iter8r to head
		//int repeat=0;
        // Read data from file
		if (fread((void*)&f, sizeof(f), 1, fp) < 1)
		{
		  fprintf(stderr, "Error: In function 'inBound': Failed to read record from file.\n");
		  exit(1);
		}
		else
		{
			/* Add all unique destination airports to their own
             * struct and keep a count of any repeaters as they 
             * are all added from the binary file.
             */
            if(iter8r==NULL){
                temp=malloc(sizeof(inbnd_t));
                strcpy(temp->airport,f.destCode);
                temp->cnt=inBound(temp->airport);
                temp->next=NULL;
                head=temp;
                //printf("HEAD\n\n");
                temp=NULL;
                continue;
            }
            //printf("FROM BIN: %s\t<->\t",f.destCode);
            do{
                //printf("IN LIST: %s\n",iter8r->airport);
                repeat=0;
                if(strcmp(iter8r->airport,f.destCode)==0){
                    //printf("FOUND\n");
                    repeat=1;
                    break;
                }
                if(iter8r->next!=NULL && !repeat){
                    //printf("NOT-FOUND\n");
                    iter8r=iter8r->next;
                }
                if(iter8r->next==NULL){
                    if(strcmp(iter8r->airport,f.destCode)==0){
                        repeat=1;
                        break;
                    }
                    //if(!repeat)printf("NOT-FOUND\n");
                }

            }while(iter8r->next!=NULL);
            
            if(!repeat){
                //printf("NEW: %s\n", f.destCode);
                temp=malloc(sizeof(inbnd_t));
                strcpy(temp->airport,f.destCode);
                temp->cnt=inBound(temp->airport);
                temp->next=NULL;
                iter8r->next=temp;
                
                //printf("%s %d\n",temp->airport, temp->cnt);
                temp=NULL;
            }
           
		}
		//printf("\n");
	}
	
	iter8r=head;
	while(iter8r->next!=NULL){
        printf("%s has %d inbound flight(s).\n",iter8r->airport,iter8r->cnt);
        iter8r=iter8r->next;
        if(iter8r->next==NULL)printf("%s has %d inbound flight(s).\n",iter8r->airport,iter8r->cnt);
        }
	return 0;
}

/*
* Name: printSortOrg
* Parameter(s):
* None
* 
* Description:
* Prints a sorted list of origin airports based on the number of outbound flights.
*/
void printSortOrg()
{
	int i;
	FILE *fp;
	flyRec f;
	NodePtr head = NULL; // Create empty list
	NodePtr listItr; // Used to iterate through the nodes of the list
	
	// Open output file for reading and appending
	if ((fp = fopen(FILENAME,"r"))== NULL)
  	{
    	fprintf(stderr, "Error: In function 'main': Failed to open the output file\n");
		exit(1);
  	}
	
	for(i = 0; i < (fileSize(FILENAME) / sizeof(f)); i++)
	{
		// Read data from file
		if (fread((void*)&f, sizeof(f), 1, fp) < 1)
		{
		  fprintf(stderr, "Error: In function 'printInfo': Failed to read record from file.\n");
		  return;
		}
		else
		{
			insert(&head, f.orgCode); // Call insert function
		}
	}
	
	sortList(head); // Sort by count
	listItr = head; // Used to iterate through the nodes of the list
	
	// Print sorted data
	if (listItr == NULL)
	{
		fprintf(stderr, "Error: No records to print. The list is empty.\n");
		fflush(stdout);
	}
	else
	{
		printf("Airport\tOutbound Flights\n");
		while (listItr != NULL)
		{
			printf("%s\t\t%d\n", listItr->symbol, listItr->count);
			fflush(stdout);
			listItr = listItr->next;
		}
	}
	
	deleteList (&head); // Delete the list
	
	fclose(fp);
}

/*
* Name: printOrgMorn
* Parameter(s):
* None
* 
* Description:
* Prints a list of origin airports that have at least 2 flights that have a departure time earlier than noon.
*/
void printOrgMorn()
{
	int i;
	FILE *fp;
	flyRec f;
	NodePtr head = NULL; // Create empty list
	NodePtr listItr; // Used to iterate through the nodes of the list
	
	// Open output file for reading and appending
	if ((fp = fopen(FILENAME,"r"))== NULL)
  	{
    	fprintf(stderr, "Error: In function 'main': Failed to open the output file\n");
		exit(1);
  	}
	
	for(i = 0; i < (fileSize(FILENAME) / sizeof(f)); i++)
	{
		// Read data from file
		if (fread((void*)&f, sizeof(f), 1, fp) < 1)
		{
		  fprintf(stderr, "Error: In function 'printInfo': Failed to read record from file.\n");
		  return;
		}
		
		// If the flight record time is greater than or equal to midnight(00:00) AND less than noon (12:00), then insert into list
		if ((strcmp("00:00", f.time) <= 0) && (strcmp("12:00", f.time) > 0)) 
		{
			insert(&head, f.orgCode); // Call insert function
		}
	}

	listItr = head;
	
	// Print data in the list
	if (listItr == NULL)
	{
		fprintf(stderr, "Error: No records to print. The list is empty.\n");
		fflush(stdout);
	}
	else
	{
		printf("Airports with departure time earlier than noon:\n");
		while (listItr != NULL)
		{
			if (listItr->count >= 2)
			{
				printf("\t\t%s\n", listItr->symbol);
				fflush(stdout);
			}
			listItr = listItr->next;
		}
	}
	
	deleteList (&head); // Delete the list
	
	fclose(fp);
}

/*
* Name: numForAirport
* Parameter(s):
* 0: Char* - A pointer to a char array containing the user input of the airport code.
* 
* Description:
* Counts and prints the number of airlines for a given airport.
*/
void numForAirport(char *ap)
{
    int i;
    FILE *fp;
    flyRec f;
    NodePtr head = NULL;
    NodePtr cur;
    int count = 0;

    if ((fp = fopen(FILENAME,"r"))== NULL)
    {
    	fprintf(stderr, "Error: In function 'flightFuncs': function 'numPerAirport' Failed to open the output file\n");
		exit(1);
  	}
    	for(i = 0; i < (fileSize(FILENAME) / sizeof(f)); i++)
        {
            if (fread((void*)&f, sizeof(f), 1, fp) < 1)// read from file
            {
              fprintf(stderr, "Error: In function 'numPerAirport': Failed to read record from file\n");
              return;
            }
            else
            {
                if((strcmp(f.orgCode,ap))==0||(strcmp(f.destCode,ap)==0))//compare given airport with origin and destinations
                {
                   insert(&head,f.airCode);//for all matches copy aircode into an array
                }
            }
        }

        cur=head;
		
        // Print # of airlines
        while(cur!= NULL)
        {
            count++;
            cur=cur->next;

        }
    printf("The number of airlines for %s is %d.\n", ap, count);

    count = 0;
}

/*
* Name: fileSize
* Parameter(s):
* 0: char* - A pointer to a file's name
* 
* Description:
* Computes the size of a file using the stat() function
*/
int fileSize(char *file)
{
	int fileSize = 0;  // Holds file size
	struct stat stats;  // stat struct

	if(stat(file, &stats) != 0)  // Get the stats of the file
	{
		fprintf(stderr, "Error: In function fileSize: Failed to retrieve file size.\n");
    	perror("Error");
	}
	else
	{
		fileSize = stats.st_size;  // Get the size of the file from the stat struct
	}
  
	return fileSize;  // Return the file size
}