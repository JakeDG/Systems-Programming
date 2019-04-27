/*
* Developed by:
* Mary Slaven, Team Leader, mslaven@albany.edu
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Monitor, dleo2@albany.edu

* This file contains the functions for creating, sorting and printing the linked list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lists.h"
#include "externs.h"
#include "constants.h"

/*
* Function Description:
* Inserts a new node into the linked list. Each node of this list will also have a linked list associated with it that holds the filename provided
* and the amount of times that token occured in the file. If there exists a node with the same token as the one provided and the a child node's
* filename matches, then the child node with will have it's count increased by 1.
*
* Parameter(s):
* 0: struct node** - The pointer pointing to the head node's values
* 1: char* - Pointer to the token string
* 2: char* - Pointer to the filename string
*/
void insert(tokNodePtr *head, char *tok, char *fname)
{
	tokNodePtr curParent = (*head), // Used to iterate through the nodes of the list
							tempParent;	// Used to allocate memory for linked list and to act as temporary node pointer
						
	countNodePtr curChild, // Used to iterate through the nodes of the list
				tempChild; // Used to allocate memory for linked list and to act as temporary node pointer

	if ((*head) == NULL) // Initial case for creating the first node, or the head of the list
	{
		
		if((tempParent = (tokNodePtr)malloc(sizeof(tokNode_t))) == NULL) // Allocate memory and return pointer to new struct
		{
		    fprintf(stderr, "Error: In function insert: Memory allocation failed.\n");
			perror("Error");
		    exit(1);
		}
		if((tempChild = (countNodePtr)malloc(sizeof(countNode_t))) == NULL)// Allocate memory and return pointer to new struct
		{
		    fprintf(stderr, "Error: In function insert: Memory allocation failed.\n");
			perror("Error");
		    exit(1);
		}
		
		tempParent->child = tempChild; // Child now points to a newly created cuntNode struct
		
		strcpy(tempParent->token, tok);	// Initialize the new parent node's token
		
		strcpy(tempParent->child->filename, fname);	// Initialize the new child node's filename
		tempParent->child->count = 1;	// Initialize the new child node's count
		
		tempParent->next = NULL;	// Point to the end of the parent list
		tempChild->next = NULL;		// Point to the end of the child list
		
		(*head) = tempParent;		// Initalize the head to NULL
		
		return;
	}
	
	// Not the end of the list AND the tokens don't match, then iterate through the list
	while (curParent->next != NULL && (strcmp(curParent->token, tok) != 0))
	{
		curParent = curParent->next;
	}

	if (strcmp(curParent->token, tok) == 0) // The iterator made it to a matching token and terminated the loop
	{
		curChild = curParent->child;	// Get the first child node in the list of child nodes of the parent node (Kind of like the head!!!)
		
		// Loop through the child list, checking to see if the filenames match
		while (curChild->next != NULL && (strcmp(curChild->filename, fname) != 0))
		{
			curChild = curChild->next;
		}
		
		if (strcmp(curChild->filename, fname) == 0)	// The iterator made it to a matching filename and terminated the loop
		{
			curChild->count++;	// Increment the child node's count
		}
		else
		{
			tempChild = (countNodePtr)malloc(sizeof(countNode_t)); // Allocate memory and return pointer to new struct
			strcpy(tempChild->filename, fname);	// Initialize the new child node's filename
			tempChild->count = 1;	// Initialize the new child node's count
			tempChild->next = NULL;	// Point to the end of the child list
			curChild->next = tempChild;
		}
		
	}
	else // The while loop is terminated because iterator reached the end of the list, and the last node does not contain a matching token.
	{		
		if((tempParent = (tokNodePtr)malloc(sizeof(tokNode_t))) == NULL) // Allocate memory and return pointer to new struct
		{
		    fprintf(stderr, "Error: In function insert: Memory allocation failed.\n");
			perror("Error");
		    exit(1);
		}
		if((tempChild = (countNodePtr)malloc(sizeof(countNode_t))) == NULL)// Allocate memory and return pointer to new struct
		{
		    fprintf(stderr, "Error: In function insert: Memory allocation failed.\n");
			perror("Error");
		    exit(1);
		}
		
		tempParent->child = tempChild; 	// Child now points to a newly created cuntNode struct
		
		strcpy(tempParent->token, tok);	// Initialize the new parent node's token
		
		strcpy(tempParent->child->filename, fname); // Initialize the new child node's filename
		tempParent->child->count = 1;	// Initialize the new child node's count
	
		tempParent->next = NULL;	// Point to the end of the parent list
		tempChild->next = NULL;		// Point to the end of the child list
		
		curParent->next = tempParent;	// Initalize the next node to NULL
	}
}

/* 
* Function Description:
* Bubblesorts a linked list by alphabetical order, while at the same time, it bubblesorts each linked list associated with the each node 
* in the list by desending order according to the count.

* Parameter(s):
* 0: struct node* - The head of the linked list
*/
void sortList(tokNodePtr head)
{
	int alphaSorted,	// Used to check if parent list was sorted during the iteration 
  		numSorted,		// Used to check if child list was sorted during the iteration 
		tempCnt;		// Temporarily stores the value of the smaller child node count
	tokNodePtr curParent;	// Used to iterate through the nodes of the list of tokens
	countNodePtr curChild,	// Used to iterate through the nodes of the list of file names
				tempChild;	// Temporarily stores the parent node's child pointer
	char tempTok[TOKEN_SIZE];	// Temporarily stores the string symbol of the smaller count node
	char tempfName[FILE_NAMESIZE];	// Temporarily stores the file name of the 

	do
	{
		alphaSorted = 0; // If sorted does not change, then nothing was swapped so the list is sorted and the loop stops.
		curParent = head; // Start at the begining of the list each time

		while (curParent->next != NULL)	// Iterate through the list until the end
		{
			if (strcmp(curParent->token, curParent->next->token) > 0)	// The current node's token is alphabetically less than the next node's token
			{
				//tempCnt = listItr->count;		 // Save count
				strcpy(tempTok, curParent->token); // Save token
				tempChild = curParent->child;
        
				//curParent->count = listItr->next->count;			// Swap the count of the 2nd node into the first
				strcpy(curParent->token, curParent->next->token); // Swap the token from the 2nd node into the first
				curParent->child = curParent->next->child;
        
				//listItr->next->count = tempCnt;			// Store the count of the 1st node into the 2nd
				strcpy(curParent->next->token, tempTok); // Store the token of the 1st node into the 2nd
				curParent->next->child = tempChild;
				
				// Sort the parent node's child list
				curChild = curParent->child; // Store the current parent's child pointer
				do
				{
				  numSorted = 0;	// If sorted does not change, then nothing was swapped so the list is sorted and the loop stops.
				  while (curChild->next != NULL)
				  {
					if (curChild->count < curChild->next->count)
					{
					  tempCnt = curChild->count;	// Save count
					  strcpy(tempfName, curChild->filename); // Save filename

					  curChild->count = curChild->next->count;	// Swap the count of the 2nd node into the first
					  strcpy(curChild->filename, curChild->next->filename); // Swap the filename 2nd node into the first

					  curChild->next->count = tempCnt;	 // Store the count of the 1st node into the 2nd
					  strcpy(curChild->next->filename, tempfName); // Store the filename of the 1st node into the 2nd

					  numSorted = 1; // Nodes were swapped. Keep sorting. 
					}
					curChild = curChild->next;	// Increment the iterator
				  }
				} while (numSorted);
				
				alphaSorted = 1; // Parent nodes were swapped. Keep sorting.         
			}
			curParent = curParent->next;	// Increment the iterator
		}
	} while (alphaSorted); // Keep sorting until nothing was swapped
}

/* 
* Function Description:
* Writes the linked list of the index to a file that was supplied. Creates file if it does not exist.
*
* Parameter(s):
* 0: struct node* - The head of the linked list.
* 1: char* - The name of the filename to write to.
*/
void writeList(tokNodePtr head, char * fileName)
{
  FILE * fp;	// Pointer to output file
  tokNodePtr curParent = head; // Used to iterate through the nodes of the list
	countNodePtr curChild;	// Pointer to child node of parent node
	
	// Open output file
	if((fp = fopen(fileName, "w+")) == NULL)
	{
	   fprintf(stderr, "Error: In function writeList: Failed to open %s.\n", fileName);
	   perror("Error");
	   exit(1);
	}
	
	// Write to output file
	while (curParent != NULL)
	{
		fprintf(fp,"<list> %s\n", curParent->token);
		
		curChild = curParent->child;
		
		while (curChild != NULL)
		{
			fprintf(fp,"%s %d ", curChild->filename, curChild->count); 
        
			curChild = curChild->next;
		}
		fprintf(fp,"\n</list>\n");
		
		curParent = curParent->next;
	}
	
	fclose(fp);
}