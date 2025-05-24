#include "activities.h"

#include "proclore.h"
char proclore_command1(const char *pid_str)
{
    char path[256];
    char buffer[1024];
    int pid;

    if (pid_str == NULL || strlen(pid_str) == 0)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(pid_str);
    }

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *stat_file = fopen(path, "r");
    if (stat_file == NULL)
    {
        perror("fopen");
        return '0';
    }

    int ppid, pgid, session, tty_nr, tpgid;
    char comm[256], state;
    unsigned long vsize;

    fscanf(stat_file, "%d %s %c %d %d %d %d %d %lu",&pid, comm, &state, &ppid, &pgid, &session, &tty_nr, &tpgid, &vsize);
    fclose(stat_file);

    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
    }
    else
    {
        perror("readlink");
        strcpy(buffer, "Unknown");
    }

    return state;
}

int compare_processes(const void *a, const void *b)
{
    return strcmp(((ProcessInfo *)a)->command, ((ProcessInfo *)b)->command);
}

void activities()
{
    qsort(processes, process_count, sizeof(ProcessInfo), compare_processes);

    printf("Command Name\tPID\tState\n");
    for (int i = 0; i < process_count; i++)
    {
        printf("%s\t%d\t%s\n", processes[i].command, processes[i].pid, processes[i].state);
    }
}
void update_process_states()
{
    for (int i = 0; i < process_count; i++)
    {
        char pid_str[16];
        snprintf(pid_str, sizeof(pid_str), "%d", processes[i].pid);

        char state = proclore_command1(pid_str);

        switch (state)
        {
        case 'R':
        case 'S':
            strcpy(processes[i].state, "running");
            break;
        case 'T':
            strcpy(processes[i].state, "stopped");
            break;
        case 'Z':
            strcpy(processes[i].state, "zombie");
            break;
        }

        if (strcmp(processes[i].state, "zombie") == 0)
        {
            for (int j = i; j < process_count - 1; j++)
            {
                processes[j] = processes[j + 1];
            }
            process_count--;
            i--;
        }
    }
}
