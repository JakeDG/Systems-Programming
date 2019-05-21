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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_SIZE 150

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
        fprintf(stderr,"This is not the proper syntax.The level should be in capital letters.\n");//write to stderr if not capital letters
        return 0;
      }
      printf("%c ", str[i]);
      i++;
    }
    i++;
    
    if(str[i+4]!= '-' || str[i+7] != '-' || str[i+10] != ' ' || str[i+13] != ':' || str[i+16] != ':')// first check format
    {
      fprintf(stderr,"This is not proper syntax. The timestamp should follow this format: YYYY-MM-DD HH:MM:SS\n");
      return 0;
    }
    while(str[i] != '-')//check that YYYY is numbers
    {
      if(str[i]< '0' || str[i] > '9')
      {
        fprintf(stderr,"this is not proper syntax. The timestamp should contain numbers\n");
        return 0;
      }
      i++;
    }
    i++;
    while(str[i] != '-')//check that MM is numbers
    {
      if(str[i]< '0' || str[i] > '9')
      {
        fprintf(stderr,"this is not proper syntax. The timestamp should contain numbers\n");
        return 0;
      }
      i++;
    }
    i++;
    while(str[i] != ' ')//check that DD is numbers
    {
      if(str[i]< '0' || str[i] > '9')
      {
        fprintf(stderr,"this is not proper syntax. The timestamp should contain numbers\n");
        return 0;
      }
      i++;
    }
    i++;
    while(str[i] != ':')//check that HH is numbers
    {
      if(str[i]< '0' || str[i] > '9')
      {
        fprintf(stderr,"this is not proper syntax. The timestamp should contain numbers\n");
        return 0;
      }
      i++;
    }
    i++;
    while(str[i] != ':')//check that MM is numbers
    {
      if(str[i]< '0' || str[i] > '9')
      {
        fprintf(stderr,"this is not proper syntax. The timestamp should contain numbers\n");
        return 0;
      }
      i++;
    }
    i++;
    while(str[i] != ':')//check that SS is numbers
    {
      if(str[i]< '0' || str[i] > '9')
      {
        fprintf(stderr,"this is not proper syntax. The timestamp should contain numbers\n");
        return 0;
      }
      i++;
      }
    //I don't think we need to check 'message' no standard syntax
    return 1;
}

/* Parse line function */
void parseLine(char *str)
{
  char *token;

  token = strtok(str, ",");

  while( token != NULL )
  {
      printf( " %s", token );

      token = strtok(NULL, ",");
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
	struct dirent *dirp;
	DIR *dir;
	FILE *fp, *mfp;
  char line[LINE_SIZE];

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

	if (argc == 2 && (dir = opendir(argv[1])) == NULL)	/* Does a program always process in the current working directory??? */
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
    /* Open the entry currently being pointed at*/
		if((fp = fopen(dirp->d_name, "r")) == NULL) // File open failed.
    {
			fprintf(stderr, "Error: Cannot open %s\n", dirp->d_name);
      perror("Error");
			exit(1);
   }
   else
   {
     printf("Opening %s\n", dirp->d_name);
     if (fgetc(fp) != '#')  /* If the first character is not a '#' then the file is not a log file! */
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
 }

	closedir(dir);
	return 0;
}
