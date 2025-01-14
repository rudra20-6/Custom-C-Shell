#include "signal.h"

#include "redirection.h"

int isValidInt(const char *str)
{
    // If string is empty or NULL, it's not a valid integer
    if (str == NULL || *str == '\0')
    {
        return 0;
    }

    // If the first character is a negative or positive sign, skip it
    if (*str == '-' || *str == '+')
    {
        str++;
    }

    // Check if the remaining characters are all digits
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0; // Invalid if a non-digit character is found
        }
        str++;
    }

    return 1; // String is a valid integer
}

int isValidPingCommand(char *argArray[], char *theCmd, int argCount)
{
    if (argCount != 3)
    {
        return 0;
    }

    trim(argArray[1]);
    trim(argArray[2]);

    if (!isValidInt(argArray[1]) || !isValidInt(argArray[2]))
    {
        return 0;
    }
    int pid = atoi(argArray[1]);

    if (kill(pid, 0) == -1) // checking if the pid is valid
    {
        if (errno == ESRCH)
        {
            printf("Process with PID %d does not exist.\n", pid);
        }
        else if (errno == EPERM)
        {
            printf("No permission to check process with PID %d.\n", pid);
        }
        else
        {
            perror("Failed to check PID");
        }
        return 0;
    }

    return 1;
}

void handlePing(char *argArray[], char *theCmd, int argCount)
{
    if (!isValidPingCommand(argArray, theCmd, argCount))
    {
        printf("Invalid Arguments for ping !\n");
        return;
    }

    int sigNum = atoi(argArray[2]) % 32;
    int killSuccess = kill(atoi(argArray[1]), sigNum);

    if (killSuccess == 0)
    {
        printf("Sent signal %d to process with PID %d\n", sigNum, atoi(argArray[1]));
    }

    else
    {
        printf("Failed to send signal %d to process with PID %d\n", sigNum, atoi(argArray[1]));
    }
}