#include "neonate.h"
#include "ping.h"
#define BUFFER_SIZE 4096

// Function to get the most recently created or modified process's PID based on modification time


int isValidNeonateString(char *str[], int numOfArgs)
{
    for (int i = 0; i < numOfArgs; i++)
    {
        // printf("%d  [%s]\n", i, str[i]);
    }
    

    if (numOfArgs != 3 || strcmp(str[1], "-n") != 0 || !isValidInt(str[2]))
    {
        return 0;
    }
    return 1;
    
}

int get_most_recent_pid()
{
    DIR *proc = opendir("/proc");
    if (!proc)
    {
        perror("opendir");
        exit(1);
    }

    struct dirent *entry;
    int most_recent_pid = -1;
    time_t most_recent_time = 0;

    while ((entry = readdir(proc)) != NULL)
    {
        // Check if the entry is a valid PID (numeric directory name)
        if (isdigit(entry->d_name[0]))
        {
            char path[BUFFER_SIZE];
            snprintf(path, sizeof(path), "/proc/%s", entry->d_name);

            struct stat file_stat;
            if (stat(path, &file_stat) == 0)
            {
                // Compare modification time of the process directory
                if (file_stat.st_mtime > most_recent_time)
                {
                    most_recent_time = file_stat.st_mtime;
                    most_recent_pid = atoi(entry->d_name);
                }
            }
        }
    }

    closedir(proc);
    return most_recent_pid;
}

// Function to check if 'x' is pressed without waiting for Enter
int check_for_exit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Get terminal attributes and disable buffering
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Make stdin non-blocking
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Restore terminal attributes and buffering
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Return if 'x' is pressed
    if (ch != EOF && ch == 'x')
    {
        return 1;
    }

    return 0;
}

// Function to monitor the most recent process's PID every `interval` seconds
void monitor_recent_pid(int interval)
{
    while (1)
    {
        int recent_pid = get_most_recent_pid();
        printf("%d\n", recent_pid);

        for (int i = 0; i < interval * 10; i++)
        {                   // 0.1 second resolution
            usleep(100000); // Sleep for 0.1 seconds
            if (check_for_exit())
            {
                printf("\nKey 'x' pressed, exiting neonate...\n");
                return;
            }
        }
    }
}
