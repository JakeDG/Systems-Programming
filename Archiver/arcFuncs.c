/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Recorder
* Mary Slaven, Monitor
*
* This file contains the archive and unarchive functions.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "constants.h"

/*
* Name: archive
* Parameter(s):
* 0: char** - A pointer to the pointer of the list of filenames 
* 1: int - Number of files in the list of files provided
* 2: char* - Pointer to the archive file's name
*
* Description:
* Prints to stdout the total size of the archive file, the number of files
* in the archive, and each filename along with the corresponding file size.
*/
void archive(char **filenames, int numFiles, char *archivename)
{
  int i, j; // Loop counters
  int fd; // File descriptor for the input files
  int ad; // Archive file descriptor
  char nSize; // Holds the size of a file's name
  char contentByte; // Holds a byte of a file's contents
  int fSize; // Holds a file's size
  char *file; // Holds the name of a file's name
  
  //Create archive file
  ad = open(archivename,O_RDWR|O_CREAT|O_APPEND, MODE & M_MASK);
  if(ad == -1)
  {
    fprintf(stderr, "Error: In function archive: Archive failed to open.\n");
    perror("Error");
    exit(1);
  }
  
  // Write number of files to archive
 	if(write(ad, &numFiles, sizeof(numFiles)) == -1)
  {
    fprintf(stderr, "Error: In function archive: Number of files not written to archive.\n");
    perror("Error");
  }

  // Iterate through every file included as an argument
  for(i = 0; i < numFiles; i++)
  {
      file = *(filenames); // Store the name of the current file in the list
      
      // Open each file in the array
      fd = open(file, O_RDONLY);
      if(fd == -1)
      {
        fprintf(stderr, "Error: In function archive: File failed to open.\n");
        perror("Error");
        exit(1);
      }

      /* Get info needed for each file in array */
      fSize = fileSize(file);   // Get size of each file in bytes

      /* Begin writing to the archive */
	  // Write the size of file name
	  nSize = (char)strlen(file);	// Get the length of the file name
	  
	  if(write(ad, &nSize, sizeof(nSize)) == -1)
      {
        fprintf(stderr, "Error: In function archive: Size of file name not written to archive.\n");
        perror("Error");
      }
    	
      // Write file name to archive
      if(write(ad,(const void *)file, (size_t)(strlen(file) + 1)) == -1)
      {
        fprintf(stderr, "Error: In function archive: File name not written to archive.\n");
        perror("Error");
      }
      
      // Write file size to archive
      fSize = fileSize(file);
      
      if(write(ad, &fSize, sizeof(fSize)) == -1)
      {
       	fprintf(stderr, "Error: In function archive: Size of file not written to archive.\n"); 
        perror("Error"); 
      }
      
      // Write the contents of the file to the archive (Read byte from file into a char buffer -> Write that by byte into the archive)
      for(j = 0; j < fSize; j++)
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
           perror("E Error");
          }
        }
      }
      
    close(fd); // Close the file
    filenames++;  // Move to next file name in list
  }
  
  close(ad);
}

/*
* Name: unarchive
* Parameter(s):
* 0: char* - A pointer to the archive file's name
* 
* Description:
* “Unpacks” the specified archive file and generates each file contained within the archive.
*/
void unarchive(char *archivename)
{
  	int ad;	// Archive descriptor
  	int fd; // File descriptor
  	int fileNum;	// Number of files
    int i, j;  // Loop counters
    int fSize; // Holds size of file
  	char nSize;	// Length of filename
  	char contentByte; 	// Holds a byte of from the contents of a file
  	char fName[NAME_LEN]; // Holds name of file
  	
  	// Open archive file for reading
  	ad = open(archivename, O_RDONLY);
  	if(ad == -1)
  	{
    	fprintf(stderr, "Error: In function unarchive: Archive failed to open.\n");
    	perror("Error");
    	exit(1);
  	}
  	
  	// Read the number of files in the archive
  	if (read(ad, &fileNum, sizeof(fileNum)) < 0)  // Print number of files
    {
      fprintf(stderr, "Error: In function unarchive: Unable to in read number of files\n");
      perror("Error");
      exit(1);
    }
   
    // Re-create every file in the archive
    for(i = 0; i < fileNum; i++)
    {
      // Read in size of filename
      if (read(ad, &nSize, sizeof(nSize)) < 0)
      {
        fprintf(stderr, "Error: In function unarchive: Unable to in read length of file name\n");
        perror("Error");
      }
      
      // Read in filename
      if (read(ad, fName, (size_t)(nSize + 1)) < 0)
      {
        fprintf(stderr, "Error: In function unarchive: Unable to in read file name\n");
        perror("Error");
      }
      
      // Create file and open the file
      fd = open(fName, O_RDWR|O_CREAT, MODE & M_MASK);
      if(fd == -1)
      {
        fprintf(stderr, "Error: In function unarchive: File failed to open.\n");
        perror("Error");
        exit(1);
      }
      
      // Read in file size
      if (read(ad, &fSize, sizeof(fSize)) < 0)
      {
        fprintf(stderr, "Error: In function unarchive: Unable to in read file size\n");
        perror("Error");
      }
      
      // Write the file's contents from the archive to the file (Read byte from archive into a char buffer -> Write that by byte into the file)
      for(j = 0; j < fSize; j++)
      {
        // Read from the input file byte-by-byte and store into a char variable from the archive
        if (read(ad, &contentByte, sizeof(contentByte)) < 0)
        { 
          fprintf(stderr, "Error: In function unarchive: Unable to read contents of the file from the archive.\n");
          perror("Error");
        }
        else
        { 
          // Write byte-by-byte from the char buffer variable to the file
          if(write(fd, &contentByte, sizeof(contentByte)) == -1)
          {
           fprintf(stderr, "Error: In function unarchive: File contents not written to archive.\n"); 
           perror("E Error");
          }
        }
      }
      close(fd);	// Close file
    }
  close(ad);	// Close archive
}

