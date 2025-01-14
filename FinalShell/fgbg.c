#include "fgbg.h"
#include "proclore.h"

void handleFg(int pid)
{
    
    if (doesPIDexist(pid))
    {

        changeToFg(pid);
        // printf("The pid does exist\n");
        printf("Bringing process %d to foreground\n", pid);
        // Bring process to foreground
        if (tcsetpgrp(shell_terminal, tcgetpgrp(pid)) == -1)
        {
            // perror("tcsetpgrp");
        } // Set terminal control to process

        // If process was stopped, resume it

        if (kill(pid, SIGCONT) == -1)
        {
            perror("kill");
        }

        // Wait for the process to finish
        int status;
        waitpid(pid, &status, WUNTRACED);

        // After the process completes, return control to shell
        if(tcsetpgrp(shell_terminal, shell_pgid))
        {
            // perror("tcsetpgrp postFG");
        }
    }

    else
    {
        printf("No such process found\n");
    }
}

void handleBg(int pid)
{
    
    if (doesPIDexist(pid))
    {

        changeToBg(pid);

        kill(pid, SIGCONT);
        printf("Process %d resumed in background\n", pid);
    }

    else
    {
        printf("No such process found\n");
    }
}