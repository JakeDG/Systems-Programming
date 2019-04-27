/* 
* Developed by:
* Jacob Gidley, Team Leader, jgidley@albany.edu
* Daniel Leo, Monitor, dleo2@albany.edu
* Mary Slaven, Recorder, mslaven@albany.edu
*
* This file contains the prototypes for the functions related to the input read into the shell.
*/

char* getLine(FILE *stream);
char** parseLine(char *line);
void redir(int fd1, int fd2);
char** reParseLine(char** cmds);
int exec(char **cmds);