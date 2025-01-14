#include "seek.h"

#define PATH_MAX 4096

void seek(char *dirToOpen, char *key, int flagD, int flagF, int flagE, int *totalFileCount, int *totalDirCount, char foundFile[], char foundDir[])
{
    struct dirent *dir;
    DIR *obj = opendir(dirToOpen);
    if (!obj)
    {
        perror("opendir");
        return;
    }

    while ((dir = readdir(obj)) != NULL)
    {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dirToOpen, dir->d_name);

        char *name = dir->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        {
            continue;
        }

        struct stat statInfo;
        if (stat(path, &statInfo) != 0)
        {

            continue;
        }

        int isDirectory = S_ISDIR(statInfo.st_mode);
        int foundKey = (strstr(name, key) == name || (name[0] == '.' && strstr(name, key) == name + 1));

        if (foundKey)
        {
            if (isDirectory && !flagF)
            {
                printf("\033[1;34m%s\033[0m\n", path);
                (*totalDirCount)++;
                strcpy(foundDir, path);
            }
            else if (!isDirectory && !flagD)
            {
                printf("\033[1;32m%s\033[0m\n", path);
                (*totalFileCount)++;
                strcpy(foundFile, path);
            }
        }

        if (isDirectory)
        {
            seek(path, key, flagD, flagF, flagE, totalFileCount, totalDirCount, foundFile, foundDir);
        }
    }

    // printf("1\n");

    closedir(obj);
}

void handle_e_flag(int totalFileCount, int totalDirCount, char foundFile[], char foundDir[])
{
    if (totalFileCount == 1 && totalDirCount == 0)
    {
        if (access(foundFile, R_OK) == 0)
        {
            FILE *fptr = fopen(foundFile, "r");
            if (fptr != NULL)
            {
                char line[256];
                while (fgets(line, sizeof(line), fptr))
                {
                    printf("%s", line);
                }
                fclose(fptr);
            }
            else
            {
                printf("Unable to open file: %s\n", foundFile);
            }
        }
        else
        {
            printf("Missing permissions for task!\n");
        }
    }
    else if (totalDirCount == 1 && totalFileCount == 0)
    {
        if (access(foundDir, X_OK) == 0)
        {
            if (chdir(foundDir) != 0)
            {
                perror("chdir");
            }
        }
        else
        {
            printf("Missing permissions for task!\n");
        }
    }
}