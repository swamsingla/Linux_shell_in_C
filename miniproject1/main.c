#include <stdio.h>
#include "variables.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "spec1.h"
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include "log.h"
#include "helper.h"
#include "hop.h"
#include "reveal.h"
#include "proclore.h"
#include "seek.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <termios.h>
#include "spec9.h"
#include "neonate.h"
#include "activities.h"
#include "signal.h"
#include "iman.h"
#include "ping.h"
#include "fgbg.h"
#include "pipe.h"

#define BUFFER_SIZE 10000

#define COLOR_RESET "\033[0m"
#define COLOR_DIR "\033[34m"  // Blue
#define COLOR_EXEC "\033[32m" // Green
#define COLOR_FILE "\033[37m" // White

#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"

#define MAX_RESULTS 1024
#define MAX_PATH_LENGTH 1024

char previous_dir[MAX_PATH_LENGTH];
#define MAX_PATH 1024

#define MAX_COMMAND_LENGTH 2048
#define MAX_PIPE_COMMANDS 20

char fore_to_back[MAX_COMMAND_LENGTH];
char command_more_than_2_seconds[1024];
int flag_2_seconds = 0;
int timee = 0;

char log_buffer[15][1024];
int log_index = 0;

AliasList *aliases = NULL;

ProcessInfo processes[100];
int process_count = 0;

pid_t foreground_pid = -1;

void terminate_all_processes()
{
    for (int i = 0; i < process_count; i++)
    {
        kill(processes[i].pid, SIGTERM);
    }
}

ProcessInfo *find_process_by_pid(pid_t pid)
{
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].pid == pid)
        {
            return &processes[i];
        }
    }
    return NULL;
}

int main()
{
    setup_signal_handlers();
    init_log_file_path();
    aliases = init_alias_list();

    load_myshrc(aliases);
    char hostname[HOST_NAME_LEN];
    char cwd[MAX_PATH_LENGTH];
    char shell_home_relative[MAX_PATH_LENGTH];
    char username[USER_LEN];

    char *color_username = "\033[32m"; // Green
    char *color_hostname = "\033[34m"; // Blue
    char *color_reset = "\033[0m";     // Reset color

    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        return 1;
    }

    strcpy(username, getenv("USER"));
    if (username == NULL)
    {
        fprintf(stderr, "Could not retrieve username.\n");
        return 1;
    }

    if (getcwd(shell_home_relative, sizeof(shell_home_relative)) == NULL)
    {
        perror("getcwd");
        return 1;
    }
    char input[MAX_COMMAND_LENGTH];

    strcpy(previous_dir, shell_home_relative);
    load_log();
    while (1)
    {
        // printf("YES\n");
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 1;
        }

        memset(input, 0, sizeof(input));

        spec1(cwd, shell_home_relative, color_username, username, color_hostname, hostname, flag_2_seconds, command_more_than_2_seconds, timee);
        printf("%s", color_reset);
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            if (feof(stdin))
            {
                // Ctrl-D detected (EOF)
                printf("\nLogging out...\n");
                terminate_all_processes();
                exit(0);
            }
            else
            {
                perror("fgets");
                continue;
            }
        }
        flag_2_seconds = 0;

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }

        int flag_log = 0;
        int log_exectute_flag = 0;
        int pointer1 = 0;
        int pointer2 = 0;

        while (input[pointer2] != '\0')
        {
            if (input[pointer2] == ';' || input[pointer2] == '&' || input[pointer2 + 1] == '\0')
            {
                int command_length;

                if (input[pointer2 + 1] == '\0')
                {
                    command_length = pointer2 - pointer1 + 1;
                }
                else
                {
                    command_length = pointer2 - pointer1;
                }

                char command_to_run[command_length + 1];
                int i = 0;

                while (pointer1 < pointer2)
                {
                    command_to_run[i++] = input[pointer1++];
                }
                if (input[pointer2 + 1] == '\0')
                {
                    command_to_run[i++] = input[pointer1++];
                }
                command_to_run[i] = '\0';
                char *command = trim_spaces(command_to_run);
                char *alias_command = get_alias(aliases, command);
                if (alias_command != NULL)
                {
                    // printf("YES\n");
                    // Replace command with alias command
                    strcpy(command, alias_command);
                }
                if (strlen(command) > 0)
                {
                    command = trim_spaces(command);

                    if (strlen(command) == 0)
                    {
                        continue;
                    }
                    if (strncmp(command, "log purge", 9) == 0)
                    {
                        purge_log();
                        printf("Log purged.\n");
                        flag_log = 1;
                    }
                    else if (strncmp(command, "log execute", 11) == 0)
                    {
                        int index = atoi(command + 12);
                        flag_log = 1;
                        if (index < 1 || index > log_index)
                        {
                            printf("INVALID LOG INDEX\n");
                        }
                        else
                        {
                            command = log_buffer[log_index - index];
                            command = trim_spaces(command);
                            process_command_with_pipes(command, cwd, shell_home_relative, input, pointer2);
                        }
                    }
                    else if (strncmp(command, "log", 3) == 0)
                    {
                        print_log();
                        flag_log = 1;
                    }
                    else
                    {
                        process_command_with_pipes(command, cwd, shell_home_relative, input, pointer2);
                    }
                }
                pointer1 = pointer2 + 1;
            }

            pointer2++;
        }
        if (flag_log == 0)
        {
            add_to_log(input);
        }
    }

    return 0;
}
