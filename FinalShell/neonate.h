#ifndef NEONATE_H_INCLUDED  
#define NEONATE_H_INCLUDED  

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

void monitor_recent_pid(int interval);
int isValidNeonateString(char *str[], int numOfArgs);


#endif