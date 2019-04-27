/* 
* Developed by:
* Jacob Gidley, Team Leader, jgidley@albany.edu
* Daniel Leo, Monitor, dleo2@albany.edu
* Mary Slaven, Recorder, mslaven@albany.edu
*
* This file contains the functions related to the commands of the shell.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
* Name: quit
* Parameter(s):
* None
*
* Description:
* Exits the shell with a goodbye message.
*
* Returns:
* Nothing
*/
void quit()
{
	printf("Goodbye!\n");
	exit(1);
}

/*
* Name: wd
* Parameter(s):
* None
*
* Description:
* Prints the shell's current working directory.
*
* Returns:
* Nothing
*/
void wd()
{	
	long size;
	char *buf;
	char *cwd;
	
	size = pathconf(".", _PC_PATH_MAX); // Get maximum length of a relative pathname for the current working diectory.
	
	if ((buf = (char*)malloc((size_t)size)) != NULL) // Allocate memory and check for errors
	{
		if ((cwd = getcwd(buf, (size_t)size)) != NULL) // Check if getcwd fails
		{
			printf("CWD: %s\n", cwd);
			
			// Free memory
			free(buf); 
			free(cwd);
		}
		else
		{
			free(buf); // Free memory
			fprintf(stderr, "Error: In function 'wd': Call to getcwd failed.\n");
			perror("Error");
		}
	}
	else
	{
		fprintf(stderr, "Error: In function 'wd': Memory allocation failed.\n");
		perror("Error");
		exit(1);
	}	
}

/*
* Name: cwd
* Parameter(s):
* 0: char* - A char* to a pathname to change the working directory to
*
* Description:
* Changes the shell's current working directory.
*
* Returns:
* Nothing
*/
void chwd(char *arg)
{
	if (chdir(arg) != 0) // Check for errors
	{
		fprintf(stderr, "Error: In function 'chwd': Could not open the directory.\n");
		perror("Error");
	}
}