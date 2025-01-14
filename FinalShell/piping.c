#include "redirection.h"
#include "systemCommands.h"
#define MAXPIPES 100

int detect_double_pipe(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {

        if (str[i] == '|')
        {
            int j = i + 1;

            while (str[j] != '\0' && isspace(str[j]))
            {
                j++;
            }
            if (str[j] == '|')
            {

                return 1;
            }
        }
    }
    return 0;
}

int isItValidPipe(char *cmd)
{
    char copyOfCmd[4096];
    strncpy(copyOfCmd, cmd, 4096);

    trim(copyOfCmd);
    if (strstr(copyOfCmd, "|") != NULL)
    {
        if (copyOfCmd[strlen(copyOfCmd) - 1] == '|' || copyOfCmd[0] == '|' || detect_double_pipe(copyOfCmd))
        {

            return 0;
        }

        else
        {
            if (strstr(copyOfCmd, "&") != NULL && copyOfCmd[strlen(copyOfCmd) - 1] != '&')
            {
                printf("Syntax Error near '|' \n");
                return 0;
            }

            return 1;
        }
    }
}

// void setupPipeCommands(char inpCmds[])
// {
//     // Check for '&' at the end and trim it if present
//     int backgroundProcess = 0;
//     int len = strlen(inpCmds);
//     if (inpCmds[len - 1] == '&')
//     {
//         backgroundProcess = 1;
//         // inpCmds[len - 1] = '\0'; // Remove '&' from the command
//     }

//     // Tokenizing the command
//     char copyOfInpCmd2[4096];
//     strncpy(copyOfInpCmd2, inpCmds, 4096);
//     char *pipeCmds[4096];
//     int k = 0;
//     char *token = strtok(copyOfInpCmd2, "|");
//     while (token != NULL)
//     {
//         trim(token);
//         pipeCmds[k++] = token;
//         token = strtok(NULL, "|");
//     }

//     int z = 0;
//     int numOfPipes = 0; // numOfPipes = number of commands - 1
//     while (z < strlen(inpCmds))
//     {
//         if (inpCmds[z] == '|')
//         {
//             numOfPipes++;
//         }
//         z++;
//     }

//     if (numOfPipes > 0)
//     {
//         int pipes[MAXPIPES][2];

//         for (int x = 0; x < numOfPipes; x++)
//         {
//             pipe(pipes[x]);
//         }

//         for (int i = 0; i < numOfPipes + 1; i++)
//         {
//             pid_t pid = fork();
//             if (pid == 0) // Child process
//             {
//                 if (i > 0) // All except the first command
//                 {
//                     dup2(pipes[i - 1][0], STDIN_FILENO); // Read from previous pipe
//                 }
//                 if (i < numOfPipes) // All except the last command
//                 {
//                     dup2(pipes[i][1], STDOUT_FILENO); // Write to next pipe
//                 }

//                 for (int j = 0; j < numOfPipes; j++)
//                 {
//                     close(pipes[j][0]);
//                     close(pipes[j][1]);
//                 }

//                 if (redirectionDetector(pipeCmds[i]))
//                 {
//                     redirectionExecute(pipeCmds[i], 1);
//                 }
//                 else
//                 {
//                     commandIdentifier(pipeCmds[i], 1, 0);
//                 }

//                 exit(EXIT_SUCCESS);
//             }
//             else if (i == numOfPipes && backgroundProcess == 0) // Last command in the foreground
//             {
//                 waitpid(pid, NULL, 0); // Wait for the last process
//             }

//         }

//         // Close all pipe file descriptors in the parent process
//         for (int l = 0; l < numOfPipes; l++)
//         {
//             close(pipes[l][0]);
//             close(pipes[l][1]);
//         }

//         if (backgroundProcess == 1)
//         {
//             /* code */
//         }

//         if (!backgroundProcess) // Wait for non-background processes
//         {
//             for (int m = 0; m < numOfPipes; m++)
//             {
//                 wait(NULL);
//             }
//         }
//     }
// }

void setupPipeCommands(char inpCmds[])
{
    int pipeBgFlag = 0;
    // tokenizing the command
    char copyOfInpCmd2[4096];
    strncpy(copyOfInpCmd2, inpCmds, 4096);
    char *pipeCmds[4096];
    int k = 0;
    char *token = strtok(copyOfInpCmd2, "|");
    while (token != NULL)
    {
        trim(token);
        pipeCmds[k++] = token;
        token = strtok(NULL, "|");
    }

    int z = 0;
    int numOfPipes = 0; // numOfPipes = number of commands -1
    while (z < strlen(inpCmds))
    {
        if (inpCmds[z] == '|')
        {
            // printf("debug\n");
            numOfPipes++;
        }

        z++;
    }
    // printf("Num Of Pipes : %d", numOfPipes);

    if (pipeCmds[numOfPipes][strlen(pipeCmds[numOfPipes]) - 1] == '&')
    {
        // printf("Bg mila toh tha... ;-;\n");
        pipeBgFlag = 1;
        // pipeCmds[numOfPipes][strlen(pipeCmds[numOfPipes]) - 1] = '\0';
    }


    if (numOfPipes == 0)
    {
        // printf("Yaha kaise aye?\n");
    }

    else
    {
        // printf("Disappointment\n");
        int pipes[MAXPIPES][2];

        for (int x = 0; x < numOfPipes; x++)
        {
            pipe(pipes[x]);
        }

        for (int i = 0; i < numOfPipes; i++)
        {
            if (fork() == 0)
            {
                // wait(NULL);
                // usleep(200000);
                // printf("I came here once\n");
                // printf("The command I am carrying : '%s'\n", pipeCmds[i]);
                if (i > 0) // all except first command
                {
                    // read from previous pipe

                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }

                if (i < numOfPipes) // all except the last command
                {
                    // write to next pipe
                    // printf("//%s//\n", pipeCmds[i]);
                    dup2(pipes[i][1], STDOUT_FILENO);
                }

                for (int j = 0; j < numOfPipes; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                // printf("//%s//\n", pipeCmds[i]);

                if (redirectionDetector(pipeCmds[i]))
                {
                    redirectionExecute(pipeCmds[i], 1);
                }

                else
                {
                    commandIdentifier(pipeCmds[i], 1, 0);
                }

                exit(EXIT_SUCCESS);
            }
        }

        int rc = fork();
        if (rc == 0)
        {
            char lastCmd[4096];
            strncpy(lastCmd, pipeCmds[numOfPipes], 4096);
            dup2(pipes[numOfPipes - 1][0], STDIN_FILENO);
            for (int j = 0; j < numOfPipes; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
                if (redirectionDetector(lastCmd))
                {
                    redirectionExecute(lastCmd, 1);
                }

                else
                {
                    commandIdentifier(lastCmd, 1, 0);
                }

                exit(EXIT_SUCCESS);
            }
        }

        for (int l = 0; l < numOfPipes; l++)
        {
            // printf("hereBoy\n");
            close(pipes[l][0]);
            close(pipes[l][1]);
        }

        char *qca[2];
        char qck[4096];
        strncpy(qck, pipeCmds[numOfPipes], 4096);

        char *tk = strtok(qck, " \t");
        qca[0] = tk;

        addEntryToProcTable(rc, qca[0], pipeBgFlag);
        // displayCurrentProcTable();
        // printf("pipe Bg Flag = %d\n", pipeBgFlag);
        if (pipeBgFlag)
        {

            for (int m = 0; m < numOfPipes; m++)
            {

                wait(NULL);
            }

            tcsetpgrp(shell_terminal, shell_pgid);
        }

        else
        {
            for (int m = 0; m < numOfPipes + 1; m++)
            {

                wait(NULL);
            }
        }
    }
}
