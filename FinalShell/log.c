#include "log.h"
#include "parser.h"
int commandCount;

int countNumberOfLines()
{
    FILE *fptr;
    fptr = fopen("logFile.txt", "r+");
    int count = 0;
    char ch;
    while ((ch = fgetc(fptr)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    commandCount = count;
    fclose(fptr);
}

int duplicateCommand(char *fname, char *inp)
{
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    char last_line[1024] = {0}; // Adjust size as needed

    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    rewind(file);
    // printf("Count = %d\n", count);
    int count2 = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        if (count2 == count - 1)
        {
            fseek(file, -1, SEEK_CUR);
            break;
        }
        if (ch == '\n')
        {
            count2++;
        }
    }

    // printf("Count2 = %d\n", count);

    fgets(last_line, sizeof(last_line), file);

    last_line[strlen(last_line) - 1] = '\0';
    fclose(file);
    // printf("last line :%-s inp = %-s inp == lastline? : %d\n", last_line, inp, strcmp(last_line, inp));
    // prin
    if (strcmp(last_line, inp) == 0)
    {
        // printf("Duplicate entry detected\n");
        return 1;
    }

    return 0;
}

int isValidWithLog(char c)
{
    return (c == ' ' || c == '&' || c == ';');
}

int isLogCmd(const char *inp)
{
    const char *log_pos = strstr(inp, "log");

    while (log_pos != NULL)
    {
        if ((log_pos == inp || isValidWithLog(*(log_pos - 1))) &&
            (*(log_pos + 3) == '\0' || isValidWithLog(*(log_pos + 3))))
        {
            return 1;
        }

        log_pos = strstr(log_pos + 3, "log");
    }

    return 0;
}

int addLog(char *fname, char *inp)
{
    FILE *file = fopen(fname, "r+");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    commandCount = count;
    if (duplicateCommand(fname, inp) == 1 || isLogCmd(inp))
    {
        // printf("Victory ROyale!");
        fclose(file);
        return 0;
    }

    // printf("Hi I am in addLog :) UwU\n");

    if (count >= 15)
    {
        rewind(file);
        FILE *temp = tmpfile();
        if (temp == NULL)
        {
            perror("Error creating temporary file");
            fclose(file);
            return -1;
        }

        int line_removed = 0;
        while ((ch = fgetc(file)) != EOF)
        {
            if (!line_removed)
            {
                if (ch == '\n')
                {
                    line_removed = 1;
                }
            }
            else
            {
                fputc(ch, temp);
            }
        }

        freopen(fname, "w", file);
        rewind(temp);
        while ((ch = fgetc(temp)) != EOF)
        {
            fputc(ch, file);
        }

        fclose(temp);
    }

    fseek(file, 0, SEEK_END);
    if (fprintf(file, "%s\n", inp) < 0)
    {
        perror("Error writing to file");
        fclose(file);
        return -1;
    }

    fclose(file);
}

void displayLog()
{
    char theShellFolder[] = "/FinalShell";
    char logAddress[4096];
    strcpy(logAddress, homeDirectory);
    strcat(logAddress, theShellFolder);
    strcat(logAddress, "/logFile.txt");

    FILE *fptr;
    fptr = fopen(logAddress, "r+");
    int t = commandCount;
    char string[1024];
    while (t--)
    {

        fgets(string, 1024, fptr);
        printf("%-s\n", string);
    }

    fclose(fptr);
}

void purgeLog()
{
    // printf("was in Purge\n");
    fflush(stdout);
    FILE *fptr;
    char theShellFolder[] = "/FinalShell";
    char logAddress[4096];
    strcpy(logAddress, homeDirectory);
    strcat(logAddress, theShellFolder);
    strcat(logAddress, "/logFile.txt");
    fptr = fopen(logAddress, "w");
    fclose(fptr);
}

void execLog(int index, int isPipe)
{
    int cmdIndex = commandCount - index + 1;
    // printf("cmd Index : %d\n", cmdIndex);
    FILE *fptr;
    char theShellFolder[] = "/FinalShell";
    char logAddress[4096];
    strcpy(logAddress, homeDirectory);
    strcat(logAddress, theShellFolder);
    strcat(logAddress, "/logFile.txt");
    fptr = fopen(logAddress, "r");
    char ch;
    int count = 0;

    while ((ch = fgetc(fptr)) != EOF)
    {
        // printf(". ");
        if (count == cmdIndex - 1)
        {
            // printf("<?>\n");

            fseek(fptr, -1, SEEK_CUR);
            break;
        }
        else if (ch == '\n')
        {
            // printf("\n");
            count++;
        }
    }

    char string[1024];
    fgets(string, 1024, fptr);
    fclose(fptr);
    string[strlen(string) - 1] = '\0';
    // printf("()%-s()\n", string);
    char *stringTokenized[256];
    char *currToken = strtok(string, ";");
    int i = 0;
    while (currToken != NULL)
    {
        stringTokenized[i] = currToken;
        currToken = strtok(NULL, ";");
        i++;
    }

    int t = 0;
    char *updatedStringArr[4096];
    int updatedStringArrLength = 0;
    while (t < i)
    {
        updatedStringArrLength = ampercentDetector(updatedStringArr, stringTokenized[t], updatedStringArrLength);
        t++;
    }

    t = 0;

    while (t < updatedStringArrLength)
    {
        updatedStringArr[t][strlen(updatedStringArr[t])] = '\0';
        commandIdentifier(updatedStringArr[t], isPipe, 0);
        t++;
    }
}