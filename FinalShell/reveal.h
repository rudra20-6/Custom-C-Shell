#ifndef REVEAL_H_INCLUDED  // If SPEC1_H_INCLUDED is not defined
#define REVEAL_H_INCLUDED  // Define SPEC1_H_INCLUDED

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

void reveal(char *revArgs[], int numOfRevArgs);
void revealM2(char *revArgs[], int numOfRevArgs);



#endif