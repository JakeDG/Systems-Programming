/* 
* Developed by:
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Team Leader, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains prototypes for functions related to the machine opcode table
*/
void inOrdTrav(tNodePtr node);
tNodePtr treeInsert(tNodePtr node, tNodePtr dataNode);
int treeHeight(tNodePtr node);
tNodePtr opFormSearch(tNodePtr node, char *inst);