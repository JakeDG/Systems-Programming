/*
*Developed by:
* Mary Slaven, Team Leader
* Jacob Gidley, Recorder
* Daniel Leo, Monitor
*
* This file contains the structs needed for the 2d linked list.
*/

#include "constants.h"

// Delare linked list using typedef
typedef struct countNode
{
	char filename[FILE_NAMESIZE];	// Stores filename
	int count;						// Stores the count of the token in the file
	struct countNode *next;		// Node pointer to next node in the list
	
} countNode_t, *countNodePtr;		// countNode_t = struct countNode, *countNodePtr = struct countNode*

// Delare linked list using typedef
typedef struct tokNode
{
	char token[TOKEN_SIZE];		// Stores symbol
	struct tokNode *next;		// Node pointer to next node in the 1st dimension
	countNodePtr child;
	
} tokNode_t, *tokNodePtr;	// tokNode_t = struct tokNode_t, *tokNodePtr = struct tokNodePtr*