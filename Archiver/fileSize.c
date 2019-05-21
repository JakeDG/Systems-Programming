/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Recorder
* Mary Slaven, Monitor

* This file contains the fileSize function.
*/

#include <stdio.h>
#include <sys/stat.h>

/*
* Name: fileSize
* Parameter(s):
* 0: char* - A pointer to a file's name
* 
* Description:
* Computes the size of a file using the stat() function
*/
int fileSize(char *file)
{
	int fileSize = 0;  // Holds file size
	struct stat stats;  // stat struct

	if(stat(file, &stats) != 0)  // Get the stats of the file
	{
		fprintf(stderr, "Error: In function fileSize: Failed to retrieve file size.\n");
    	perror("Error");
	}
	else
	{
		fileSize = stats.st_size;  // Get the size of the file from the stat struct
	}
  
	return fileSize;  // Return the file size
}