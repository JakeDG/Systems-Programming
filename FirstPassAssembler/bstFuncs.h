/* 
* Developed by:
* Jacob Gidley, Recorder
* Daniel Leo, Team Leader
* Mary Slaven, Monitor
*
* This file contains prototypes for functions related to the machine opcode table
*/
void inOrdTrav(tNodePtr node);
tNodePtr treeInsert(tNodePtr node, tNodePtr dataNode);
int treeHeight(tNodePtr node);
tNodePtr opFormSearch(tNodePtr node, char *inst);