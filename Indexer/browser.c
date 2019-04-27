/*
* Developed by:
* Mary Slaven, Team Leader, mslaven@albany.edu
* Jacob Gidley, Recorder, jgidley@albany.edu
* Daniel Leo, Monitor, dleo2@albany.edu
*
* This file implements the FTW
*/

#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int browser (const char *name, const struct stat *st, int type)
{
  if (type == FTW_NS)
  {
	fprintf(stderr, "Error: In function browser: File doesn't exist.\n");
    perror("Error");
    exit(1);
  }
  if (type == FTW_DNR)
  {
	fprintf(stderr, "Error: In function browser: Directory cannot be accessed.\n");
    perror("Error");
    exit(1);
  }
  if (type == FTW_F)
  {

	indexer(name);
  }
  
  return 0;
}