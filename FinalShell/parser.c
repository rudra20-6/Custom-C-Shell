
#include "hop.h"
#include "parser.h"
#include "reveal.h"
#include "systemCommands.h"
#include "log.h"
#include "proclore.h"
#include "seek.h"
#include "redirection.h"
#include "piping.h"
#include "activities.h"
#include "ping.h"
#include "iMan.h"
#include "fgbg.h"
#include "neonate.h"
#include "time.h"

int firstTimeFlag = 1;
char homeDirectory[4096];
int bigTimeFlag = 0;
int timeFinal = 0;
char *bigTimeProcName[2];

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_terminal_out;
int originalSTDIN, originalSTDOUT, originalSTDERR;

void replaceSubstring(char *str, const char *oldSubstr, const char *newSubstr)
{
    char buffer[1024];
    char *pos;

    pos = strstr(str, oldSubstr);

    if (pos != NULL)
    {
        int lenOld = strlen(oldSubstr);
        int lenNew = strlen(newSubstr);

        strncpy(buffer, str, pos - str);
        buffer[pos - str] = '\0';

        strcat(buffer, newSubstr);
        strcat(buffer, pos + lenOld);

        strcpy(str, buffer);
    }
}

void returnDirectory(char desiredDirectory[], int length)
{

    getcwd(desiredDirectory, length);
    if (firstTimeFlag)
    {

        strcpy(homeDirectory, desiredDirectory);
        strcpy(desiredDirectory, "~");
        firstTimeFlag = 0;
    }

    else
    {
        replaceSubstring(desiredDirectory, homeDirectory, "~");
        // if (strcmp(desiredDirectory, homeDirectory) == 0)
        // {

        //     strcpy(desiredDirectory, "~");
        // }
    }
}

void printPrompt()
{
    char sysName[256];
    char *userName;
    gethostname(sysName, sizeof(sysName));
    userName = getlogin();
    char theDirectory[4096];
    returnDirectory(theDirectory, sizeof(theDirectory));
    if (!bigTimeFlag)
    {
        printf("\n<%s%s@%s:%s%s%s> ", BGRN, userName, sysName, BBLU, theDirectory, RESET);
    }

    else
    {
        
        printf("\n<%s%s@%s:%s%s%s %s : %ds> ", BGRN, userName, sysName, BBLU, theDirectory, RESET, bigTimeProcName[0],timeFinal);
        bigTimeFlag = 0;
        strcpy(bigTimeProcName[0], "");
    }
    
    
    
}


void trimTrailingSpaceDup(char *str)
{
    int length = strlen(str);
    while (length > 0 && str[length - 1] == ' ')
    {
        str[length - 1] = '\0';
        length--;
    }
}
int takingInput(char *inputStringArray[])
{

    // takes input from user and separates it using ; delimeter
    // takes a (empty) array string as input and puts ; separated commands in that array
    // also RETURNS the length of that array, i.e how many commandsm thus the int function type

    char command[4096];

    int scanStatus = scanf(" %[^\n]s", command);

    if (scanStatus == EOF)
    {
        raise(SIGUSR1);
    }
    char theShellFolder[] = "/FinalShell";
    char logAddress[4096];
    strcpy(logAddress, homeDirectory);
    strcat(logAddress, theShellFolder);
    strcat(logAddress, "/logFile.txt");

    addLog(logAddress, command);
    trimTrailingSpaceDup(command);
    char *token = strtok(command, ";");
    int i = 0;
    while (token != NULL)
    {
        inputStringArray[i] = token;
        token = strtok(NULL, ";");
        i++;
    }

    return i;
}

void commandIdentifier(char cmd[], int isPipe, int isRedir)
{
    



    char copyOfCmd[4096];
    char copyOfCmd2[4096];
    char copyOfCmd3[4096];
    char copyOfCmd4[4096];
    char copyOfCmd5[4096];
    char copyOfCmd6[4096];
    char copyOfCmd7[4096];
    char copyOfCmd8[4096];
    char copyOfCmd9[4096];
    char copyOfCmd10[4096];
    char copyOfCmd11[4096];
    char copyOfCmd12[4096];
    char copyOfCmd13[4096];

    strcpy(copyOfCmd, cmd);
    strncpy(copyOfCmd2, cmd, 255);
    strncpy(copyOfCmd4, cmd, 255);
    strncpy(copyOfCmd3, cmd, 255);
    strncpy(copyOfCmd5, cmd, 255);
    strncpy(copyOfCmd6, cmd, 255);
    strncpy(copyOfCmd7, cmd, 255);
    strncpy(copyOfCmd8, cmd, 255);
    strncpy(copyOfCmd9, cmd, 255);
    strncpy(copyOfCmd10, cmd, 255);
    strncpy(copyOfCmd11, cmd, 255);
    strncpy(copyOfCmd12, cmd, 255);
    strncpy(copyOfCmd13, cmd, 255);
    copyOfCmd2[255] = '\0';

    char *cmdSubStr = strtok(copyOfCmd, " \t");

    if (strcmp(cmdSubStr, "hop") == 0)
    {
        // printf("INHOP\n");
        char *argsOfHop[256];
        int j = 0;
        char *currArg = strtok(copyOfCmd2, " \t");
        while (currArg != NULL)
        {
            argsOfHop[j++] = currArg;
            currArg = strtok(NULL, " \t");
        }

        if (j == 1)
        {
            hop("~");
        }

        else
        {
            int ctr = 1;

            while (ctr < j)
            {

                hop(argsOfHop[ctr]);
                ctr++;
            }
        }

        // printf("OutOfHop\n");
    }

    else if (strcmp(cmdSubStr, "reveal") == 0)
    {
        // printf("INREVEAL\n");
        // fprintf(stderr, "INREVEAL\n");
        char *argsOfReveal[256];
        int k = 0;
        char *tokenRev = strtok(copyOfCmd2, " \t");

        while (tokenRev != NULL)
        {

            argsOfReveal[k++] = tokenRev;
            tokenRev = strtok(NULL, " \t");
        }

        if (k == 1)
        {
            argsOfReveal[1] = strdup("FakeArgument");
        }

        revealM2(argsOfReveal, k);

        // fprintf(stderr, "OUTREVEAL\n");
    }

    else if (strcmp(cmdSubStr, "log") == 0)
    {
        int numOfArgsOfLog = 0;

        char *currArg = strtok(copyOfCmd5, " \t");
        char *argsOfLog[256];

        while (currArg != NULL)
        {
            argsOfLog[numOfArgsOfLog] = currArg;
            currArg = strtok(NULL, " \t");
            numOfArgsOfLog++;
        }

        if (numOfArgsOfLog <= 1)
        {
            displayLog();
        }

        else
        {
            if (strcmp(argsOfLog[1], "purge") == 0)
            {

                purgeLog();
            }

            else if (strcmp(argsOfLog[1], "execute") == 0)
            {
                if (numOfArgsOfLog == 2)
                {
                    execLog(1, isPipe);
                }

                else
                {
                    execLog(atoi(argsOfLog[2]), isPipe);
                }
            }
        }
    }

    else if (strcmp(cmdSubStr, "proclore") == 0)
    {
        int numOfProcArgs = 0;
        char *currArg = strtok(copyOfCmd6, " \t");
        char *argsOfProc[256];

        while (currArg != NULL)
        {
            argsOfProc[numOfProcArgs] = currArg;
            currArg = strtok(NULL, " \t");
            numOfProcArgs++;
        }

        int shellPid = getpid();

        if (numOfProcArgs <= 1)
        {
            displayData(shellPid);
        }

        else if (numOfProcArgs == 2)
        {
            displayData(atoi(argsOfProc[1]));
        }

        else
        {

            printf("Invalid Arguments!\n");
        }
    }

    else if (strcmp(cmdSubStr, "seek") == 0)
    {
        int initFileCount = 0;
        int initDirCount = 0;
        int numOfSeekArgs = 0;
        char foundFile[4096] = {0};
        char foundDir[4096] = {0};
        char *currArg = strtok(copyOfCmd6, " \t");
        char *argsOfSeek[256];

        while (currArg != NULL)
        {
            argsOfSeek[numOfSeekArgs] = currArg;
            currArg = strtok(NULL, " \t");
            numOfSeekArgs++;
        }
        int flagD = 0, flagF = 0, flagE = 0;
        char keyToSearch[4096];
        strcpy(keyToSearch, argsOfSeek[numOfSeekArgs - 1]);
        if (numOfSeekArgs == 2)
        {
            seek(".", keyToSearch, 0, 0, 0, &initFileCount, &initDirCount, foundFile, foundDir);
        }

        else
        {
            for (int i = 1; i < numOfSeekArgs - 1; i++)
            {
                if (strcmp(argsOfSeek[i], "-d") == 0)
                {
                    flagD = 1;
                }

                if (strcmp(argsOfSeek[i], "-e") == 0)
                {
                    flagE = 1;
                }

                if (strcmp(argsOfSeek[i], "-f") == 0)
                {
                    flagF = 1;
                }
            }
            seek(".", keyToSearch, flagD, flagF, flagE, &initFileCount, &initDirCount, foundFile, foundDir);
            if (flagE)
            {
                handle_e_flag(initFileCount, initDirCount, foundFile, foundDir);
            }
        }

        if (initDirCount == 0 && initFileCount == 0)
        {
            printf("No File/Directory found\n");
        }
    }

    else if (strcmp(cmdSubStr, "activities") == 0)
    {
        char *currArg = strtok(copyOfCmd7, " \t");
        int numOfArgs = 0;

        while (currArg != NULL)
        {
            currArg = strtok(NULL, " \t");
            numOfArgs++;
        }

        if (numOfArgs > 1)
        {
            printf("Invalid Arguments for activities\n");
        }

        else
        {
            printActivities();
        }
    }

    else if (strcmp(cmdSubStr, "ping") == 0)
    {
        char *currArg = strtok(copyOfCmd8, " \t");
        char *argsOfPing[256];
        int pingArgCount = 0;
        while (currArg != NULL)
        {
            argsOfPing[pingArgCount] = currArg;
            currArg = strtok(NULL, " \t");
            pingArgCount++;
        }

        handlePing(argsOfPing, copyOfCmd9, pingArgCount);
    }

    else if (strcmp(cmdSubStr, "iMan") == 0)
    {
        char *currArg = strtok(copyOfCmd10, " \t");
        char *argsOfiMan[256];
        int iManArgCount = 0;
        while (currArg != NULL)
        {
            argsOfiMan[iManArgCount] = currArg;
            currArg = strtok(NULL, " \t");
            iManArgCount++;
        }

        if (iManArgCount == 1)
        {
            printf("Invalid Arguments for iMan\n");
        }

        else
        {
            displayMan(argsOfiMan[1]);
        }
        
        
        
    }

    else if (strcmp(cmdSubStr, "fg") == 0)
    {
        char *currArg = strtok(copyOfCmd11, " \t");
        char *argsOfFg[256];
        int fgArgCount = 0;
        while (currArg != NULL)
        {
            argsOfFg[fgArgCount] = currArg;
            currArg = strtok(NULL, " \t");
            fgArgCount++;
        }

        if (fgArgCount != 2 || !isValidInt(argsOfFg[1]))
        {
            printf("Invalid Arguments for fg\n");
        }
        
        else
        {
            if (isValidInt(argsOfFg[1]))
            {
                handleFg(atoi(argsOfFg[1]));
            }
            
        }
        
    }

    else if (strcmp(cmdSubStr, "bg") == 0)
    {
        char *currArg = strtok(copyOfCmd12, " \t");
        char *argsOfBg[256];
        int bgArgCount = 0;
        while (currArg != NULL)
        {
            argsOfBg[bgArgCount] = currArg;
            currArg = strtok(NULL, " \t");
            bgArgCount++;
        }

        if (bgArgCount != 2 || !isValidInt(argsOfBg[1]))
        {
            printf("Invalid Arguments for fg\n");
        }
        
        else
        {
            if (isValidInt(argsOfBg[1]))
            {
                handleBg(atoi(argsOfBg[1]));
            }
            
        }
        
    }

    else if (strcmp(cmdSubStr, "neonate") == 0)
    {
        char *currArg = strtok(copyOfCmd13, " \t");
        char *argsOfNeonate[256];
        int neonateArgCount = 0;
        while (currArg != NULL)
        {
            argsOfNeonate[neonateArgCount] = currArg;
            currArg = strtok(NULL, " \t");
            neonateArgCount++;
        }

        if (!isValidNeonateString(argsOfNeonate, neonateArgCount))
        {
            printf("Invalid Arguments for Neonate!!\n");
        }

        else
        {
            monitor_recent_pid(atoi(argsOfNeonate[2]));
        }
        
        
    }
        
    else if (strcmp(cmdSubStr, "exit") == 0)
    {
        // exit(0);
        raise(SIGUSR1);
    }



    else
    {
        char *inputArray[256];
        char *tokenize = strtok(copyOfCmd3, " \t");
        int i = 0;
        while (tokenize != NULL)
        {
            inputArray[i++] = tokenize;
            tokenize = strtok(NULL, " \t");
        }

        inputArray[i] = NULL;
        // int t= 0;
        // while (t < i)
        // {
        //     printf("%s//", inputArray[t++]);
        // }
        // printf("\n");
        int ctr = 0;
        // while (ctr < i)
        // {
        //     printf("%s_", inputArray[ctr++]);
        // }
        // printf("\n");
        if (isPipe)
        {
            // printf("ThatsQuiteFar\n");

            execPipeCommands(inputArray, i, copyOfCmd4);
        }

        else
        {

            fflush(stdout);

            execSysCommand(inputArray, i, copyOfCmd4, isRedir);
        }
    }
}

int ampercentDetector(char *commsArr[], char *currCom, int numOfComms)
{

    if (strstr(currCom, "&") != NULL)
    {

        int i, count;
        for (i = 0, count = 0; currCom[i]; i++)
        {
            count += (currCom[i] == '&');
        }

        char copyCurrCom[400];
        strncpy(copyCurrCom, currCom, 400);
        char *token = strtok(currCom, "&");
        int countampercent = 0;
        while (token != NULL)
        {
            commsArr[numOfComms] = malloc(400);
            if (countampercent == count)
            {
                snprintf(commsArr[numOfComms], 400, "%s", token);
            }
            else
            {
                snprintf(commsArr[numOfComms], 400, "%s &", token);
                countampercent++;
            }

            numOfComms++;

            token = strtok(NULL, "&");
        }
    }
    else
    {
        commsArr[numOfComms] = malloc(400);
        strncpy(commsArr[numOfComms++], currCom, 400);
    }

    return numOfComms;
}

void allHandlerLoop()
{
    
    originalSTDIN = dup(STDIN_FILENO);
    originalSTDOUT = dup(STDOUT_FILENO);
    originalSTDERR = dup(STDERR_FILENO);


    shell_terminal = STDIN_FILENO;
    shell_terminal_out = STDOUT_FILENO;
    shell_pgid = getpid();

    // Put the shell in its own process group
    setpgid(shell_pgid, shell_pgid);
    tcsetpgrp(shell_terminal, shell_pgid);

    // Save the shell's terminal modes
    tcgetattr(shell_terminal, &shell_tmodes);
    setupSignalHandlers();

    while (1)
    {
        
        int mainTerminalPID = getpid();

        char command[4096];
        char *commands_arr[4096];
        char *newCommandsArr[4096];
        // if (sigtstp_flag)
        // {
        //    printPrompt();
        // }
        
        printPrompt();

        int numOfCommands = takingInput(commands_arr);
        check_bg_processes();
        
        int t = 0;
        int newComArrayLength = 0;
        while (t < numOfCommands)
        {
            trimTrailingSpaceDup(commands_arr[t]);
            if (!isItValidPipe(commands_arr[t]) && strstr(commands_arr[t], "|") != NULL)
            {
                printf("Invalid use of Pipe\n");
                t++;
                continue;
            }

            if (strstr(commands_arr[t], "&") != NULL && (strstr(commands_arr[t], ">") != NULL || strstr(commands_arr[t], "<") != NULL) && commands_arr[t][strlen(commands_arr[t]) - 1] != '&')
            {
                printf("Invalid use of & with redirection\n");
                t++;
                continue;
            }
            
            
            newComArrayLength = ampercentDetector(newCommandsArr, commands_arr[t], newComArrayLength);
            t++;
        }

        t = 0;
        while (t < newComArrayLength)
        {

            struct timespec start;
            struct timespec end;
            clock_gettime(CLOCK_REALTIME, &start);
            int isRedirection = redirectionDetector(newCommandsArr[t]);
            if (isItValidPipe(newCommandsArr[t]))
            {
                setupPipeCommands(newCommandsArr[t]);
            }

            else if (!isItValidPipe(newCommandsArr[t]) && strstr(newCommandsArr[t], "|") != NULL)
            {
                printf("Invalid use of pipe\n");
            }

            // int isRedirection = redirectionDetector(newCommandsArr[t]);
            else if (isRedirection)
            {
                redirectionExecute(newCommandsArr[t], 0);
            }

            else
            {
                commandIdentifier(newCommandsArr[t], 0, 0);
            }

            clock_gettime(CLOCK_REALTIME, &end);
            timeFinal = (double)(end.tv_sec - start.tv_sec);
            if (timeFinal > 2)
            {
                char quickCopyString[4096];
                strncpy(quickCopyString, newCommandsArr[t], 4096);

                char *token = strtok(quickCopyString, " \t");
                bigTimeProcName[0] = token;
                bigTimeFlag = 1;
                
                // strcpy(bigTimeProcName, strtok(quickCopyString, " "));

                // strcpy(bigTimeProcName, strstr(newCommandsArr[t], ""));
            }
            
            t++;
        }
    }
}
