#include "ping.h"

void ping_original(char *command)
{
    char *token = strtok(command, " ");
    token = strtok(NULL, " "); 

    if (token == NULL)
    {
        fprintf(stderr, "Usage: ping <pid> <signal_number>\n");
        return;
    }

    pid_t pid = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        fprintf(stderr, "Usage: ping <pid> <signal_number>\n");
        return;
    }

    int signal_number = atoi(token) % 32;

    if (kill(pid, signal_number) == -1)
    {
        perror("No such process found");
    }
    else
    {
        printf("Signal %d sent to process %d\n", signal_number, pid);
    }
}