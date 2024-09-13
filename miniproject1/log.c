#include "log.h"

char log_file_path[MAX_PATH_LENGTH];

void init_log_file_path()
{
    if (getcwd(log_file_path, sizeof(log_file_path)) != NULL)
    {
        snprintf(log_file_path + strlen(log_file_path),sizeof(log_file_path) - strlen(log_file_path),"/command_log.txt");
    }
    else
    {
        perror("getcwd");
    }
}

void load_log()
{
    FILE *file = fopen(log_file_path, "r");
    if (file)
    {
        while (log_index < 15 && fgets(log_buffer[log_index], sizeof(log_buffer[log_index]), file))
        {
            log_buffer[log_index][strcspn(log_buffer[log_index], "\n")] = '\0';
            if (log_index < LOG_SIZE)
            {
                log_index++;
            }
        }
        fclose(file);
    }
}

void save_log()
{
    FILE *file = fopen(log_file_path, "w");
    if (file)
    {
        int i;
        for (i = 0; i < log_index; i++)
        {
            fprintf(file, "%s\n", log_buffer[i]);
        }
        fclose(file);
    }
}

void add_to_log(char *command)
{
    char *com1 = trim_spaces(log_buffer[log_index - 1]);
    char *com2 = trim_spaces(command);
    if (log_index > 0 && strcmp(log_buffer[log_index - 1], command) == 0)
    {
        return;
    }
    if (log_index == 15)
    {
        for (int i = 0; i < LOG_SIZE - 1; i++)
        {
            strcpy(log_buffer[i], log_buffer[i + 1]);
        }
        strcpy(log_buffer[14], command);
    }
    else
    {
        strcpy(log_buffer[log_index++], command);
    }
    save_log();
}

void print_log()
{
    int i;
    for (i = 0; i < log_index; i++)
    {
        printf("%d: %s\n", i + 1, log_buffer[i]);
    }
}

void purge_log()
{
    log_index = 0;
    save_log();
}

void execute_log_command(int index)
{
    if (index < 1 || index > log_index)
    {
        printf("Invalid log index\n");
        return;
    }

    int pid = fork();
    if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", log_buffer[log_index - index], (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}