#ifndef LOG_H_INCLUDED  
#define LOG_H_INCLUDED  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "parser.h"
#include "systemCommands.h"

extern int commandCount;
int countNumberOfLines();
int addLog(char *fname, char *inp);
void displayLog();
void purgeLog();
void execLog(int index, int isPipe);
#endif