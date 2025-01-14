
#include "parser.h"
#include "reveal.h"

struct allInfo
{
    char *p_name;
    char perm[11];
    int num_of_link;
    char *user;
    char *group;
    int bsize;
    char time[14];
    int fde;
};

int compare(const void *a, const void *b)
{
    struct allInfo *infoA = (struct allInfo *)a;
    struct allInfo *infoB = (struct allInfo *)b;
    return strcmp(infoA->p_name, infoB->p_name);
}

int whichFlagisOn(char *args[], int numOfArgs)
{

    // checking last argument is flag or directory
    if (args[numOfArgs - 1][0] != '-')
    {
        numOfArgs--;
    }

    int t = 1;
    int flagA = 0;
    int flagL = 0;
    int flagInvalid = 0;
    // printf("numofargs = %d\n", numOfArgs);
    while (t < numOfArgs)
    {
        if (args[t][0] != '-')
        {
            // printf("Out from 1\n");
            flagInvalid = 1; // invalid flag
            return 0;
        }

        for (int i = 1; i < strlen(args[t]); i++)
        {
            if (args[t][i] != 'a' && args[t][i] != 'l')
            {
                // printf("outfrom2\n");
                return 0; // Invalid character found
            }

            if (args[t][i] == 'l')
            {
                flagL = 1;
            }

            if (args[t][i] == 'a')
            {
                flagA = 1;
            }
        }

        t++;
    }

    if (flagA && !flagL)
    {
        return 1; // only flag A
    }

    else if (flagL && !flagA)
    {
        return 2; // only flag L
    }

    else if (flagA && flagL)
    {
        return 3;
    }

    else
    {
        return 4;
    }
}

void reveal(char *revArgs[], int numOfRevArgs)
{
    // int x = 0;

    // while (x < numOfRevArgs)
    // {
    //    printf("%-s\t", revArgs[x++]);
    // }

    struct allInfo laPrintData[256];
    char currwd[4096];
    getcwd(currwd, 4096);
    DIR *obj;

    if (numOfRevArgs <= 2)
    {
        if (revArgs[1][0] == '-' || numOfRevArgs == 1)
        {
            obj = opendir(".");
            if (chdir(".") == -1)
            {
                perror("Chdir Error: ");
                printf("\n");
                return;
            }
        }
        else
        {
            replaceSubstring(revArgs[1], "~", homeDirectory);
            obj = opendir(revArgs[1]);
            if (chdir(revArgs[1]) == -1)
            {
                perror("Chdir Error");
                printf("\n");
                return;
            }
        }
    }
    else
    {
        if (revArgs[2][0] != '-')
        {
            replaceSubstring(revArgs[2], "~", homeDirectory);
            obj = opendir(revArgs[2]);
            if (chdir(revArgs[2]) == -1)
            {
                perror("Chdir Error");
                // printf("1\n");
                return;
            }
        }

        else
        {
            if (numOfRevArgs == 3)
            {
                obj = opendir(".");
                if (chdir(".") == -1)
                {
                    perror("Chdir Error");
                    // printf("2\n");
                    return;
                }
            }

            else
            {
                replaceSubstring(revArgs[numOfRevArgs - 1], "~", homeDirectory);
                obj = opendir(revArgs[numOfRevArgs - 1]);
                if (chdir(revArgs[numOfRevArgs - 1]) == -1)
                {
                    printf("\nInvalid Arguments for Hop !!\n");
                    // printf("2\n");
                    return;
                }
            }
        }
    }

    if (!obj)
    {
        perror("ERROR: ");
        printf("\n");
        return;
    }

    struct dirent *tempdir;
    struct stat fs[256];
    int l = 0;
    int countOfEntries = 0;

    while (tempdir = readdir(obj))
    {
        countOfEntries++;
        struct group *grp;
        struct passwd *pwd;
        stat(tempdir->d_name, &fs[l]);

        pwd = getpwuid(fs[l].st_uid);
        grp = getgrgid(fs[l].st_gid);

        char *time_t = ctime(&fs[l].st_mtime);
        strncpy(laPrintData[l].time, time_t + 4, 12);

        laPrintData[l].p_name = strdup(tempdir->d_name);
        laPrintData[l].user = strdup(pwd->pw_name);
        laPrintData[l].group = strdup(grp->gr_name);
        laPrintData[l].num_of_link = fs[l].st_nlink;
        laPrintData[l].bsize = fs[l].st_size;

        if (S_ISDIR(fs[l].st_mode))
        {
            laPrintData[l].fde = 1;
        }
        else if (S_ISREG(fs[l].st_mode))
        {
            if ((fs[l].st_mode & S_IXUSR) || (fs[l].st_mode & S_IXGRP) || (fs[l].st_mode & S_IXOTH))
            {
                laPrintData[l].fde = 2;
            }
            else
            {
                laPrintData[l].fde = 0;
            }
        }

        strcpy(laPrintData[l].perm, "");
        strcat(laPrintData[l].perm, (S_ISDIR(fs[l].st_mode)) ? "d" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IRUSR) ? "r" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IWUSR) ? "w" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IXUSR) ? "x" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IRGRP) ? "r" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IWGRP) ? "w" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IXGRP) ? "x" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IROTH) ? "r" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IWOTH) ? "w" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IXOTH) ? "x" : "-");

        l++;
    }

    chdir(currwd);

    // Sort laPrintData by p_name
    qsort(laPrintData, l, sizeof(struct allInfo), compare);

    if (numOfRevArgs < 2)
    {
        printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
        int _ctr = 0;
        while (_ctr < l)
        {
            if (laPrintData[_ctr].p_name[0] != '.')
            {
                if (laPrintData[_ctr].fde == 0)
                {
                    printf(WHT);
                }

                else if (laPrintData[_ctr].fde == 1)
                {
                    printf(BBLU);
                }

                else if (laPrintData[_ctr].fde == 2)
                {
                    printf(GRN);
                }

                printf("%-s\n", laPrintData[_ctr].p_name);
            }
            _ctr++;
        }
    }
    else if (numOfRevArgs == 2 && revArgs[1][0] != '-')
    {
        printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
        int _ctr = 0;
        while (_ctr < l)
        {
            if (laPrintData[_ctr].fde == 0)
            {
                printf(WHT);
            }

            else if (laPrintData[_ctr].fde == 1)
            {
                printf(BBLU);
            }

            else if (laPrintData[_ctr].fde == 2)
            {
                printf(GRN);
            }
            if (laPrintData[_ctr].p_name[0] != '.')
            {
                printf("%-s\n", laPrintData[_ctr].p_name);
                printf(RESET);
            }
            _ctr++;
        }
    }
    else
    {
        if (strstr(revArgs[1], "-l") != NULL || strstr(revArgs[1], "-a") != NULL)
        {
            if (strstr(revArgs[1], "a") == NULL && revArgs[2][0] != '-')
            {
                printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
                // printf("\nOnly -l\n");
                int _ctr = 0;
                while (_ctr < l)
                {

                    if (laPrintData[_ctr].p_name[0] != '.')
                    {
                        printf("%-s\t%-d\t%-s\t%-s\t%-d\t\t%-s\t", laPrintData[_ctr].perm, laPrintData[_ctr].num_of_link, laPrintData[_ctr].user, laPrintData[_ctr].group, laPrintData[_ctr].bsize, laPrintData[_ctr].time);
                        if (laPrintData[_ctr].fde == 0)
                        {
                            printf(WHT);
                        }

                        else if (laPrintData[_ctr].fde == 1)
                        {
                            printf(BBLU);
                        }

                        else if (laPrintData[_ctr].fde == 2)
                        {
                            printf(GRN);
                        }

                        printf("%-s\n", laPrintData[_ctr].p_name);
                        printf(RESET);
                    }
                    _ctr++;
                }
            }
            else if (strstr(revArgs[1], "l") == NULL && revArgs[2][0] != '-')
            {
                printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
                // printf("\nOnly -a\n");
                int _ctr = 0;
                while (_ctr < l)
                {
                    if (laPrintData[_ctr].fde == 0)
                    {
                        printf(WHT);
                    }

                    else if (laPrintData[_ctr].fde == 1)
                    {
                        printf(BBLU);
                    }

                    else if (laPrintData[_ctr].fde == 2)
                    {
                        printf(GRN);
                    }
                    printf("%-s\n", laPrintData[_ctr++].p_name);
                    printf(RESET);
                }
            }
            else
            {
                if (!((!strcmp(revArgs[2], "-a")) || (!strcmp(revArgs[2], "-l"))))
                {
                    printf("\nInvalid Flags For Hop!!\n");
                    return;
                }

                int t = 0;
                printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
                while (t < l)
                {

                    printf("%-s\t%-d\t%-s\t%-s\t%-d\t\t%-s\t", laPrintData[t].perm, laPrintData[t].num_of_link, laPrintData[t].user, laPrintData[t].group, laPrintData[t].bsize, laPrintData[t].time);
                    if (laPrintData[t].fde == 0)
                    {
                        printf(WHT);
                    }

                    else if (laPrintData[t].fde == 1)
                    {
                        printf(BBLU);
                    }

                    else if (laPrintData[t].fde == 2)
                    {
                        printf(GRN);
                    }
                    printf("%-s\n", laPrintData[t].p_name);
                    printf(RESET);
                    t++;
                }
            }
        }

        else
        {
            printf("\nInvalid Flags for Hop!!\n");
        }
    }

    for (int i = 0; i < l; i++)
    {
        free(laPrintData[i].p_name);
        free(laPrintData[i].user);
        free(laPrintData[i].group);
    }
}

int isAddress(char *args[], int numOfArgs)
{
    // printf("the last arg : %-s\t\t and first char: %c\n", args[numOfArgs -1], args[numOfArgs-1][0] );
    if (args[numOfArgs - 1][0] != '-' && numOfArgs > 1)
    {

        return 1;
    }

    return 0;
}

void revealM2(char *revArgs[], int numOfRevArgs)
{

    struct allInfo laPrintData[256];
    char currwd[4096];
    getcwd(currwd, 4096);
    DIR *obj;

    int isItAddress = isAddress(revArgs, numOfRevArgs);
    // printf("Is it address? : %d\n", isItAddress);
    if (isItAddress)
    {
        replaceSubstring(revArgs[numOfRevArgs - 1], "~", homeDirectory);
        obj = opendir(revArgs[numOfRevArgs - 1]);
        if (chdir(revArgs[numOfRevArgs - 1]) == -1)
        {
            perror("Chdir Error");
            printf("\n");
            return;
        }
    }

    else
    {
        obj = opendir(".");
        if (chdir(".") == -1)
        {
            perror("Chdir Error");
            printf("\n");
            return;
        }
    }

    if (!obj)
    {
        perror("ERROR: ");
        printf("\n");
        return;
    }

    struct dirent *tempdir;
    struct stat fs[256];
    int l = 0;
    int countOfEntries = 0;

    while (tempdir = readdir(obj))
    {
        countOfEntries++;
        struct group *grp;
        struct passwd *pwd;
        stat(tempdir->d_name, &fs[l]);

        pwd = getpwuid(fs[l].st_uid);
        grp = getgrgid(fs[l].st_gid);

        char *time_t = ctime(&fs[l].st_mtime);
        strncpy(laPrintData[l].time, time_t + 4, 12);

        laPrintData[l].p_name = strdup(tempdir->d_name);
        laPrintData[l].user = strdup(pwd->pw_name);
        laPrintData[l].group = strdup(grp->gr_name);
        laPrintData[l].num_of_link = fs[l].st_nlink;
        laPrintData[l].bsize = fs[l].st_size;

        if (S_ISDIR(fs[l].st_mode))
        {
            laPrintData[l].fde = 1;
        }
        else if (S_ISREG(fs[l].st_mode))
        {
            if ((fs[l].st_mode & S_IXUSR) || (fs[l].st_mode & S_IXGRP) || (fs[l].st_mode & S_IXOTH))
            {
                laPrintData[l].fde = 2;
            }
            else
            {
                laPrintData[l].fde = 0;
            }
        }

        strcpy(laPrintData[l].perm, "");
        strcat(laPrintData[l].perm, (S_ISDIR(fs[l].st_mode)) ? "d" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IRUSR) ? "r" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IWUSR) ? "w" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IXUSR) ? "x" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IRGRP) ? "r" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IWGRP) ? "w" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IXGRP) ? "x" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IROTH) ? "r" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IWOTH) ? "w" : "-");
        strcat(laPrintData[l].perm, (fs[l].st_mode & S_IXOTH) ? "x" : "-");

        l++;
    }

    chdir(currwd);

    // Sort laPrintData by p_name
    qsort(laPrintData, l, sizeof(struct allInfo), compare);

    int flagDecider = whichFlagisOn(revArgs, numOfRevArgs);
    // printf("***Printing the flags***\n");
    // printf("theFlagReturnedIs = %d", flagDecider);
    if (flagDecider == 0)
    {
        printf("Invalid Arguments for Hop!\n");
        return;
    }

    else
    {
        if (flagDecider == 1) // only -a
        {

            printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
            // printf("\nOnly -a\n");
            int _ctr = 0;
            while (_ctr < l)
            {
                if (laPrintData[_ctr].fde == 0)
                {
                    printf(WHT);
                }

                else if (laPrintData[_ctr].fde == 1)
                {
                    printf(BBLU);
                }

                else if (laPrintData[_ctr].fde == 2)
                {
                    printf(GRN);
                }
                printf("%-s\n", laPrintData[_ctr++].p_name);
                printf(RESET);
            }
        }

        else if (flagDecider == 2) // only -l
        {
            printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
            // printf("\nOnly -l\n");
            int _ctr = 0;
            while (_ctr < l)
            {

                if (laPrintData[_ctr].p_name[0] != '.')
                {
                    printf("%-s\t%-d\t%-s\t%-s\t%-d\t\t%-s\t", laPrintData[_ctr].perm, laPrintData[_ctr].num_of_link, laPrintData[_ctr].user, laPrintData[_ctr].group, laPrintData[_ctr].bsize, laPrintData[_ctr].time);
                    if (laPrintData[_ctr].fde == 0)
                    {
                        printf(WHT);
                    }

                    else if (laPrintData[_ctr].fde == 1)
                    {
                        printf(BBLU);
                    }

                    else if (laPrintData[_ctr].fde == 2)
                    {
                        printf(GRN);
                    }

                    printf("%-s\n", laPrintData[_ctr].p_name);
                    printf(RESET);
                }
                _ctr++;
            }
        }

        else if (flagDecider == 3) // both -l and -a
        {
            int t = 0;
            printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
            while (t < l)
            {

                printf("%-s\t%-d\t%-s\t%-s\t%-d\t\t%-s\t", laPrintData[t].perm, laPrintData[t].num_of_link, laPrintData[t].user, laPrintData[t].group, laPrintData[t].bsize, laPrintData[t].time);
                if (laPrintData[t].fde == 0)
                {
                    printf(WHT);
                }

                else if (laPrintData[t].fde == 1)
                {
                    printf(BBLU);
                }

                else if (laPrintData[t].fde == 2)
                {
                    printf(GRN);
                }
                printf("%-s\n", laPrintData[t].p_name);
                printf(RESET);
                t++;
            }
        }

        else if (flagDecider == 4) // No flags
        {
            printf("\n%stotal %d%s\n", MAG, countOfEntries, RESET);
            int _ctr = 0;
            while (_ctr < l)
            {
                if (laPrintData[_ctr].fde == 0)
                {
                    printf(WHT);
                }

                else if (laPrintData[_ctr].fde == 1)
                {
                    printf(BBLU);
                }

                else if (laPrintData[_ctr].fde == 2)
                {
                    printf(GRN);
                }
                if (laPrintData[_ctr].p_name[0] != '.')
                {
                    printf("%-s\n", laPrintData[_ctr].p_name);
                    printf(RESET);
                }
                _ctr++;
            }
        }
    }


    for (int i = 0; i < l; i++)
    {
        free(laPrintData[i].p_name);
        free(laPrintData[i].user);
        free(laPrintData[i].group);
    }
}
