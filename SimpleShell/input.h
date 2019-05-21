/* 
* Developed by:
* Jacob Gidley, Team Leader
* Daniel Leo, Monitor
* Mary Slaven, Recorder
*
* This file contains the prototypes for the functions related to the input read into the shell.
*/

char* getLine(FILE *stream);
char** parseLine(char *line);
void redir(int fd1, int fd2);
char** reParseLine(char** cmds);
int exec(char **cmds);