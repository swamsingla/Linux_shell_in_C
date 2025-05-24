#include "proclore.h"
void proclore_command(const char *pid_str)
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
        return;
    }

    int ppid, pgid, session, tty_nr, tpgid;
    char comm[256], state;
    unsigned long vsize;

    fscanf(stat_file, "%d %s %c %d %d %d %d %d %lu",
           &pid, comm, &state, &ppid, &pgid, &session, &tty_nr, &tpgid, &vsize);
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

    printf("PID: %d\n", pid);
    printf("Process Status: %c\n", state);
    printf("Process Group: %d\n", pgid);
    printf("Virtual Memory: %lu KB\n", vsize / 1024);
    printf("Executable Path: %s\n", buffer);
}