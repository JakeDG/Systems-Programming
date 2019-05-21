/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Monitor
* Mary Slaven, Recorder
*
* This file contains the create function.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* create.c 
 * Takes arguments in the form of options (f,d,s,h)
 * f-creates a new file if one w/ the same name (argv[2]) does not already exist
 * d-makes a new directory with given argument (2) as name
 * h-creates hard link to argv[2] file with name argv[3]
 * s-creates symlink to argv[2] file with name argv[3]
 * */

int main (int argc, char *argv[]){
    int fd;
    mode_t fmode=640, dmode=750;
    
    //Check for proper number of arguments
    if(argc < 3){
		fprintf(stderr, "Error: Not enough arguments provided!\n");
		exit(1);
	}
	if(argc > 5){
        fprintf(stderr, "Error: Too many arguments provided!\n");
		exit(1);
    }
    //There are two options that use three arguments: creat and mkdir
    if(argc==3){ 
        //creat system call creates file with given path/name if not already in existence
        if(strcmp(argv[1],"-f")==0){
            umask(0000);
            fd=creat(argv[2],(mode_t)0640);//create file with user:rw and group: r
            if(fd==-1){
                fprintf(stderr, "Error: In create program: File not created.\n");
                perror("Error");
                exit(1);
            }
            close(fd);
        }
        //make a directory with the path/name from the given arguments
        else if(strcmp(argv[1],"-d")==0){
            umask(0000);
            if(mkdir(argv[2],(mode_t)0750)==-1){ //creat directory with user: rwx and group: r-x
                fprintf(stderr, "Error: In create program: Directory not created.\n");
                perror("Error");
                exit(1);
            }
        }
        //imporoper option found
        else{
            fprintf(stderr, "Error: In create program: Unrecognized option.\n");
            exit(1);
        }
    }
    if (argc==4){
        //create a hard link for input path/file with name after input file
        if(strcmp(argv[1],"-h")==0){
            if(access(argv[2],F_OK)!=0){
                fprintf(stderr, "Error: In create program: File does not exist, hard link creation failed.\n");
                exit(1);
            }
            else{
                if((fd=link(argv[2],argv[3]))<0){
                    fprintf(stderr, "Error: In create program: Hard link creation failed.\n");
                    exit(1);
                }
                close(fd);
            }
        }
        //create symlink to input path/file with path/name given after file
        else if(strcmp(argv[1],"-s")==0){
            if(access(argv[2],F_OK)!=0){
                fprintf(stderr, "Error: In create program: File does not exist, symlink creation failed.\n");
                exit(1);
            }
            else{
                if((fd=symlink(argv[2],argv[3]))<0){
                    fprintf(stderr, "Error: In create program: Symlink creation failed.\n");
                    exit(1);
                }
                close(fd);
            }
        }
    }
    return 0;
}
