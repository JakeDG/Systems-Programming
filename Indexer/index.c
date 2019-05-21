/*
* Developed by:
* Mary Slaven, Team Leader
* Jacob Gidley, Recorder
* Daniel Leo, Monitor
*
*This file contains the indexer function
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "miscFuncs.h"
#include "lists.h"
#include "externs.h"

#define LINE_SIZE 255

/*
* Function Description:
* Tokenizes the incoming file and calls the corresponding list functions.
*
* Parameter(s):
* 0: char* - Pointer to the filename string
*/
void indexer(char *filename)
{
	char buffer[LINE_SIZE];
	char delims[] = " \t\n\v\b\f\r\"\\~`!@#$%^&*()_+-=[]{}|;':,./<>?";
	char *token;
	FILE *fp;
	
	if((fp = fopen(filename, "r")) == NULL)
    {
		fprintf(stderr, "Error: In function index: Failed to open file.\n");
        perror("Error");
  	    exit(1);
    } 
	
	while (fgets(buffer, LINE_SIZE, fp) != NULL)
	{
		// Get the first token 
		token = strtok(buffer, delims);
		lowerCase(token);
		insert(&tokListHead, token, filename);
		
		// Walk through other tokens
		while( token != NULL ) 
		{
			token = strtok(NULL, delims);
			if (token != NULL)
			{
				lowerCase(token);
				insert(&tokListHead, token, filename);
			}
		}
	}
	
	sortList(tokListHead); // Sort the list

	fclose(fp);
}
