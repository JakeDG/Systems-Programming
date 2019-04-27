/* 
* Developed by:
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Team Leader, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* Description:
* This program will generate a machine opcode table, stored in a binary search tree, and a symbol table, stored in a hash table.
* It will construct the opcode table by inserting the instructions into the BST in the order they appear in the opcode file provide 
* in the commmand line. It will also construct the symbol table by inserting the symbols into the hash table in the order they appear 
* in a label field in the program file provided in the command line.
* The program will print to an in-order traversal of the BST, in which, for each node in the tree, only the instruction name will 
* be written stdout. Also, height of the BST will be printed as well.
* The program will also output the contents of the hash table, with one array element per line. Each array element's linked list will have 
* each of its node's symbol and LC value printed to stdout.
*
* Assumptions: 
* - It will be assumed that a file's line size will be at most 255 characters in length.
* - It is also assumed that there will be no blank lines or whitespace before characters in a file as stated in the assignment document.
*
* Command line input example: firstpass instructionset programfile hashtablesize
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bst.h" 			// BST struct definition
#include "bstFuncs.h" 		// BST functions
#include "hashTable.h" 		// Hash table struct definition
#include "hashTableFuncs.h" // Hash table functions

#define LINESIZE 255 // Size of a file line

int main(int argc, char *argv[])
{
	char fileLine[LINESIZE]; // Holds a line of text from a file
	FILE *opfp, 			// File pointer for the opcode file
		*pfp; 				// File pointer for the assembly program file
	tNode treeNode; 		// Structure to hold values from the opcode file
	tNodePtr tPtr = NULL; 	// Pointer to a treeNode struct
	int i; 					// Counter for for loop that NULLs every pointer in the array of pointers
	int j = 0; 				// Boolean used to check if it's the first time inserting a node into the BST
	long lc = 0; 			// Counter used to store what line of the program file that is currently being read
	int size; 				// Holds the size of the hash table
	char *token; 			// Holds a token in a file line
	
	// Check for if wrong amount of arguments provided
	if (argc != 4)
	{
		fprintf(stderr, "Error: In function 'main': Wrong amount of arguments provided.\n");
		exit(1);
	}
	
	size = atoi(argv[3]); 	// Initialize size to the hashtable size provided
	hNodePtr hashTable[size]; 	// An array of hashNode pointers to the heads of linked lists that each hold a specific symbol from the program file.
	
	// Initialize each pointer in the array to NULL
	for (i = 0; i < size; i++)
	{
		hashTable[i] = NULL;
	}
	
	// Open the instruction file 
	if((opfp = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "Error: In function 'main': Failed to open opcode file.\n");
		perror("Error");
		exit(1);
	}
	
	// Open the assembly program file
	if ((pfp = fopen(argv[2], "r")) == NULL)
	{
		fprintf(stderr, "Error: In function 'main': Failed to open program file.\n");
		perror("Error");
		exit(1);
	}
	
	// Get lines from the instruction file
	while (fscanf(opfp,"%s %d %d", treeNode.inst, &treeNode.opcode, &treeNode.format) == 3)
	{
		if (j == 0) // First node in the tree
		{
			tPtr = treeInsert(tPtr, &treeNode);
			j++; // Set flag
		}
		else
		{
			treeInsert(tPtr, &treeNode);
		}
	}
	
	// Print inoder traversal of the BST
	printf("In-Order Traversal of BST: ");
	inOrdTrav(tPtr);
	printf("\b\b \n"); // Get rid of last comma
	
	// Print the height of the tree
	printf("Height of the BST: %d\n\n", treeHeight(tPtr));
	
	/*
	* Read lines from assembly program file.
	* For each symbol encountered in the label field, insert 1 node into the hash table. The node will contain the symbol,
	* and its LC value.
	*/
	while (fgets(fileLine, LINESIZE, pfp) != NULL)
	{
		if (fileLine[0] == '\t') // Check if the 1st character on a line is a token
		{
			// Get instruction following the '\t'
			token = strtok(fileLine, " \t\r");
			
			// Get the amount of bytes the instruction is and increment the LC value based on what is returned
			lc += (long)((opFormSearch(tPtr, token))->format);
		}
		else // There is a symbol in the label field
		{
			// Get the symbol and insert it into the hash table
			token = strtok(fileLine, " ");
			insertTable(hashTable, size, token, lc);
			
			// Check to see if the next token is a directive (word, resw, resb)
			token = strtok(NULL , " ");
			
			if (strcmp(token, "resw") == 0)
			{
				token = strtok(NULL , " ");
				lc += 3 * (atoi(token)); // Increment LC value to 3 * the number of words reserved because the size of a word is 3 bytes
			}
			else if (strcmp(token, "resb") == 0)
			{
				token = strtok(NULL , " ");
				lc += atoi(token); // Increment LC value to the number of bytes provided as an argument to resb directive
			}
			else if (strcmp(token, "word") == 0)
			{
				lc += 3; // Increment LC value by the size of a word (3 bytes)
			}
			else // The next token is an instruction
			{
				// Get the amount of bytes the instruction is and increment the LC value by the value retrun from the BST
				lc += (long)((opFormSearch(tPtr, token))->format);
			}
		}
	}
	
	printTable(hashTable, size);
    return 0;
}