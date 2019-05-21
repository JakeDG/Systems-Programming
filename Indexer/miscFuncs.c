/*
* Developed by:
* Mary Slaven, Team Leader
* Jacob Gidley, Recorder
* Daniel Leo, Monitor
*
* This file contains functions to check if a file is a directory
* and to change tokens to all lower case
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*
* Function Description:
* Determines if a file is a directory
*
* Parameter(s):
* 0: char* - Pointer to the name string
*
*/
int isDir(char *name)
{
	struct stat stats;  // stat struct
	
	// Check for errors
	if (stat(name, &stats) != 0)  // Get the stats of the file
	{
		fprintf(stderr, "Error: In function isDir: Failed to retrieve file stats.\n");
    	perror("Error");
		return 0;
	}
	
	// Check if file is a directory
	if (S_ISDIR(stats.st_mode))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
* Function Description:
* Converts string to all lower case
*
* Parameter(s):
* 0: char* - Pointer to the string
*
*/
char* lowerCase(char *str)
{
    int i; // loop counter

    for(i = 0; str[i] != '\0'; i++)
    {
		str[i]=(tolower(str[i])); // Change char to lower case if possible
    }
	
    return  str;
}