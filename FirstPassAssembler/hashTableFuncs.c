/* 
* Developed by:
* Jacob Gidley, Recorder
* Daniel Leo, Team Leader
* Mary Slaven, Monitor
*
* This file contains the hash table functions related to the creation of the symbol table
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hashTable.h"
#include "hashFunction.h"

/*
* Name: listInsert
* Parameter(s):
* 0: struct* - A pointer to a hashNode struct that is/should be the head of a linked list
* 1: char* - A char pointer a symbol encountered in an assembly file
* 2: long - The LC value of the symbol encountered in an assembly file
*
* Description:
* Inserts the symbol and its LC value into a node into a linked list in an array of linked lists
*/
void listInsert(hNodePtr *head, char *symbol, long lc)
{
	hNodePtr listItr = (*head); // Used to iterate through the nodes of the list
	hNodePtr temp; // Used to create a temporary struct in the linked list
	
	// If the head is empty then create the list
	if ((*head) == NULL)
	{	
		if ((temp = (hNodePtr)malloc(sizeof(hNode))) == NULL) // Allocate memory and check for errors
		{
			fprintf(stderr, "Error: In function 'listInsert': Memory allocation failed.\n");
			perror("Error");
		    exit(1);
		}
		
		strcpy(temp->symbol, symbol); // Initialize the new node's symbol
		temp->LC_val = lc;	// Initialize the new node's LC value
		temp->next = NULL;	// Point to the end of the list
		(*head) = temp;		// Initalize the head to NULL
		return;
	} 
	
	// Iterate through the list until the end
	while (listItr->next != NULL)
	{
		listItr = listItr->next;
	}
	
	// End of list reached, create new node.
	if ((temp = (hNodePtr)malloc(sizeof(hNode))) == NULL) // Allocate memory and check for errors
	{
		fprintf(stderr, "Error: In function 'listInsert': Memory allocation failed.\n");
		perror("Error");
		exit(1);
	}
	
	strcpy(temp->symbol, symbol); // Initialize the new node's symbol
	temp->LC_val = lc; // Initialize the new node's LC value
	temp->next = NULL; // Point to the end of the list
	listItr->next = temp; // Initalize the next node to temp
}

/*
* Name: insertTable
* Parameter(s):
* 0: struct hashNode* array[] - A hash table array of struct hashNode pointers that each are the head of a linked list
* 1: int - The size of the hash table array
* 2: char* - A char pointer a symbol string encountered in an assembly file
* 3: long - The LC value of the symbol encountered in an assembly file
*
* Description:
* Inserts a node into a hash table at the index specified by the value returned by the hash function
*/
void insertTable(hNodePtr table[], int size, char *symbol, long lc)
{
	int index; // Holds the index of a hash table array returned by the hash fucntion
	
	index = hashFunction(symbol, size);		// Get the index where the symbol should be stored in the array
	listInsert(&table[index], symbol, lc);	// Insert the symbol and its LC value into the hash table
}

/*
* Name: printTable
* Parameter(s):
* 0: struct hashNode* array[] - A hash table array of struct hashNode pointers that each are the head of a linked list
* 1: int - The size of the hash table array
*
* Description:
* Prints the contents of the hash table, with one array element per line. Each node will have it's symbol and LC value displayed.
*/
void printTable(hNodePtr table[], int size)
{
	int i; // loop counter
	hNodePtr listItr; // Used to iterate through the nodes of a linked list
	
	printf("Hash Table: \n");
    for(i = 0; i < size; i++)
    {
        if(table[i] != NULL)
        {
			listItr = table[i];
			printf("%d: ", i);
            while (listItr != NULL)
			{
				printf("%s %d → ", listItr->symbol, listItr->LC_val); // Print the Symbol and it's LC value
				listItr = listItr->next;
			}
			printf("NULL\n");
        }
    }
}

/*
* Name: getLC
* Parameter(s):
* 0: struct hashNode* array[] - A hash table array of struct hashNode pointers that each are the head of a linked list
* 1: int - The size of the hash table array
* 2: char* - A char pointer a symbol string
*
* Description:
* Retrieves the LC value for a given symbol in the hash table. Returns a long value of the LC value.
*/
long getLC(hNodePtr table[], int size, char *symbol)
{
	int index; // Holds the index that is returned by the hash function
	
	index = hashFunction(symbol, size); // Find where the symbol is in the table
	
	// If symbol exists, then return the head nodes LC value
	if (table[index] != NULL)
	{
		return table[index]->LC_val; // Retrun the LC value of the head node
	}
	else
	{
		fprintf(stderr, "Error: symbol does not exist in the table.\n");
		return 0;
	}
}