/* 
* Developed by:
* Mary Slaven, Team Leader
* Jacob Gidley, Recorder
* Daniel Leo, Monitor
*
* Description:
* This program will take in arguments from the command line to create an index of terms 
* found withhin files from a provided directory supplied in the command line arguments. 
* The first argument, inverted-index file name, is the name of a file that your program should
* create to hold the inverted index. The indexer should save the index in the following format:
* <list> term
* name1 count1 name2 count2 name3 count3 name4 count4 name5 count5
* </list>
* The second argument, directory or file name, is the name of the directory or file that your
* indexer should index. If a file, your program needs to index that single file. If a directory is
* provided instead, your program will need to recursively index all files in the directory and its
* sub-directories.
* If no argument is given, your indexer program should operate on the current working directory
* and generate file “invind.txt”. If only a directory is provided as argument indexer program should
* operate on the specified directory and generate the default index file “invind.txt”
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>

#include "lists.h"
#include "listFuncs.h"
#include "indexList.h"	// Globally defined linked list head
#include "miscFuncs.h"
#include "indexProtos.h"

int main (int argc, char *argv[])
{

	if (argc > 3)
	{
		fprintf(stderr, "Error: Too many arguments provided!\n");
		exit(1);
	}
	
	// If no argument is given, the indexer program should operate on the current working directory and generate file “invind.txt”
	if (argc == 1)
	{
		printf("ftw called on current directory\n");
		ftw(".", browser, FTW_D);
		writeList(tokListHead, "invind.txt");
	}
	
	// If only a directory is provided as argument (i.e., the first argumentis a directory), your indexer program should operate on the specified directory and generate the default index file “invind.txt”.
	if (argc == 2 && isDir(argv[1]))
	{
		ftw(argv[1], browser, FTW_D);
		writeList(tokListHead, "invind.txt");
	}
	else if (argc == 2 && !isDir(argv[1]))
	{
		indexer(argv[1]);
		writeList(tokListHead, "invind.txt");
	}
	
	// If all arguments are given then index the directory or file and output the index to the file provided as and argument
	if (argc == 3 && isDir(argv[2]))
	{
		ftw(argv[2], browser, FTW_D);
		writeList(tokListHead, argv[1]);
	}
	else if (argc == 3 && !isDir(argv[2]))
	{
		indexer(argv[2]);
		writeList(tokListHead, argv[1]);
	}
	
	return 0;
}