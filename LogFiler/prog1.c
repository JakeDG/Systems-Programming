/* 
* Developed by:
* Mary Slaven, Team Leader
* Jacob Gidley, Recorder
* Daniel Leo, Monitor
*
*/

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_SIZE 150

typedef struct logline
{
  char level[20];
  char timestamp[20];
  char message[100];
} logline_t, *loglinePtr;

typedef struct loglist 
{
  logline_t line;
  struct loglist *next;
} loglist_t, *loglistPtr;

/*	
* Name: Delete List
* Parameter(s):
* 0: struct node** - The pointer pointing to the head node's value(s)
*/
void deleteList (loglistPtr *head)
{
	loglistPtr listItr = *head;		// Used to iterate through the nodes of the list
	loglistPtr nextNode; 			// Pointer pointing to the next node after the list iterator's current position	
	
	while (listItr != NULL)
	{
		nextNode = listItr->next;	// Point to the next node in the list	
		free(listItr);				// Delete the node the list iterator is currently pointing at!		
		listItr = nextNode;			// Go to next node
	}
	
	*head = NULL;	// Delete the head of the list
}

/* Merge */
loglistPtr mergeLists(loglistPtr *resultlist, loglistPtr *inlist)
{
  loglistPtr listItr;
  
  if(*resultlist == NULL)
  {
    *resultlist = *inlist;
  }
  else
  {
    listItr = *resultlist;
    while (listItr->next != NULL)
    {
      listItr = listItr->next;
    }
    listItr->next = *inlist;
  }

  *inlist = NULL;
  return *resultlist;
}

/* Sort */
loglistPtr sortList(loglistPtr inlist) // sort LL inlist and return pointer to the head of sorted list
{
    int sorted;
    loglistPtr hptr = inlist;
    loglistPtr listItr;
    char tempLevel[20];
    char tempTS[20];
    char tempMsg[100];
    
    do
    {
      sorted = 0;  // If sorted doesn’t change, then nothing was swapped so the list is sorted and the loop stops.
      listItr = inlist;  // Start at the begining of the list each time

      while(listItr->next != NULL)
      {
          if(strcmp(listItr->line.timestamp,listItr->next->line.timestamp) > 0)   // Sort if timestamp is less than current node's timestamp 
          {
      			strcpy(tempLevel, listItr->line.level); // Save level
              strcpy(tempTS, listItr->line.timestamp); // Save timestamp
              strcpy(tempMsg, listItr->line.message); // Save message
      
              strcpy(listItr->line.level, listItr->next->line.level);  // Swap the level of the 2nd node into the first
				strcpy(listItr->line.timestamp, listItr->next->line.timestamp);  // Swap the timestamp of the 2nd node into the first
              strcpy(listItr->line.message, listItr->next->line.message);  // Swap the message of the 2nd node into the first

      			strcpy(listItr->next->line.level, tempLevel);  // Store the level of the 1st node into the 2nd
              strcpy(listItr->next->line.timestamp, tempTS); // Store the timestamp of the 1st node into the 2nd
              strcpy(listItr->next->line.message, tempMsg);  // Store the message of the 1st node into the 2nd
              
              sorted = 1; // Nodes were swapped. Keep sorting.
          }
          listItr = listItr->next;	// Increment the iterator
      }
    } while (sorted);

    return hptr;

}

/*
* Name: Print List
* Parameter(s):
* 0: struct loglist* - The pointer pointing to the head of the linked list
*/
void printList(loglistPtr head)
{
	loglistPtr listItr = head; // Used to iterate through the nodes of the list
  
	if (listItr == NULL)
	{
		printf("The list is empty.\n");
		fflush(stdout);
	}
	else
	{
		/* Write the output to the screen until end of list */
		while (listItr != NULL)
		{
     //printf("PRINTING: ");
		  printf("%s,%s,%s\n", listItr->line.level, listItr->line.timestamp, listItr->line.message);
			fflush(stdout);
			listItr = listItr->next;
		}
	}
}

loglinePtr parseLine(char *line)
{
	loglinePtr temp = NULL;
	
	if(!(temp=malloc(sizeof(logline_t)))){
		printf("Memory allocation (temp var of type logline*) failed.");
		exit(1);
	}
	const char delim[2]=",";
	char *token;
	int count=0;
	
	token=strtok(line,delim);
	while(token!=NULL){
		if(count==0){
			strcpy((temp->level),token);
		}
		else if(count==1){
			strcpy((temp->timestamp),token);
		}
		else if(count==2){
			//strcat((temp->line.message),delim);
			strcpy((temp->message),token);
		}
		else{
			strcat((temp->message),delim);
			strcat((temp->message),token);
		}
		//printf("%s\n",line);
		count++;
		token=strtok(NULL, delim);
	}
	//printf("PARSING: %s\n%s\n%s\n",temp->level,temp->timestamp,temp->message);
	return temp;
}

void insert(loglistPtr *head, loglinePtr log)
{
	loglistPtr listItr = (*head), // Used to iterate through the nodes of the list
						temp;	  // Used to allocate memory for linked list and to act as temporary node pointer

	if ((*head) == NULL) // Initial case for creating the first node, or the head of the list
	{
		temp = (loglistPtr)malloc(sizeof(loglist_t)); // Allocate memory
    
    // Copy contents of the struct argument into the struct line of loglist struct
		strcpy(temp->line.level, log->level);
    strcpy(temp->line.timestamp, log->timestamp);
    strcpy(temp->line.message, log->message);
    //printf("INSERTING: %s %s %s\n", temp->line.level, temp->line.timestamp, temp->line.message);
		temp->next = NULL;			// Point to the end of the list
		(*head) = temp;				// Initalize the head to NULL
		return;
	}

	// Not the end of the list AND the symbols don't match, then iterate through the list
	while (listItr->next != NULL)
	{
		listItr = listItr->next;
	}
 
	 // The while loop is terminated because iterator reached the end of the list
		temp = (loglistPtr)malloc(sizeof(loglist_t));		// Allocate memory
		
   // Copy contents of the struct argument into the struct line of loglist struct
		strcpy(temp->line.level, log->level);
    strcpy(temp->line.timestamp, log->timestamp);
    strcpy(temp->line.message, log->message);
    //printf("INSERTING: %s %s %s\n", temp->line.level, temp->line.timestamp, temp->line.message);
    
		temp->next = NULL;				// Point to the end of the list
		listItr->next = temp;			// Initalize the next node to NULL
}

/* Error checking */
int errorCheck(char *str)
{
  int i = 0;
  int commaCount = 0;

  /* Loop through the string to look for at least two commas */
  while(str[i] != '\0' && str[i] != '\n')// check for # of commas
  {
    if(str[i] == ',')
    {
      commaCount++;
    }
    i++;
  }

  if(commaCount < 2)
  {
    fprintf(stderr,"Error: log entry does not contain proper syntax. At least two commas required!\n");//write to stderr if not enough commas
    return 0;
  }

  i = 0;
    while(str[i] != ',') //Check 'level' for capital letters, no spaces
    {
      if(str[i]< 'A'|| str[i] >'Z')
      {
        fprintf(stderr,"Error: Not the proper syntax! The level should be in capital letters.\n");//write to stderr if not capital letters
        return 0;
      }
      i++;
    }
    i++;
    if(str[i+4]!= '-' || str[i+7] != '-' || str[i+10] != ' ' || str[i+13] != ':' || str[i+16] != ':')// first check format
    {
      fprintf(stderr,"This is not proper syntax. The timestamp should follow this format: YYYY-MM-DD HH:MM:SS");
      return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
	struct dirent *dirp;
	DIR *dir;
	FILE *fp, *mfp;
  char line[LINE_SIZE];
  loglistPtr inListHead = NULL; // Create empty list
  loglistPtr resultListHead = NULL; // Create empty list
  
/* Need to check if the directorie's conetents can be accessed */
	if (argc > 2)
	{
		perror("To many arguments provided");
		exit(1);
	}
	
	if (argc == 1 && (dir = opendir(".")) == NULL)
	{
		perror("Cannot open directory");
		exit(1);
	}

	if (argc == 2 && (dir = opendir(argv[1])) == NULL)
	{
		perror("Cannot open directory"); 
		exit(1);
	}
	else
	{
		chdir(argv[1]);	// Change current working driectory
	}
   
 /* Read through the directory's entries */
	while((dirp = readdir(dir)) != NULL)
	{
    /* Dont read from combinedlogs.log */
    if(strcmp(dirp->d_name, "combinedlogs.log") != 0)
    {
      /* Open the entry currently being pointed at*/
  		if((fp = fopen(dirp->d_name, "r")) == NULL) // File open failed.
      {
  			fprintf(stderr, "Error: Cannot open %s\n", dirp->d_name);
        perror("Error");
  			exit(1);
     }
     else
     {
       fgets(line,LINE_SIZE,fp);
       if (line[0] != '#')  /* If the first character is not a '#' then the file is not a log file! */
       {
         //fprintf(stderr, "Error: file is not a log file\n");
       }
       else
       {
         /* Open combined-logs file for appending */
         if((mfp = fopen("combinedlogs.log", "a")) == NULL)
         {
           fprintf(stderr, "Error: Cannot open %s\n", "combinedlogs.log");
           perror("Error");
    	    exit(1);
         }
         
         fprintf(mfp, "%s", line); 
         while(fgets(line, LINE_SIZE, fp) != NULL)
         {
          // lineParse(line);
          if (errorCheck(line) == 1)
          {
            fprintf(mfp, "%s", line); /* Writing to merge file */
          }
         }
         
         fprintf(mfp, "\n"); /* Put newline at end of file */
         fclose(mfp);
       }
     }
   }
	}
 
 /* Open combined-logs file for reading */
 if((mfp = fopen("combinedlogs.log", "r")) == NULL)
 {
   fprintf(stderr, "Error: Cannot open %s\n", "combinedlogs.log");
   perror("Error");
   exit(1);
 }
 
 /* Read from merged log file */
 while (fgets(line, LINE_SIZE, mfp) != NULL)
 {
   //printf("%s\n", line);
   if (line[0] != '#')
   {
    insert(&inListHead, parseLine(line));
    sortList(inListHead);
    mergeLists(&resultListHead, &inListHead);
   }
 }
   sortList(resultListHead);
   printList(resultListHead);
   closedir(dir);
   return 0;
}