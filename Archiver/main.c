/* 
* Developed by:
* Jacob Gidley, Team Leader, jgidley@albany.edu
* Daniel Leo, Recorder, dleo2@albany.edu
* Mary Slaven, Monitor, mslaven@albany.edu
*
* Description:
* This program will take in arguments from the command line and based on those arguments do the following:
* 1.) If -a flag is detected, then the program will create an archive file and store the amount of files provided as arguments, 
* as well as each file's name, name length, size, and contents. If a size is provided, then the program will create archive with
* the specified name and size from the specified files. If the desired archive size does not suffice for all specified files
* to be stored in a single archive file, then the program will create as many archive files as needed.
* 2.) If the -u flag is detected, then the program will "unpack" the specified archive and create the files whose data is stored in it.
* 3.) If the -l flag is detected, then the program will print to stdout the total size of the archive, the number of files in the archive, and each filename along with the corresponding file size.
* 4.) If the -v flag is detected, then the program will determine whether or not the specified archive is damaged
* It will be assumed that all file names will be 1-255 characters in length. It will also be assumed that the 
* permissions given to each file upon creation are: rwx --- ---
* 
* Command line input example: archiver [-a|-u|-l|-v] archivename [archivesize] [file1 file2 . . . fileN]
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arcFuncsProtos.h"
#include "statVerifyProtos.h"
#include "sizeProtos.h"

int main (int argc, char *argv[])
{
  int flagCunt = 0; // Holds the number of flags detected in the command line arguments
  int i; // Loop counter
  int fileNum;  // Holds the amount of files being sent to the archiver
  int archSize; // Holds potential archive size
  char **filesList; // Holds a pointer to the list of filenames
  
  // Check for more than one flag in command line
  for(i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "-a") == 0)
      flagCunt++;
      
    if (strcmp(argv[i], "-u") == 0)
      flagCunt++;
      
    if (strcmp(argv[i], "-l") == 0)
      flagCunt++;
      
    if (strcmp(argv[i], "-v") == 0)
      flagCunt++;
    
    if (flagCunt > 1)
    {
      fprintf(stderr, "Error: More than one flag provided.\n");
      exit(1);
    }
  }
  
  /* Check for correct flags and if the number of arguments supplied match the flags needed */
  if(strcmp(argv[1], "-a") == 0) // -a flag detected
  {
    if(argc >= 4)
    {
      if (isalpha(*argv[3]) == 0) // Check if an archive size was provided
      {
        fileNum = argc - 4; // Get the number of files in the command line
      	filesList = argv + 4;  // Get the list of file names
        archSize = atoi(argv[3]); // Get archive size
     	archiveSize(filesList, fileNum, argv[2], archSize); // Build the archive
      }
      else
      {
        fileNum = argc - 3; // Get the number of files in the command line
      	filesList = argv + 3;  // Get the list of file names
     	archive(filesList, fileNum, argv[2]); // Build the archive
      }
    }
    else
    {
      fprintf(stderr, "Error: Not enough arguments provided for the flag.\n");
      exit(1);
    }
  }
  else if(strcmp(argv[1], "-u") == 0) // -u flag detected
  {
    if(argc == 3)
    {
      unarchive(argv[2]); // Unpack the archive
    }
    else
    {
      fprintf(stderr, "Error: Too many arguments provided for the flag.\n");
      exit(1);
    }
  }
  else if(strcmp(argv[1], "-l") == 0) // -l flag detected
  {
    if(argc == 3)
    {
      archStat(argv[2]);  // Display archive statistics
    }
    else
    {
      fprintf(stderr, "Error: Too many arguments provided for the flag.\n");
      exit(1);
    }
  }
  else if(strcmp(argv[1], "-v") == 0) // -v flag detected
  {
    if(argc >= 4)
    {
      fileNum = argc - 3; // Get the number of files in the command line
	  filesList = argv + 3;  // Get the list of file names
      archVerify(filesList, fileNum, argv[2]);
    }
    else
    {
      fprintf(stderr, "Error: Not enough arguments provided for the flag.\n");
      exit(1);
    }
  }
  else  // Wrong flag provided
  {
    fprintf(stderr, "Error: Unknown flag detected.\n");
    exit(1);
  }  
  
  return 0;
}