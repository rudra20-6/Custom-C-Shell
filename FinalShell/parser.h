#ifndef PARSER_H_INCLUDED  
#define PARSER_H_INCLUDED  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


#include "systemCommands.h"

#define RED   "\x1B[31m"
#define BGRN   "\e[1;32m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BBLU   "\e[1;34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
// #define RESET "\x1B[0m"
#define RESET "\033[0m"
#define BLACK "\e[0;30m"
#define BGGRN "\e[42m"
#define BHWHT "\e[1;97m"
#define BGWHT "\e[47m"
extern char homeDirectory[4096];

void returnDirectory(char desiredDirectory[], int length);
void commandIdentifier(char cmd[], int isPipe, int isRedir);
void printPrompt();
void allHandlerLoop();
int ampercentDetector(char *commsArr[], char *currCom, int numOfComms);
void replaceSubstring(char *str, const char *oldSubstr, const char *newSubstr);

extern int originalSTDIN; extern int originalSTDOUT; extern int originalSTDERR; extern int shell_terminal_out;

#endif