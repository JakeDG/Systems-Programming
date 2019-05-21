/* 
* Developed by:
* Jacob Gidley, Recorder
* Daniel Leo, Team Leader
* Mary Slaven, Monitor
*
* This file contains the prototyes for the hash table functions
*/
void listInsert(hNodePtr *head, char *symbol, long lc);
void insertTable(hNodePtr table[], int size, char *fileLine, long lc);
void printTable(hNodePtr table[], int size);
long getLC(hNodePtr table[], char *symbol, int size);