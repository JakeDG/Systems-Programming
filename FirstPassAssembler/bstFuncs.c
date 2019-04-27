/* 
* Developed by:
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Team Leader, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains the functions related to the creation of the machine opcode table a.k.a. the binary search tree
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bst.h"

/*
* Name: inOrdTrav
* Parameter(s):
* 0: struct* - A pointer to a treeNode struct that is/should be the head of a binary search tree.
*
* Description:
* Prints an in-order traversal of the binary search tree to stdout.
*/
void inOrdTrav(tNodePtr node)
{
    if (node != NULL)
    {
        inOrdTrav(node->left); 		// Traverse the left branch of the node
        printf("%s, ", node->inst); // Print only the instruction name of the node
        inOrdTrav(node->right);		// Traverse the right branch of the node
    }
}
 
/*
* Name: treeInsert
* Parameter(s):
* 0: struct* - A pointer to a treeNode struct that is/should be the head of a binary search tree.
* 1: struct* - A pointer to a treeNode struct that includes all of the data needed to store into a node in the tree.
*
* Description:
* Inserts a new node into the BST organized by alphabetical value and returns a pointer to the head of the tree.
*/
tNodePtr treeInsert(tNodePtr node, tNodePtr dataNode)
{
	tNodePtr temp; // Used to create a temporary node in the tree
	
    // If the tree is empty return a newly created node
    if (node == NULL)
	{
		if ((temp = (tNodePtr)malloc(sizeof(tNode))) == NULL)
		{
			fprintf(stderr, "Error: In function 'treeInsert': Memory allocation failed.\n");
			perror("Error");
		    exit(1);
		}
		strcpy(temp->inst, dataNode->inst);
		temp->opcode = dataNode->opcode;
		temp->format = dataNode->format;
		temp->left = NULL;
		temp->right = NULL;
		return temp;
	}
 
    // If tree is not empty, then traverse the tree
    if (strcmp(node->inst, dataNode->inst) > 0) // If the instruction string is less than the node's string, then branch to the left.
	{
		node->left = treeInsert(node->left, dataNode);
	}
    else if (strcmp(node->inst, dataNode->inst) < 0) // If the instruction string is greater than the node's string, then branch to the right.
	{
        node->right = treeInsert(node->right, dataNode);   
	}
	
    // Return the head of the tree
    return node;
}

/*
* Name: treeHeight
* Parameter(s):
* 0: struct* - A pointer to a treeNode struct that is/should be the head of a binary search tree.
*
* Description:
* Determine the height of a tree, which is the maximum length of a path from the root to the deepest node of the tree.
* Returns the integer value of the height of the tree.
*/
int treeHeight(tNodePtr node) 
{
   int lSubHeight; // Holds the left subtree's depth
   int rSubHeight; // Holds the right subtree's depth
   
   // If the tree is empty or has one node, then return 0.
   if (node == NULL)
   {
	   return 0;
   }
   else
   {
	   // Get both subtrees depths recursively
       lSubHeight = treeHeight(node->left);
       rSubHeight = treeHeight(node->right);
 
       // Return the larger of the two subtrees
       if (lSubHeight > rSubHeight)
	   {
		   return(lSubHeight + 1);
	   }
       else
	   {
		   return(rSubHeight + 1);
	   }
   }
} 

/*
* Name: opFormSearch
* Parameter(s):
* 0: struct* - A pointer to a treeNode struct that is/should be the head of a binary search tree.
* 1: char* - A char pointer to symbol to be searched in the tree.
*
* Description:
* Retrieves the instruction format given the instruction name.
*/
tNodePtr opFormSearch(tNodePtr node, char *inst)
{
    // Head of tree is null or holds the argument instruction
    if (node == NULL || (strcmp(node->inst, inst) == 0))
	{
		return node;
	}
    
    if (strcmp(node->inst, inst) < 0) // If the instruction string is greater than the node's string, then branch to the right.
	{
		return opFormSearch(node->right, inst);
	}
    
    return opFormSearch(node->left, inst);
}