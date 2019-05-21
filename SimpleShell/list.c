/*
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Monitor
* Mary Slaven, Recorder
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

/* Name: isDir
Parameter(s):
0: char * - A Pointer to the Directory's name

Description:
Determines if the given argument is a directory
*/

int isDir(char *name)
{

	struct stat stats;  // stat struct

	// Check for errors
	if (stat(name, &stats) != 0)  // Get the stats of the file
	{

		return 0;
	}

	// Check if file is a directory
	if (S_ISDIR(stats.st_mode))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
Name: fileStat
Parameter(s):
0: char * - A pointer to the given path name


Description:
Prints to screen the file name, size, permissions and node number for all files in a directory.

*/

void fileStat(char *pathName)
{
    struct dirent *dp;
    DIR *dir;
    struct stat stats;
    if(isDir(pathName)==0)
    {
        fprintf(stderr,"Error: Argument is not a directory.\n");
       exit(1);

    }
    else
       {
        chdir(pathName);
       }
         if((dir=opendir("."))== NULL)
       {
           fprintf(stderr,"Error: Could not open directory.\n");
		   perror("Error");
           exit(1);
       }
       while((dp= readdir(dir)) != NULL)
            {
            if(stat(dp->d_name, &stats)!=0)
            {
                fprintf(stderr,"Did not get file stats.\n");
				perror("Error");
            }
            else
            {
            printf("%-20s%-5d\t", dp->d_name,stats.st_size);
            printf("%-5o\t", (stats.st_mode &(S_IRWXU | S_IRWXG | S_IRWXO)));
            printf("%d\n", stats.st_ino);

            }
        }
}


int main(int argc, char *argv[])
{
  struct dirent *dp;
  DIR *dir;

    if(argc==1)
    {
        if((strcmp(argv[0],"list")==0))
        {
           // printf("Testing current directory 'list'");
            dir=opendir(".");
            while((dp= readdir(dir)) != NULL)
            {
            if(dp->d_name[0]!= '.')
            {
            printf("%s\n", dp->d_name);
            }
            }
        }

    }
   else if(argc == 2)
   {
      if(isDir(argv[1]))
       {
       //list all non-hidden files in pathname
       chdir(argv[1]);
        if((dir=opendir("."))== NULL)
       {
           fprintf(stderr,"Could not open directory");
           exit(1);
       }
       while((dp= readdir(dir)) != NULL)
            {
            if(dp->d_name[0]!= '.')
            {
            printf("%s\n", dp->d_name);
            }
            }
       }
       else if(strcmp(argv[1],"-i")==0)
       {
           //current file
           //print file name, size and permissions(oct) and inode#
           fileStat(".");


       }
       else if(strcmp(argv[1],"-h")==0)
       {
        //current file
        //print names of hidden files
        dir=opendir(".");
            while((dp= readdir(dir)) != NULL)
            {
            if(dp->d_name[0]== '.')
            {
            printf("%s\n", dp->d_name);
            }
            }
       }
       else
       {
           fprintf(stderr, "Argument is not a directory.\n");
		   perror("Error");
           exit(1);
       }
   }
   else if(argc==3)
   {
       if(strcmp(argv[1],"-i")==0)
       {
        //on given filepath
        //print file name, size and permissions(oct) and inode#
            fileStat(argv[2]);

       }
       else if(strcmp(argv[1],"-h")==0)
       {
        //on given filepath
        //print names of hidden files
        if(isDir(argv[2]))
        {
        chdir(argv[2]);
        }
         else
       {
        fprintf(stderr,"Argument is not a directory\n");
		perror("Error");
       exit(1);
       }
       if((dir=opendir("."))== NULL)
       {
           fprintf(stderr,"Could not open directory");
		   perror("Error");
           exit(1);
       }
       while((dp= readdir(dir)) != NULL)
            {
            if(dp->d_name[0]== '.')
            {
            printf("%s\n", dp->d_name);
            }
            }

       }
   }
   else
   {
    fprintf(stderr,"Error: Incorrect number of arguments given.\n");
   exit(1);
   }
return 0;
}
