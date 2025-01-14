#ifndef PROCLORE_H_INCLUDED  
#define PROCLORE_H_INCLUDED  

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
#include <errno.h>


void displayData(int pid);
int doesPIDexist(int pid);



#endif