/* 
* Developed by:
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Team Leader, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains the struct definition of a node in the binary search tree.
*/

typedef struct treeNode
{
    char inst[6]; 	// Holds an instruction name
	int opcode;		// Holds the opcode for that instruction
	int format;		// Holds the size of the instruction in bytes
    struct treeNode *left; // A pointer to the left branch of a node in the tree
	struct treeNode *right; // A pointer to the right branch of a node in the tree
} tNode, *tNodePtr;