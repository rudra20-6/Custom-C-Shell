
#include "parser.h"
#include "hop.h"

extern char homeDirectory[4096];
char oldWD[4096] = "";

void hop(char arg[])
{
    char cwd[4096];
    int bsFlag = 0;
    getcwd(cwd, 4096);

    if (strcmp(arg, "-") == 0)
    {
        if (strlen(oldWD) == 0)
        {

            printf("\nNo previous working directory found!!\n");

            return;
        }

        else
        {
            chdir(oldWD);
        }

        bsFlag = 1;
    }
    strcpy(oldWD, cwd);

    if (strcmp(arg, "..") == 0)
    {
        chdir("..");
    }
    else if (strcmp(arg, "~") == 0)
    {
        chdir(homeDirectory);
    }

    else if (!bsFlag)
    {
        char reqPath[4096];
        strcpy(reqPath, arg);

        if (arg[0] == '~')
        {
            replaceSubstring(reqPath, "~", homeDirectory);
        }

        if (chdir(reqPath) != 0)
        {

            printf("\nInvalid Directory Path!!\n");
            return;
        }
    }

    getcwd(cwd, 4096);
    printf(YEL);
    printf("\n%s\n", cwd);
    printf(RESET);
}
