#include "proclore.h"
#include "systemCommands.h"

int doesPIDexist(int pid)
{
    if (kill(pid, 0) == -1)
    {
        if (errno == ESRCH)
        {
            // printf("Process with PID %d does not exist.\n", pid);
        }
        else if (errno == EPERM)
        {
            // printf("No permission to check process with PID %d.\n", pid);
        }
        else
        {
            perror("Failed to check PID");
        }
        return 0;
    }

    return 1;
}

void displayData(int pid)
{

    char path[256];
    char buffer[1024];
    FILE *file;

    if (kill(pid, 0) == -1)
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
        return;
    }

    printf("PID: %d\n", pid);

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    file = fopen(path, "r");
    if (file)
    {
        char state;
        int pgrp, tpgid;
        fscanf(file, "%*d %*s %c %*d %d %*d %*d %d", &state, &pgrp, &tpgid);

        char *state_description;
        if (state == 'R')
        {
            state_description = (pgrp == tpgid) ? "R+" : "R";
        }
        else if (state == 'S')
        {
            state_description = (pgrp == tpgid) ? "S+" : "S";
        }
        else if (state == 'Z')
        {
            state_description = "Z";
        }
        else
        {
            state_description = "Unknown";
        }

        printf("Process Status: %s\n", state_description);
        printf("Process Group ID: %d\n", pgrp);
        fclose(file);
    }
    else
    {
        perror("Failed to open /proc/[pid]/stat");
        return;
    }


    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    file = fopen(path, "r");
    if (file)
    {
        unsigned long vsize;
        fscanf(file, "%lu", &vsize);
        printf("Virtual Memory Size: %lu bytes\n", vsize * getpagesize());
        fclose(file);
    }
    else
    {
        perror("Failed to open /proc/[pid]/statm");
        return;
    }


    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
        printf("Executable Path: %s\n", buffer);
    }
    else
    {
        perror("Failed to read /proc/[pid]/exe");
        return;
    }
}