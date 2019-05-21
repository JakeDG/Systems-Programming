/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Recorder
* Mary Slaven, Monitor
*
* This file contains the archiveSize function.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "constants.h"

// Struct that holds file name and size, as well as flags
typedef struct fileRecord
{
  int fSize; // Holds file's original size
  int size;	// Holds the file's size it will take up in the archive
  int archived;	// Flag for if the file has been archived
  int canFit;	// Flag for if the file can fit into the archive
  char name[NAME_LEN];	// Holds the name of the file
} fileRec;

/*
* Name: archiveSize
* Parameter(s):
* 0: char** - A pointer to the pointer of the list of filenames 
* 1: int - Number of files in the list of files provided
* 2: char* - Pointer to the archive file's name
* 3: int - Archive size
*
* Description:
* Creates an archive with the specified name and size from the specified files provided 
* in the command line.
*/
void archiveSize(char **fileNames, int numFiles, char* archiveName, int archiveSize)
{  
  int ad;	// Archive descriptor
  int fd; // File descriptor
  int filesArchived = 0;  // Holds the amount of files archived
  int oversized = 0; // Holds the amount of oversized files
  int i, j; // Loop counter
  int archSize = archiveSize - sizeof(int); // Stores the archive size minus the 4 bytes need to store the number of files in the archive
  int filesToArch = 0; // Holds number of files that fit in archive
  int archCntr = 0; // Holds the number of archives
  
  char **fnames = fileNames;  // Holds list of file names
  char newArchName[NAME_LEN]; // Holds the new name of the archive file
  char contentByte;	// Holds a byte of from the contents of a file
  char nSize; // Holds the size of the file name
  
  fileRec files[numFiles]; // Array of file record structs
  
  // Initialize the file records in the struct array
  for (i = 0; i < numFiles; i++)
  {
    strcpy(files[i].name, *fnames);
	files[i].fSize = fileSize(*fnames);
    files[i].size = sizeof(char) + (strlen(*fnames) + 1) + sizeof(int) + fileSize(*fnames);
    files[i].archived = 0;
    files[i].canFit = 0;
    
    // If the file size is greater than the archive size argument then increment the oversized variable
    if (files[i].size > archSize)
    {
      oversized++;
      fprintf(stderr, "Warning: %s too large to fit into archive of size %d. It will not be written to the archive(s).\n", files[i].name, archiveSize);
    }
    
    fnames++;
  }
  
  // If none of the files could fit into the archive then output a messege and return to main.
  if (oversized == numFiles)
  {
    fprintf(stderr, "Error: None of the files provided can fit into an archive size of %d bytes.\n", archiveSize);
    return;
  }
  
  // Keep looping until all files that are not oversized have been archived
  while (filesArchived < numFiles - oversized)
  {
    // Loop through file list to get the number of files that can fit in the archive
    for(i = 0; i < numFiles; i++)
    {
      if (files[i].archived != 1 && files[i].size <= archSize)
      {
        archSize = archSize - files[i].size;
        
        if (archSize >= 0)
    	{
			files[i].canFit = 1;  // Set flag that file can fit to true
			filesToArch++;
		}
      }
    }
    
    // If all files can fit into the single archive then call the archive function and return to main
    if (filesToArch == numFiles)
    {
      archive(fileNames, numFiles, archiveName); // Build the archive
      return;
    }
    
    // Append number to file
    sprintf(newArchName, "%s%d", archiveName, ++archCntr);	// Assign number to archive
    
    // Create archive file
    ad = open(newArchName, O_RDWR|O_CREAT|O_APPEND, MODE & M_MASK);
    if(ad == -1)
    {
      fprintf(stderr, "Error: In function archive: Archive failed to open.\n");
      perror("Error");
      exit(1);
    }
     
    // Write number of files to archive
    if(write(ad, &filesToArch, sizeof(filesToArch)) == -1)
    {
      fprintf(stderr, "Error: In function archive: Number of files not written to archive.\n");
      perror("Error");
    }
    
    // Iterate through every file included as an argument
    for(i = 0; i < numFiles; i++)
    {
      // Check if file has been archived
      if(files[i].archived != 1 && files[i].canFit == 1)
      {
        // Open each file in the array
        fd = open(files[i].name, O_RDONLY);
        if(fd == -1)
        {
          fprintf(stderr, "Error: In function archive: File failed to open.\n");
          perror("Error");
          exit(1);
        }
        
        // Write the file name size to archive
        nSize = (char)strlen(files[i].name);	// Get the length of the file name
      	if(write(ad, &nSize, sizeof(nSize)) == -1)
        {
          fprintf(stderr, "Error: In function archive: Size of file name not written to archive.\n");
          perror("Error");
        }
        
        // Write file name to archive
        if(write(ad, (const void *)files[i].name, (size_t)(strlen(files[i].name) + 1)) == -1)
        {
			fprintf(stderr, "Error: In function archive: File name not written to archive.\n");
			perror("Error");
        }
        
        // Write file size to archive
        if(write(ad, &files[i].fSize, sizeof(files[i].fSize)) == -1)
        {
			fprintf(stderr, "Error: In function archive: Size of file not written to archive.\n"); 
			perror("Error"); 
        }
        
        // Write the contents of the file to the archive (Read byte from file into a char buffer -> Write that by byte into the archive)
        for(j = 0; j < fileSize(files[i].name); j++)
        {
          // Read from the input file byte-by-byte and store into a char variable
          if (read(fd, &contentByte, sizeof(contentByte)) < 0)
          { 
            fprintf(stderr, "Error: In function archive: Cannot read contents of file.\n");
            perror("Error");
          }
          else
          { 
            // Write byte-by-byte from the char buffer variable
            if(write(ad, &contentByte, sizeof(contentByte)) == -1)
            {
             fprintf(stderr, "Error: In function archive: File contents not written to archive.\n"); 
             perror("Error");
            }
          }
        }
       
        files[i].archived = 1;  // Set file flag as being archived
        filesArchived++; // Increment the amount of files archived 
        close(fd);  // Close file
      }
      
    }
      
      close(ad);  // Close archive file
      archSize = archiveSize - sizeof(int); // Reset archive size
      filesToArch = 0;  // Reset the number of files to archive
  }
}