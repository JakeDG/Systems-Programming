/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Monitor
* Mary Slaven, Recorder
*
* Description:
* This program creates a simplified command–line shell, similar to bash.
* It will shell will interpret script files, in addition to accepting one command at a time from the keyboard.
* The user types a command, and then presses ‘return’ on the keyboard to execute it.
* The shell will until the command completes and, if the return code is abnormal, a message will be printed out to that effect. 
* The shell will also open a given file file argument and interpret the contents as a script.
* It is assumed that each line of the script file corresponds to one command.
* The shell will accept the following commands:
*	- list [-i | -h] [pathname] [-i pathname] [-h pathname]\
*	- create [-f filepath] [-d dirpath] [-h oldname linkname] [-s oldname linkname]
*	- wd
*	- chwd
*	- quit
* The shell will also allow output to be redirected to a file (eg. list -l > foo)
*
* Assumptions: 
* - It will be assumed that a file's line size will be at most 255 characters in length.
* - It is also assumed that there will be no blank lines or whitespace before characters in a file.
*
* Command line input example: simpleshell [scriptfile]
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "input.h"
#include "shellFunctions.h"

#define M_MASK 0777 // File mode mask
#define MODE 0644 // File mode

int main(int argc, char *argv[])
{
  char *command; // Holds a line of input
  char **commands; // Holds an array of command arguments
  FILE *fp; // Input file pointer
  int i; // Counter variable
  int outfd; // Output file descriptor
  int stdout_cpy; // Saves a copy of the current stdout's file descriptor
  int isReDir = 0; // Boolean to check if redirection occurred

  // Check if no script file was provided
  if (argc == 1)
  {
	while(1) // Continue asking for a command until user enters 'quit'
	{
		printf("$> "); // Prompt user
		command = getLine(stdin); // Get line of input
		
		if(command[0] == NULL) // Check if command is empty
			continue;
			
		// Check for if redirection was input in the input command
		if (strstr(command, " > ") != NULL) 
		{
			commands = parseLine(command); // Parse the line of input into a 2D array of arguments
			for(i = 0; commands[i] != NULL; i++)
			{
				// Loop unitl '>' if found
				if (strcmp(commands[i], ">") == 0)
				{
					i++;
					isReDir = 1;
						
					// Open the output file
					outfd = open(commands[i], O_WRONLY|O_CREAT|O_APPEND, MODE & M_MASK);
					if(outfd == -1)
					{
						fprintf(stderr, "Error: In function 'main': File '%s' failed to open.\n", commands[i]);
						perror("Error");
						exit(1);
					}
						
					stdout_cpy = dup(STDOUT_FILENO); // Save the current stdout file descriptor
					redir(outfd, STDOUT_FILENO); // Redirect output
					commands = reParseLine(commands); // Re-parse the line of input
					break;
				}
			}
		}
		else
		{
			commands = parseLine(command); // Parse the line of input into a 2D array of arguments
		}
		
		if (commands[0] == NULL) // Check if command was blank
		{
			continue;
		}
		else if (strcmp(commands[0], "quit") == 0) // Quit the program
		{
			quit();	
		}
		else if (strcmp(commands[0], "wd") == 0) // Print current working directory
		{
			wd();
		}
		else if (strcmp(commands[0], "chwd") == 0) // Change the current working directory
		{
			chwd(commands[1]);
		}
		else if (strcmp(commands[0], "list") == 0) // Execute the list program
		{
			exec(commands);
		}
		else if (strcmp(commands[0], "create") == 0) // Execute the create program
		{
			exec(commands);
		}
		else
		{
			fprintf(stderr, "Error: %s: command not found.\n", commands[0]);
		}
			
		// Free memory
		free(command);
		free(commands);
			
		// Redirect output back to stdout if redirection was applied
		if (isReDir)
		{
			redir(stdout_cpy, STDOUT_FILENO); 
			isReDir = 0;
			close(outfd);
		}
	}
  }
  else if (argc == 2) 
  {
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "Error: In function 'main': File '%s' failed to open.\n", argv[1]);
		perror("Error");
		exit(1);
	}
	
	while (1)
	{
		command = getLine(fp); // Get line of input from file
		
		if(command[0] == NULL) 
			break;
	
		// Check for if redirection was input in the input command
		if (strstr(command, " > ") != NULL) 
		{
			
			commands = parseLine(command); // Parse the line of input into a 2D array of arguments
			for(i = 0; commands[i] != NULL; i++)
			{
				// Loop unitl '>' is found
				if (strcmp(commands[i], ">") == 0)
				{
					i++;
					isReDir = 1;
					
					outfd = open(commands[i], O_WRONLY|O_CREAT|O_APPEND, MODE & M_MASK);
					if(outfd == -1)
					{
					  fprintf(stderr, "Error: In function 'main': File '%s' failed to open.\n", commands[i]);
					  perror("Error");
					  exit(1);
					}
					
					stdout_cpy = dup(STDOUT_FILENO); // Save the current stdout file descriptor
					redir(outfd, STDOUT_FILENO); // Redirect output
					commands = reParseLine(commands); // Re-parse the line of input
					break;
				}
			}
		}
		else
		{
			commands = parseLine(command); // Parse the line of input into a 2D array of arguments
		}
	
		if (commands[0] == NULL) // Check if command was blank
		{
			continue;
		}
		else if (strcmp(commands[0], "quit") == 0) // Quit the program
		{
			quit();	
		}
		else if (strcmp(commands[0], "wd") == 0) // Print current working directory
		{
			wd();
		}
		else if (strcmp(commands[0], "chwd") == 0) // Change the current working directory
		{
			chwd(commands[1]);
		}
		else if (strcmp(commands[0], "list") == 0) // Execute the list program
		{
			exec(commands);
		}
		else if (strcmp(commands[0], "create") == 0) // Execute the create program
		{
			exec(commands);
		}
		else
		{
			fprintf(stderr, "Error: %s: command not found.\n", commands[0]);
		}
		
		// Free memory
		free(command);
		free(commands);
		
		// Redirect output back to stdout if redirection was applied
		if (isReDir)
		{
			redir(stdout_cpy, STDOUT_FILENO);
			isReDir = 0;
			close(outfd);
		}
	}
	
	fclose(fp);
  }
  else
  {
	fprintf(stderr, "Error: Too many command line arguments provided.\n");
	exit(1);
  }
  
  return 0;
}