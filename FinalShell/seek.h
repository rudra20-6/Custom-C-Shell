#ifndef SEEK_H_INCLUDED  
#define SEEK_H_INCLUDED  

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

// void seek(char *dirToOpen, char *key, int flag_d, int flag_f, int flag_e);

// void seek(char *dirToOpen, char *key, int flag_d, int flag_f, int flag_e, int *totalFileCount, int *totalDirCount);

void seek(char *dirToOpen, char *key, int flag_d, int flag_f, int flag_e, int *totalFileCount, int *totalDirCount, char foundFile[], char foundDir[]);
void handle_e_flag(int totalFileCount, int totalDirCount, char foundFile[], char foundDir[]);

#endif