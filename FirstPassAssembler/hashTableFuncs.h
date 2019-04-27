/* 
* Developed by:
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Team Leader, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains the prototyes for the hash table functions
*/
void listInsert(hNodePtr *head, char *symbol, long lc);
void insertTable(hNodePtr table[], int size, char *fileLine, long lc);
void printTable(hNodePtr table[], int size);
long getLC(hNodePtr table[], char *symbol, int size);