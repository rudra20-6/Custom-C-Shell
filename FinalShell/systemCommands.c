
#include "parser.h"

#include "systemCommands.h"

#define MAX_BG_COUNT 120

int proc_count = 0;

int bg_count = 0;
struct bgProcessData bg_processes[MAX_BG_COUNT];

void changeToFg(int pid)
{
    for (int i = 0; i < proc_count; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            bg_processes[i].isBg = 0;
        }

        return;
    }
}

void changeToBg(int pid)
{
    for (int i = 0; i < proc_count; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            bg_processes[i].isBg = 1;
        }

        return;
    }
}

void addEntryToProcTable(int pid, char *cmd, int isBg)
{
    bg_processes[proc_count].pid = pid;
    strcpy(bg_processes[proc_count].cmd, cmd);
    bg_processes[proc_count].isBg = isBg;
    proc_count++;
    if (isBg == 1)
    {
        bg_count++;
    }
}

void trimTrailingSpace(char *str)
{

    int length = strlen(str);
    while (length > 0 && str[length - 1] == ' ')
    {
        str[length - 1] = '\0';
        length--;
    }
}

void displayCurrentProcTable()
{
    printf("\n**************THE PROC TABLE*************\n");
    for (int i = 0; i < proc_count; i++)
    {
        printf("E%d) pid = %d, cmd = %s, isBg = %d\n", i, bg_processes[i].pid, bg_processes[i].cmd, bg_processes[i].isBg);
    }
}

int copyTheBgProcessArray(struct bgProcessData arr[])
{
    int i;
    for (i = 0; i < proc_count; i++)
    {
        arr[i] = bg_processes[i];
    }

    return i;
}

void check_bg_processes()
{
    // printf("I did come here to check\n");
    // displayCurrentProcTable();
    int status;
    pid_t pid;
    // printf("Proc count when i came to check = %d\n", proc_count);
    for (int i = 0; i < proc_count; i++)
    {
        // printf("The cmd %s gave pid %d\n", bg_processes[i].cmd, pid);
        pid = waitpid(bg_processes[i].pid, &status, WNOHANG);

        if (bg_processes[i].isBg == 1)
        {
            // printf("MyBg is big\n");
            if (pid > 0 || pid == -1)
            {
                printf("\n%s ", bg_processes[i].cmd);
                if (WIFEXITED(status) || errno == ECHILD)
                {
                    printf(BGRN);
                    printf("exited normally (%d)", bg_processes[i].pid);
                    printf(RESET);
                    printf("\n");
                }
                else if (WIFSIGNALED(status))
                {
                    printf(RED);
                    printf("exited abnormally due to signal %d (%d).\n", WTERMSIG(status), pid);
                    printf(RESET);
                }

                else
                {
                    printf("pid = %d\n", pid);
                    perror("inBGCHECK: ");
                }

                for (int j = i; j < proc_count - 1; j++)
                {
                    bg_processes[j] = bg_processes[j + 1];
                }
                proc_count--;
                bg_count--;
                i--;
            }

            else if (pid == 0)
            {
            }

            else
            {
                // printf("I came here :)\n");
                perror("inBGCHECK: ");
            }
        }

        else if (bg_processes[i].isBg == 0)
        {

            if (pid > 0 || (pid == -1 && errno == ECHILD))
            {
                if (WIFEXITED(status) || errno == ECHILD || WIFSIGNALED(status))
                {
                    for (int j = i; j < proc_count - 1; j++)
                    {
                        bg_processes[j] = bg_processes[j + 1];
                    }
                    // printf("I decreased the count ;-;\n");
                    // printf("Proc -- with BG STATUS 0\n");
                    proc_count--;
                    i--;
                }
            }

            else if (pid == 0)
            {
            }
        }
    }
}

void sigIntHandler(int sig)
{
    printf("\nCtrl - C Recieved\n");

    int t = proc_count;
    while (t--)
    {
        if (!bg_processes[t].isBg)
        {
            kill(bg_processes[t].pid, SIGINT);
            break;
        }
    }

    signal(SIGINT, sigIntHandler);
}

void sigTSTPHandler(int sig)
{
    dup2(originalSTDIN, STDIN_FILENO);
    dup2(originalSTDOUT, STDOUT_FILENO);
    dup2(originalSTDERR, STDERR_FILENO);

    printf("\nCtrl - Z received\n");

    int fgPID = 0;
    struct bgProcessData copyOfTheProcArr[4096];
    copyTheBgProcessArray(copyOfTheProcArr);

    int t = proc_count;
    while (t--)
    {
        if (!copyOfTheProcArr[t].isBg)
        {

            fgPID = copyOfTheProcArr[t].pid;
            break;
        }
    }

    if (fgPID > 0)
    {
        if (kill(fgPID, SIGTSTP) == -1)
        {
            perror("kill");
        }

        bg_processes[t].isBg = 1;

        if (setpgid(fgPID, fgPID) == -1)
        {
            // perror("setpgid");
        }

        // sigtstp_flag = 1;
        tcsetpgrp(shell_terminal, shell_pgid);
        fgPID = -1;
    }

    else
    {
        printf("No foreground process to stop\n");
    }

    signal(SIGTSTP, sigTSTPHandler);
}

void ctrlDHandler(int sig)
{
    int fgPID = 0;
    struct bgProcessData copyOfTheProcArr[4096];
    copyTheBgProcessArray(copyOfTheProcArr);

    int t = proc_count;
    while (t--)
    {
        fgPID = copyOfTheProcArr[t].pid;
        printf("Killed process with PID %d\n", fgPID);
        kill(fgPID, SIGKILL);
    }
    printf(BGRN);
    printf("Terminal Exitted Successfully\n");
    printf(RESET);
    exit(0);
}

void setupSignalHandlers(void)
{
    // Handle SIGTSTP (Ctrl+Z)
    signal(SIGTSTP, sigTSTPHandler);
    signal(SIGINT, sigIntHandler);
    signal(SIGUSR1, ctrlDHandler);
}

void execSysCommand(char *inpArgs[], int argCount, char *inpCmd, int isRedir)
{

    int bgFlag = 0;

    trimTrailingSpace(inpCmd);

    if (strcmp(inpArgs[argCount - 1], "&") == 0)
    {
        bgFlag = 1;
        // printf("checking for redBackg\n");
        inpArgs[argCount - 1] = NULL;
    }

    int rc = fork();

    if (rc == 0)
    {

        char inpCmdCopy[400];
        strncpy(inpCmdCopy, inpCmd, 400);

        if (!bgFlag)
        {
            if (setpgid(0, shell_pgid) == -1)
            {
                // perror("setpgid");
            }

            tcsetpgrp(shell_terminal, shell_pgid);
        }

        if (bgFlag)
        {
            inpCmdCopy[strlen(inpCmdCopy) - 1] = '\0';

            // if (!isRedir)
            // {
            if (setpgid(0, 0) == -1)
            {
                perror("setpgid 2 Child BG");
            }

            // close(STDIN_FILENO);
        }

        // if (!bgFlag)
        // {
        //     tcsetpgrp(STDIN_FILENO, getpid());
        // }

        char *actualArgs[] = {"/bin/sh", "-c", inpCmdCopy, NULL};

        execvp(inpArgs[0], inpArgs);

        exit(EXIT_FAILURE);
    }
    else if (rc > 0)
    {

        int status;
        usleep(100000);
        // sleep(0.5);

        // if (!bgFlag)
        // {
        //     if (setpgid(rc, tcgetpgrp(STDIN_FILENO)) == -1)
        //     {
        //         perror("setpgid 2 parent FG");
        //     }

        //     tcsetpgrp(STDIN_FILENO, tcgetpgrp(STDIN_FILENO));
        // }

        // if (bgFlag)
        // {
        //     if (setpgid(rc, rc) == -1)
        //     {
        //         perror("setpgid 2 parent BG");
        //     }
        // }
        // printf("");
        if (waitpid(rc, &status, WNOHANG) > 0 && WIFEXITED(status))
        {
            // printf("Debug\n");

            if (WEXITSTATUS(status) != 0)
            {

                printf("Error : '%s' is not a valid command\n", inpArgs[0]);

                return;
            }
        }

        // if (bgFlag)
        // {

        //     if (proc_count < MAX_BG_COUNT)
        //     {
        //         printf("Hai! P P\n");
        //         bg_processes[proc_count].pid = rc;
        //         inpCmd[strlen(inpCmd) - 1] = '\0';
        //         strncpy(bg_processes[proc_count].cmd, inpCmd, sizeof(bg_processes[proc_count].cmd) - 1);
        //         proc_count++;
        //         // printf("proc_count = %d\n", proc_count);

        //         if (isRedir && bgFlag)
        //         {
        //             // int savedState = dup(STDOUT_FILENO); // saving the curr FD from the redirection
        //             // dup2(1, STDOUT_FILENO);                 // redirecting the output to STDOUT
        //             // printf(YEL);

        //             // printf("[%d] %d\n", proc_count, rc);
        //             // // // printf("TEST\n");
        //             // printf(RESET);
        //             // dup2(savedState, 1);

        //             int saved_stdout = dup(STDOUT_FILENO);

        //             // Open /dev/tty (the terminal) for writing
        //             int tty = open("/dev/tty", O_WRONLY);
        //             if (tty == -1)
        //             {
        //                 perror("Failed to open /dev/tty");
        //                 return;
        //             }

        //             // Redirect stdout to the terminal
        //             dup2(tty, STDOUT_FILENO);

        //             // Write directly to the terminal
        //             printf(YEL);

        //             printf("[%d] %d\n", proc_count, rc);
        //             // printf("TEST\n");
        //             printf(RESET);

        //             // Restore the original stdout (redirected to fptr)
        //             dup2(saved_stdout, STDOUT_FILENO);

        //             // Close the temporary file descriptor and /dev/tty
        //             close(saved_stdout);
        //             close(tty);
        //         }

        //         else
        //         {
        //             printf(YEL);

        //             printf("[%d] %d\n", proc_count, rc);
        //             // printf("TEST\n");
        //             printf(RESET);
        //         }
        //     }
        //     else
        //     {
        //         printf(RED);
        //         printf("Too many background processes.\n");
        //         printf(RESET);
        //     }
        // }
        // else
        // {
        //     waitpid(rc, NULL, 0);
        // }

        if (proc_count < MAX_BG_COUNT)
        {
            // printf("Hai! P P\n");
            // bg_processes[proc_count].pid = rc;
            if (bgFlag)
            {
                // bg_processes[proc_count].isBg = 1;
                inpCmd[strlen(inpCmd) - 1] = '\0';
                addEntryToProcTable(rc, inpCmd, 1);
                // bg_count++;
            }

            else
            {
                addEntryToProcTable(rc, inpCmd, 0);
            }

            // else
            // {
            //     bg_processes[proc_count].isBg = 0;
            // }

            // strncpy(bg_processes[proc_count].cmd, inpCmd, sizeof(bg_processes[proc_count].cmd) - 1);
            // proc_count++;

            // printf("proc_count = %d\n", proc_count);

            if (isRedir && bgFlag)
            {
                // int savedState = dup(STDOUT_FILENO); // saving the curr FD from the redirection
                // dup2(1, STDOUT_FILENO);                 // redirecting the output to STDOUT
                // printf(YEL);

                // printf("[%d] %d\n", proc_count, rc);
                // // // printf("TEST\n");
                // printf(RESET);
                // dup2(savedState, 1);

                int saved_stdout = dup(STDOUT_FILENO);

                // Open /dev/tty (the terminal) for writing
                int tty = open("/dev/tty", O_WRONLY);
                if (tty == -1)
                {
                    perror("Failed to open /dev/tty");
                    return;
                }

                // Redirect stdout to the terminal
                dup2(tty, STDOUT_FILENO);

                // Write directly to the terminal
                printf(YEL);

                printf("[%d] %d %s\n", bg_count, rc, RESET);
                // printf("TEST\n");
                // printf(RESET);

                // Restore the original stdout (redirected to fptr)
                dup2(saved_stdout, STDOUT_FILENO);

                // Close the temporary file descriptor and /dev/tty
                close(saved_stdout);
                close(tty);
            }

            else if (bgFlag && !isRedir)
            {
                printf(YEL);

                printf("[%d] %d %s\n", bg_count, rc, RESET);
                // printf(RESET);
            }
        }
        else
        {
            printf(RED);
            printf("Too many background processes.\n");
            printf(RESET);
        }

        if (!bgFlag)
        {
            waitpid(rc, NULL, WUNTRACED);
            tcsetpgrp(shell_terminal, shell_pgid);
        }
    }

    // dup2(stdin_copy, STDIN_FILENO);
    // dup2(stdout_copy, STDOUT_FILENO);
    // dup2(stderr_copy, STDERR_FILENO);
    // close(stdin_copy);
    // close(stdout_copy);
    // close(stderr_copy);
}

void execPipeCommands(char *inpArgs[], int argCount, char *inpCmd)
{

    // adding to proc table

    int bgFlag = 0;

    trimTrailingSpace(inpCmd);

    if (strcmp(inpArgs[argCount - 1], "&") == 0)
    {
        bgFlag = 1;

        inpArgs[argCount - 1] = NULL;
        // argCount--;
    }

    // bg_processes[proc_count].pid = getpid();
    if (bgFlag)
    {
        // bg_processes[proc_count].isBg = 1;
        // printf("Bg flag for proc %s : %d\n", inpCmd, bg_processes[proc_count].isBg);
        inpCmd[strlen(inpCmd) - 1] = '\0';
        // addEntryToProcTable(getpid(), inpCmd, 1);
        // displayCurrentProcTable();
        // bg_count++;
    }

    else
    {
        // addEntryToProcTable(getpid(), inpCmd, 0);
        // displayCurrentProcTable();
    }

    // strncpy(bg_processes[proc_count].cmd, inpCmd, sizeof(bg_processes[proc_count].cmd) - 1);
    // printf("The entry just made\n cmd = %s\n pid = %d\n bg = %d\n", bg_processes[proc_count].cmd, bg_processes[proc_count].pid, bg_processes[proc_count].isBg);
    // proc_count++;
    // printf("Proc count when I last checked it = %d\n", proc_count);

    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    int stderr_copy = dup(STDERR_FILENO);

    char inpCmdCopy[400];
    strncpy(inpCmdCopy, inpCmd, 400);
    char *actualArgs[] = {"/bin/sh", "-c", inpCmdCopy, NULL};

    if (bgFlag)
    {
        if (setpgid(0, 0) == -1)
        {
            perror("setpgid in execPipe");
        }

        if (tcsetpgrp(shell_terminal, shell_pgid) == -1)
        {
            // perror("tcsetpgrp in execPipe");
        }

        // tcset

        // inpCmdCopy[strlen(inpCmdCopy) - 1] = '\0';

        // int saved_stdout = dup(STDOUT_FILENO);

        // // Open /dev/tty (the terminal) for writing
        // int tty = open("/dev/tty", O_WRONLY);
        // if (tty == -1)
        // {
        //     perror("Failed to open /dev/tty");
        //     return;
        // }

        // // Redirect stdout to the terminal
        // dup2(tty, STDOUT_FILENO);

        // // Write directly to the terminal
        // printf(YEL);

        // printf("[%d] %d %s\n", bg_count, getpid(), RESET);
        // // printf("TEST\n");
        // // printf(WHT);

        // // Restore the original stdout (redirected to fptr)
        // dup2(saved_stdout, STDOUT_FILENO);

        // // Close the temporary file descriptor and /dev/tty
        // close(saved_stdout);
        // close(tty);

        printf("%s[%d] %d %s\n", YEL, bg_count, getpid(), RESET);

        // close(STDIN_FILENO);
    }

    int status = execvp(inpArgs[0], inpArgs);

    if (status == -1)
    {
        printf("Error : '%s' is not a valid command\n", inpArgs[0]);

        return;
    }

    exit(EXIT_FAILURE);
}