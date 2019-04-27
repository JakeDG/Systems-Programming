/* 
* Developed by:
* Jacob Gidley, Team Leader, jgidley@albany.edu
* Daniel Leo, Recorder, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* This file contains all prototypes
*/

#include <stdio.h>
#define FILENAME "flights.bin" // Name of flight record file

// A flight record struct
typedef struct flightRec
{
	char airCode[5];
	char flyNum[5];
	char orgCode[5];
	char destCode[5];
	char dayOfWeek[5];
	char month[5];
	int day;
	char time[6];
	int year;

} flyRec, *flyRecPtr;

// Function Prototypes
void printInfo();
int countAirlines(char *file);
void insertFile(flyRec fRec);
int fileSize(char *file);
void printSortOrg();
void numForAirport(char * ap);
