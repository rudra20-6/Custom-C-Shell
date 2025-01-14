#ifndef REDIRECTION_H_INCLUDED  
#define REDIRECTION_H_INCLUDED  

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

int redirectionDetector(char *theCmd);
void redirectionExecute(char *theCmd, int isPipe);
void inputFromFile(char cmd[], char fname[], char *ma[], int ctr);
void trim(char *str);




#endif