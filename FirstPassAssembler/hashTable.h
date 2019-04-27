/* 
* Developed by:
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Team Leader, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains the struct definition of a node in the hash table.
*/

 typedef struct hashNode
 {
    char symbol [11];	// Holds a symbol of an assembly program
    long LC_val;		// Holds the LC value of where the symbol was encountered in the assembly file
    struct hashNode *next; // Pointer to the next hashNode struct in a list
 } hNode, *hNodePtr;
