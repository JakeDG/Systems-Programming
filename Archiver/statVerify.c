/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Recorder
* Mary Slaven, Monitor

* This file contains the archStat and archVerify functions.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "constants.h"

/*
* Name: archStat
* Parameter(s):
* 0: char* - A pointer to the archive file's name
* 
* Description:
* Prints to stdout the total size of the archive file, the number of files
* in the archive, and each filename along with the corresponding file size.
*/
void archStat(char *archive)
{
  int numFiles;  // Holds the number of files in the archive
  int archiveSize;  // Holds archive size
  int fSize;  // Holds file size
  int i;  // Loop counter
  int ad;  // Archive descriptor
  char fileName[NAME_LEN]; // Holds filename
  char nameSize; // Holds size of filename
  
  // Open archive file
  ad = open(archive, O_RDONLY);
  if(ad == -1)
  {
    fprintf(stderr, "Error: In function archStat: Archive failed to open.\n");
    perror("Error");
    exit(1);
  }

  // Print size of archive
  archiveSize = fileSize(archive);
  printf("Archive size: %d bytes\n", archiveSize);

  // Get the number of files in the archive
  read(ad, &numFiles, sizeof(numFiles));
  printf("Number of files in archive: %d\n", numFiles);

  // Iterate through each of the files in the archive and print their stats
  for (i = 0; i < (numFiles); i++)
  {
    read(ad, &nameSize, sizeof(nameSize));

    read(ad,fileName, (size_t)(nameSize+1));
    printf("File: %s, ", fileName);

    read(ad, &fSize, sizeof(fSize));  
    printf("Size: %d bytes\n", fSize);
	
    lseek(ad,fSize, SEEK_CUR);  // Jump to next record
  }
    
  close(ad); // Close archive
}

/*
* Name: archVerify
* Parameter(s):
* 0: char** filenames- pointer to array of char array pointers that are the incoming filenames
* 1: int numFiles- the size of the incoming array of filenames
* 2: char* archivename- A pointer to the archive file's name
* 
* Description:
* Verifies contents of archive against files in args array
* -there is no assumption that the files in the incoming array of arguments
*	will make up 100% of the files in the archive, though it is possible 
*	that this is the case.
* -if there are two files in the incoming array to compare to the archive contents, and both
*	of those files are completely and accurately represented in the archive, that archive will
*	be considered successfully verified by this function
*/
void archVerify(char** filenames, int numFiles, char* archivename){
	
	int ad, fd, i, j, k;		//descriptors and loop counters
	
	//archive vars
	int fileNum; 				//number of files
	char archNSize; 			//filename length from archive
	char archFName[NAME_LEN]; 	//name of file from archive
	int aFSize; 				//archived file size
	int aCnt; 					//counter for number of bytes in archive file content
	char a;						//holds a byte to be compared to similar byte in file
	
	//file arg vars
	//int numFiles; is count of file names in args
	char nSize;				 	//filename length
	char *fName; 				//name of file
	int fSize;					//size of file to check
	int fCnt; 					//counter for number of bytes in file content
	char f;						//holds a byte to be compared to similar byte in archive
	
	//int missingBytes=0; 		//counter for missing bytes (may not be neccessary)
	int isCorrupt=0;  			//boolean int to indicate file corruption in archive
	int match=0;   				//boolean int to indicate current state of file and archive match
	int byteMatch;				//boolean int to indicate if a mismatched byte has yet been found
	int matchCounter=0; 		//increments as files are verified
	int missingBytes=0;			//running total of missing bytes (if any)
	int extraBytes=0;			//running total of bytes in archive, that are not in file
	int fileConfirmed;			//if file is found and not corrupt (boolean)
	
  	
	
  	//associate ad file descriptor with archive
	ad = open(archivename, O_RDONLY);
	if(ad == -1){
		fprintf(stderr, "Archive failed to open.\n");
		perror("Error");
		exit(1);
  	}
  	
  	if(read(ad, &fileNum, sizeof(fileNum))<0){
  		fprintf(stderr, "Error: In function verify: Unable to read in number of files\n");
  		perror("Error");
  	}
  	
  	
  	for(i=0;i<numFiles;i++){  //start iterating through files (total: numFiles)
  		//assign current file name to fName
  		fName=(*filenames);
  		//check filename in output
  		
  		//associate fd file descriptor with current file
  		fd = open(fName, O_RDONLY);
  		if(fd==-1){
  			fprintf(stderr, "File failed to open.\n");
			perror("Error");
			exit(1);
  		}
  		fSize=lseek(fd,0,SEEK_END);
  		lseek(fd,0,SEEK_SET);
  		nSize = (char)strlen(fName);
  		
  		/* LOOP: 1, traverse archive following 'ad' to each file within
  			archive until a match to the outer loops current file is found (or not found).
  		*/
  		match=0;
  		for(j=0;j<fileNum && match==0;j++){ //search sequentially through the archive for each file (arg)

	  		//FILE NAME LENGTH CHECK
	  		if(read(ad,&archNSize,sizeof(archNSize))<0){
	  			fprintf(stderr, "Error: In function verify: Unable to read length of file name\n");
	  			perror("Error");
	  		}
			
	  		//compare name sizes here...continue if they are the same
  			if(nSize==archNSize)
  			{
  				//FILENAMES CHECK
	  			if (read(ad, archFName, (size_t)(archNSize + 1)) < 0){
					fprintf(stderr, "Error: In function verify: Unable to read archived file name\n");
					perror("Error");
				}

				//compare name strings here
				if(strcmp(fName, archFName)==0)
				{
					//FILE SIZES CHECK
					match=1;
				  	if(read(ad, &aFSize, sizeof(aFSize))<0){
				  		fprintf(stderr, "Error: In function verify: Unable to read archived file size\n");
						perror("Error");
				  	}

				  	if(aFSize==fSize)
				  	{
				  		//CONTENTS CHECK
		      	
					  	isCorrupt=0;
					  	fCnt=0;
					  	aCnt=0;
					  	
					  	/*LOOP: 2, char by char compare
					  	Need to advance file descriptors (fd,ad) one byte at a time and compare each until expected
					  	content size is met
					  	*/
					  	
					  	for(k=0;k<aFSize;k++){
					  		//printf("LOOP: 3");
					  		if(read(ad, &a, sizeof(a))<0){
					  			fprintf(stderr, "Error: In function verify: Unable to read char in archived file\n");
								perror("Error");
					  		}
					  		aCnt++;
						  	if(fCnt<fSize){	
						  		if(read(fd, &f, sizeof(f))<0){
						  			fprintf(stderr, "Error: In function verify: Unable to read char in file\n");
									perror("Error");
						  		}
						  		fCnt++;
						  	}
					  		if(a!=f){
					  			//fileConfirmed=0;
					  			isCorrupt=1;
					  		}
					  	} // end: LOOP: 3
					  	
					  	if(!isCorrupt){
					  		matchCounter+=1;
					  	}else {
					  		
					  		printf("Archive is corrupted.\n");
					  		break;
					  	}
					}else if(aFSize!=fSize){//find missing bytes
						if(aFSize>fSize){
				  			missingBytes+=aFSize-fSize;
				  		}
				  		if(aFSize<fSize){
				  			extraBytes+=fSize-aFSize;
				  		}
					}
					else{
						lseek(ad,aFSize,SEEK_CUR);
					}// end: if: aFSize==fSize, CONTENTS CHECK
				}
				else{
					if(read(ad, &aFSize, sizeof(aFSize))<0){//advances 'ad' to contents and gives size
				  		fprintf(stderr, "Error: In function verify: Unable to read archived file size\n");
						perror("Error");
				  	}
					lseek(ad,aFSize,SEEK_CUR);//advances past contents to next file name's length
				}// end: if: FILE SIZES CHECK
			}
			else{
				//lseek(ad,archNSize,SEEK_CUR);
				if (read(ad, archFName, (size_t)(archNSize + 1)) < 0){
					fprintf(stderr, "Error: In function verify: Unable to read archived file name\n");
					perror("Error");
				}
				
				//printf("Skipped file #%d: %s\n", j+1,archFName);
				
				if(read(ad, &aFSize, sizeof(aFSize))<0){
					fprintf(stderr, "Error: In function verify: Unable to read archived file size\n");
					perror("Error");
				}
				
				lseek(ad,aFSize,SEEK_CUR); //must get aFSize before doing this due to file sizes varrying
			}// end: if: FILE NAMES CHECK
		}// end: LOOP: 2, FILE NAME LENGTH CHECK
  		
  		//reset archive descriptor for next file comparison
  		lseek(ad,sizeof(fileNum),SEEK_SET); //reset to first archive file after fileNum for new file to check
  		close(fd); //free up file descriptor for next file to use
  		filenames++; //increment to next file from argument array of char arrays (filenames/strings)
  	} //end: LOOP: 1
	
  	if(matchCounter == numFiles){
	  	
  		printf("Archive verified.\n");
  	}

  	if(extraBytes>0)printf("Archive contains %d extra byte(s).\n",extraBytes);
  	if(missingBytes>0)printf("Archive is missing %d byte(s).\n", missingBytes);
}
