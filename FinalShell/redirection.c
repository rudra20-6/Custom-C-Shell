#include "redirection.h"
#include "systemCommands.h"

char *mergeStrings(char *arr[], int size)
{
    // Calculate total length of all strings
    int totalLength = 0;
    for (int i = 0; i < size; i++)
    {
        totalLength += strlen(arr[i]);
    }

    // Allocate memory for the merged string (+1 for the null terminator)
    char *mergedString = (char *)malloc(totalLength + 1);
    if (!mergedString)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Copy strings one by one into the mergedString
    mergedString[0] = '\0'; // Initialize as an empty string
    for (int i = 0; i < size; i++)
    {
        strcat(mergedString, arr[i]);
    }

    return mergedString;
}

void trim(char *str)
{
    char *start = str;
    char *end;

    // Remove leading spaces
    while (isspace((unsigned char)*start))
    {
        start++;
    }

    // If all spaces or empty, return an empty string
    if (*start == '\0')
    {
        str[0] = '\0';
        return;
    }

    // Find the end of the string
    end = start + strlen(start) - 1;

    // Remove trailing spaces
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    // Shift the trimmed string to the original pointer
    memmove(str, start, end - start + 2);
}

int redirectionDetector(char *theCmd)
{
    char *ptr = strstr(theCmd, "<");
    char *ptr2 = strstr(theCmd, ">");
    char *ptr3 = strstr(theCmd, ">>");

    if (ptr != NULL || ptr2 != NULL || ptr3 != NULL)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

int isBgProcess(char *theCmd)
{
    // printf("theCmd = %s\n", theCmd);
    // char *ptr = strstr(theCmd, '&');
    if (strstr(theCmd, "&")!= NULL)
    {
        // printf("&hai\n");
        if (theCmd[strlen(theCmd) - 1] == '&')
        {
            return 1;
        }

        else
        {
            return 2;
        }
    }

    return 0;
}

void redirectionExecute(char *theCmd, int isPipe)
{
    check_bg_processes();
    int stdcopy = dup(STDIN_FILENO);
    int fd;

    char *ptr = strstr(theCmd, "<");
    char *ptr2 = strstr(theCmd, ">");
    char *ptr3 = strstr(theCmd, ">>");
    char copyOfCmd[4096];
    char copyOfCmd2[4096];

    strncpy(copyOfCmd, theCmd, 4096);
    strncpy(copyOfCmd2, theCmd, 4096);

    trim(theCmd);
    int bgStatus = isBgProcess(copyOfCmd);
    // printf("BGSTATUS = %d\n", bgStatus);
    if (bgStatus == 2)
    {
        printf("Invalid use of & with redirection\n");
        return;
    }
    
    int fptr;
    char *mergeArr[256];
    int i = 0;
    int inpRedFlag = 0;
    int outRedFlag = 0;
    if (ptr != NULL)
    {
        inpRedFlag = 1;
        char *tok1 = strtok(copyOfCmd, "<");
        char *arg1 = tok1;
        char *tok2 = strtok(NULL, " \t");
        char *arg2 = tok2;
        char *tok3 = strtok(NULL, " \t");
        char *rest = tok3;
        // inputFromFile(arg1, arg2, mergeArr, i);
        fd = open(arg2, O_RDONLY, 0644);
        if (fd < 0)
        {
            printf("File %-s Does Not Exist!\n", arg2);
            return;
        }

        int stdcopy = dup(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        mergeArr[0] = arg1;
        i += 1;
    }

    // wc < newfile.txt > a.txt
    // echo "dolor sit amet" >> newfile.txt
    // wc < a.txt
    if (ptr2 != NULL || ptr3 != NULL)
    {
        char *tk1, *tk2;
        char *a1, *a2;
        outRedFlag = 1;
        if (ptr2 != NULL && ptr3 == NULL)
        {

            tk1 = strtok(copyOfCmd2, ">");
            a1 = tk1;
            tk2 = strtok(NULL, " \t");
            a2 = tk2;
            // printf("'%s'\n", a1);
            fptr = open(a2, O_CREAT | O_RDWR | O_TRUNC, 0644);
            // outputToFile(mergeArr, i, arg1, 0);
            // i += 2;
        }

        else if (ptr3 != NULL)
        {
            // printf("CHeck!\n");

            tk1 = strtok(copyOfCmd2, ">>");
            a1 = tk1;
            tk2 = strtok(NULL, ">");
            // tk2 = strtok(NULL, "");
            a2 = tk2;
            // printf("'%s'\n", a2);
            trim(a2);
            // printf("'%s'\n", a2);

            fptr = open(a2, O_CREAT | O_RDWR | O_APPEND, 0644);
        }

        if (!inpRedFlag)
        {
            mergeArr[i++] = a1;
        }

        if (fptr < 0)
        {
            printf("Unable to Open/Create File!\n");
            return;
        }

        if (bgStatus == 1)
        {
            mergeArr[i++] = " &";
        }
        
        char *cmd = mergeStrings(mergeArr, i);

        int stdoutCopy = dup(STDOUT_FILENO);
        dup2(fptr, STDOUT_FILENO);
        commandIdentifier(cmd, isPipe, 1);
        free(cmd);
        dup2(stdoutCopy, STDOUT_FILENO);

        if (inpRedFlag)
        {
            dup2(stdcopy, STDIN_FILENO);
        }
    }

    else if (!outRedFlag && inpRedFlag)
    {
        if (bgStatus == 1)
        {
            mergeArr[i++] = " &";
        }

        char *final = mergeStrings(mergeArr, i);
        // printf("Final string in redirectio input only [%s]\n", final);
        commandIdentifier(final, isPipe, 1);
        dup2(stdcopy, STDIN_FILENO);
        close(fd);
        free(final);
    }
}

void outputToFile(char *args[], int numOfArgs, char filename[], int overWriteFlag)
{
    int fptr;
    if (overWriteFlag)
    {
        fptr = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    }

    else
    {
        fptr = open(filename, O_CREAT | O_RDWR | O_APPEND, 0644);
    }

    if (fptr < 0)
    {
        printf("Unable to Open/Create File!\n");
        return;
    }

    int stdoutCopy = dup(STDOUT_FILENO);
    dup2(fptr, STDOUT_FILENO);
}

void inputFromFile(char cmd[], char fname[], char *ma[], int ctr)
{
    int fd = open(fname, O_RDONLY, 0644);
    if (fd < 0)
    {
        printf("File %-s Does Not Exist!\n", fname);
        return;
    }

    int stdcopy = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    char thefile[4096];
    // scanf(" %[^\n]s", thefile);
    dup2(stdcopy, STDIN_FILENO);
    ma[ctr++] = cmd;
    ma[ctr++] = thefile;
}