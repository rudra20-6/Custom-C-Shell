#include "systemCommands.h"
#include "activities.h"

void printActivities()
{
    struct bgProcessData bgProcArr[4096];
    int numOfProcesses = copyTheBgProcessArray(bgProcArr);

    int ctr = 0;
    while (ctr < numOfProcesses)
    {
        char path[256];
        char buffer[1024];
        FILE *file;

        snprintf(path, sizeof(path), "/proc/%d/stat", bgProcArr[ctr].pid);
        file = fopen(path, "r");

        if (file)
        {
            char state;
            int pgrp, tpgid;
            fscanf(file, "%*d %*s %c %*d %d %*d %*d %d", &state, &pgrp, &tpgid);

            char *state_description;
            if (state == 'R' || state == 'S' || state == 'Z' || state == 'D' )
            {
                state_description = "Running";
            }

            else if (state == 'T')
            {
                state_description = "Stopped";
            }

            printf("%d : %s - %s\n", bgProcArr[ctr].pid, bgProcArr[ctr].cmd, state_description);
            fclose(file);
            // printf("%d : %s - %s", bgProcArr[ctr].pid, bgProcArr[ctr].cmd);
        }

        ctr++;
    }
}