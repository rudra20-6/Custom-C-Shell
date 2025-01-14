#ifndef SYSTEMCOMMANDS_H_INCLUDED  
#define SYSTEMCOMMANDS_H_INCLUDED  

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


struct bgProcessData
{
    pid_t pid;
    char cmd[256];
    int isBg;
};

extern pid_t shell_pgid;               // Shell process group ID
extern struct termios shell_tmodes;    // Shell terminal modes
extern int shell_terminal; 
// extern int sigtstp_flag;

// #define MAX_BG_COUNT 120
// extern int proc_count;

// extern int bg_count;
// extern struct bgProcessData bg_processes[MAX_BG_COUNT];

void execSysCommand(char *inpArgs[], int argCount, char *inpCmd, int isRedir);
void execPipeCommands(char *inpArgs[], int argCount, char *inpCmd);

int copyTheBgProcessArray(struct bgProcessData arr[]);

void check_bg_processes();
void addEntryToProcTable(int pid, char *cmd, int isBg);


void setupSignalHandlers();
void handleJobControl(pid_t fg_pid);
void changeToFg(int pid);
void changeToBg(int pid);


void displayCurrentProcTable();
#endif