/* 
* Developed by:
* Jacob Gidley, Team Leader, jgidley@albany.edu
* Daniel Leo, Monitor, dleo2@albany.edu
* Mary Slaven, Recorder, mslaven@albany.edu
*
* This file contains the functions related to the input read into the shell.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define LINESIZE 256 // Max size of input string
#define CMDS 20 // Max amount of commands

/*
* Name: getLine
* Parameter(s):
* 0: FILE* - A file pointer to a file stream
*
* Description:
* Reads a line of input from a file stream
*
* Returns:
* char* - A pointer to a line of input or NULL if EOF or '\n' is detected 
*/
char* getLine(FILE *stream)
{
	char* line; // Holds a line of input
	int lineSize = LINESIZE;
	int i = 0;
	int ch;
	
	if ((line = (char*)malloc(LINESIZE * sizeof(char))) == NULL) // Allocate memory and check for errors
	{
		fprintf(stderr, "Error: In function 'getLine': Memory allocation failed.\n");
		perror("Error");
		exit(1);
	}
	
	while (1)
	{
		ch = fgetc(stream); // Read input from the stream
		
		if (ch == '\n' || ch == EOF)
		{
			line[i] = NULL; // Insert null character at end of string
			printf("line: %s\n", line);
			return line;
		}
		else
		{
			line[i] = ch; // Insert the next char in line
		}
		
		i++;
		
		// Check if line size was exceeded and reallocate if neeeded
		if (i >= lineSize)
		{
			lineSize += 1;
			
			if ((line = (char*)realloc(line, lineSize)) == NULL) // Allocate memory and check for errors
			{
				fprintf(stderr, "Error: In function 'getLine': Memory re-allocation failed.\n");
				perror("Error");
				exit(1);
			}
		}
	}
}

/*
* Name: parseLine
* Parameter(s):
* 0: char* - A string of input
*
* Description:
* Parses the string parameter and stores its tokens into a 2D array.
*
* Returns:
* char** - A 2D array that contains the passed in line's tokens.
*/
char** parseLine(char *line)
{
	int i = 0; // Element of tokens array
	int tokenAmt = CMDS;
	char delims[] = " \n\t\r"; // Token delims
	char **tokens; // Stores tokens from the line of input
	char *token; // Holds a token from the line of input
	
	if ((tokens = malloc(CMDS * sizeof(char*))) == NULL) // Allocate memory and check for errors
	{
		fprintf(stderr, "Error: In function 'parseLine': Memory allocation failed.\n");
		perror("Error");
		exit(1);
	}
	
	token = strtok(line, delims);
	while (token != NULL)
	{
		tokens[i] = token;	// Put the tokens into there own array
		i++;
		
		if (i >= tokenAmt)
		{
			tokenAmt += 1;
			if ((tokens = realloc(tokens, tokenAmt * sizeof(char*))) == NULL) // Allocate memory and check for errors
			{
				fprintf(stderr, "Error: In function 'parseLine': Memory allocation failed.\n");
				perror("Error");
				exit(1);
			}
		}
		
		token = strtok(NULL, delims); // Get next token
	}

	tokens[i] = NULL; // Assign the last element in the array to NULL
	
	return tokens;
}

/*
* Name: redir
* Parameter(s):
* 0: int - File descriptor 1
* 1: int - File descriptor 2

* Description:
* Redirects the output of the program to file descriptor 1 by using dup2 and closes
* it after the call to dup2 because it is unnecessary.
*
* Returns:
* Nothing
*/
void redir(int fd1, int fd2)
{	
	dup2(fd1, fd2);   // stdout
	close(fd1); // Descriptor not needed anymore
}

/*
* Name: parseLine
* Parameter(s):
* 0: char** - A 2D array of strings
*
* Description:
* Removes the file redirection arguments '>' and the file name that comes after it.
* Creates a new 2D array 
*
* Returns:
* char** - A 2D array that has had the file redirection arguments removed
*/
char** reParseLine(char** cmds)
{
	int i = 0;
	int cmdAmt = CMDS;
	char** newCmds;
	char* str;

	if ((newCmds = (char**)malloc(CMDS * sizeof(char*))) == NULL) // Allocate memory and check for errors
	{
		fprintf(stderr, "Error: In function 'getOutFile': Memory allocation failed.\n");
		perror("Error");
		exit(1);
	}

	for(i = 0; cmds[i] != NULL; i++)
	{
		if (strcmp(cmds[i], ">") == 0)
		{
			i++; // skips over file name string that comes next
		}
		else
		{
			if (i >= cmdAmt)
			{
				cmdAmt += 1;
				if ((newCmds = realloc(newCmds, cmdAmt * sizeof(char*))) == NULL) // Allocate memory and check for errors
				{
					fprintf(stderr, "Error: In function 'reParseLine': Memory re-allocation failed.\n");
					perror("Error");
					exit(1);
				}
			}
			
			str = cmds[i];
			newCmds[i] = str;
		}
	}

	free(cmds);
	newCmds[i] = NULL;

	return newCmds;
}

/*
* Name: exec
* Parameter(s):
* 0: char** - A 2D char array of argument strings
*
* Description:
* Forks and executes the process that is indicated by the argmuments in the newly created child process
* while the parent waits for the process to finish.
*
* Returns:
* int - 0
*/
int exec(char **cmds)
{
	pid_t child;
	pid_t c;
	int cstatus;
	
	if ((child = fork()) == 0)
	{
		// Child process
		
		execvp(cmds[0], cmds); // Execute the commands
		
		// Print errors if execvp fails
		fprintf(stderr, "Error: In function 'exec': Child process failed execvp call.\n");
		perror("Error");
		exit(1);
	}
	else
	{
		// Parents process
		if (child == (pid_t)(-1))
		{
			fprintf(stderr, "Error: In function 'exec': Fork call failed.\n");
			perror("Error");
			exit(1);
		}
		else
		{
			c = wait(&cstatus); // Wait for child to complete
			if (cstatus != 0)
			{
				printf("Error: '%s' (process id: %ld) exited with non-zero status: %d\n", cmds[0], (long)c, cstatus);
			}
		}
	}
	
	return 0;
}