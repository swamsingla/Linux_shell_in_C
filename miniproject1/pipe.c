#include "pipe.h"

void execute_command(char *command, char *cwd, char *shell_home_relative, char *input, int pointer2)
{
    int input_redirection = -1;
    int output_redirection = -1;
    int append_redirection = -1;
    char input_file[MAX_PATH_LENGTH];
    char output_file[MAX_PATH_LENGTH];
    memset(input_file, 0, sizeof(input_file));
    memset(output_file, 0, sizeof(output_file));

    for (int j = 0; command[j] != '\0'; j++)
    {
        if (command[j] == '<')
        {
            input_redirection = j;
        }
        else if (command[j] == '>' && command[j + 1] != '>')
        {
            output_redirection = j;
        }
        else if (command[j] == '>' && command[j + 1] == '>')
        {
            append_redirection = j;
            j++;
        }
    }

    if (input_redirection != -1)
    {
        sscanf(command + input_redirection + 1, "%s", input_file);
        command[input_redirection] = '\0';
    }
    if (output_redirection != -1)
    {
        sscanf(command + output_redirection + 1, "%s", output_file);
        command[output_redirection] = '\0';
    }
    if (append_redirection != -1)
    {
        sscanf(command + append_redirection + 2, "%s", output_file);
        command[append_redirection] = '\0';
    }

    command = trim_spaces(command);
    char *alias_command = get_alias(aliases, command);
    if (alias_command != NULL)
    {
        // printf("YES\n");
        alias_command = trim_spaces(alias_command);
        strcpy(command, alias_command);
    }
    // printf("%s\n", command);

    int saved_stdin = -1, saved_stdout = -1;
    int fd_in = -1, fd_out = -1;

    if (input_redirection != -1)
    {
        fd_in = open(input_file, O_RDONLY);
        if (fd_in < 0)
        {
            perror("No such input file found!");
            return;
        }
        saved_stdin = dup(STDIN_FILENO);
        dup2(fd_in, STDIN_FILENO);
    }

    if (output_redirection != -1)
    {
        fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0)
        {
            perror("Error opening output file!");
            return;
        }
        saved_stdout = dup(STDOUT_FILENO);
        dup2(fd_out, STDOUT_FILENO);
    }

    if (append_redirection != -1)
    {
        fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_out < 0)
        {
            perror("Error opening output file!");
            return;
        }
        saved_stdout = dup(STDOUT_FILENO);
        dup2(fd_out, STDOUT_FILENO);
    }
    if (strlen(command) > 0)
    {
        if (strncmp(command, "hop", 3) == 0)
        {
            hop_original(cwd, command, shell_home_relative);
        }

        else if (strncmp(command, "reveal", 6) == 0)
        {
            reveal_original(command, shell_home_relative);
        }
        else if (strncmp(command, "proclore", 8) == 0)
        {
            command = trim_spaces(command);
            char *pid_str = NULL;

            if (strlen(command) > 8)
            {
                pid_str = command + 9;
                pid_str = trim_spaces(pid_str);
            }

            proclore_command(pid_str);
        }

        else if (strncmp(command, "seek", 4) == 0)
        {
            seek_original(command);
        }

        else if (strncmp(command, "exit", 4) == 0)
        {
            exit(0);
        }

        else if (strcmp(command, "activities") == 0)
        {
            update_process_states();
            activities();
        }
        else if (strncmp(command, "ping", 4) == 0)
        {
            ping_original(command);
        }
        else if (strncmp(command, "fg", 2) == 0)
        {
            int status;
            struct timeval start, end;
            gettimeofday(&start, NULL);
            char *pid_str = strtok(command + 2, " ");
            if (pid_str != NULL)
            {
                pid_t pid = (pid_t)atoi(pid_str);
                fg_command(pid);
            }
            else
            {
                printf("Usage: fg <pid>\n");
            }
            gettimeofday(&end, NULL);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

            if (time_taken > 2.0)
            {
                timee = (int)time_taken;
                flag_2_seconds = 1;
                strcpy(command_more_than_2_seconds, command);
            }
        }
        else if (strncmp(command, "bg", 2) == 0)
        {
            char *pid_str = strtok(command + 2, " ");
            if (pid_str != NULL)
            {
                pid_t pid = (pid_t)atoi(pid_str);
                bg_command(pid);
            }
            else
            {
                printf("Usage: bg <pid>\n");
            }
        }
        else if (strncmp(command, "iMan", 4) == 0)
        {
            int status;
            struct timeval start, end;
            gettimeofday(&start, NULL);
            char *cmd_str = strtok(command + 4, " ");
            cmd_str = trim_spaces(cmd_str);
            if (cmd_str != NULL)
            {
                retrieve_man_page(cmd_str);
            }
            else
            {
                printf("Usage: iMan <command_name>\n");
            }
            gettimeofday(&end, NULL);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

            if (time_taken > 2.0)
            {
                timee = (int)time_taken;
                flag_2_seconds = 1;
                strcpy(command_more_than_2_seconds, command);
            }
        }
        else if (strncmp(command, "neonate -n", 10) == 0)
        {
            int status;
            struct timeval start, end;
            gettimeofday(&start, NULL);
            char *cmd_str = strtok(command + 10, " ");
            cmd_str = trim_spaces(cmd_str);
            int n = atoi(cmd_str);
            monitor_and_terminate('x', n);
            gettimeofday(&end, NULL);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

            if (time_taken > 2.0)
            {
                timee = (int)time_taken;
                flag_2_seconds = 1;
                strcpy(command_more_than_2_seconds, command);
            }
        }
        else
        {
            if (input[pointer2] == '&')
            {
                if (command[strlen(command) - 1] == '&')
                {
                    command[strlen(command) - 1] = '\0';
                }
                int pid = fork();
                if (pid == 0)
                {
                    char *args[] = {"/bin/sh", "-c", command, NULL};
                    for (int i = 0; i < sizeof(args); i++)
                    {
                        if (args[i] == NULL)
                        {
                            break;
                        }
                    }
                    execvp(args[0], args);
                    perror("execvp");
                    exit(1);
                }
                else if (pid > 0)
                {
                    strcpy(processes[process_count].command, command);
                    processes[process_count].pid = pid;
                    strcpy(processes[process_count].state, "running");
                    process_count++;
                    printf("[%d] %d\n", pid, pid);
                }
                else
                {
                    perror("fork");
                }
            }
            else
            {
                strcpy(fore_to_back, command);
                int pid = fork();
                if (pid < 0)
                {
                    perror("fork");
                }
                else if (pid == 0)
                {
                    execl("/bin/sh", "sh", "-c", command, (char *)NULL);

                    perror("execl");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    foreground_pid = pid;
                    int status;
                    struct timeval start, end;
                    gettimeofday(&start, NULL);
                    // printf("%d\n", foreground_pid);
                    if (waitpid(pid, &status, WUNTRACED) == -1)
                    {
                        perror("waitpid");
                    }
                    gettimeofday(&end, NULL);
                    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

                    if (time_taken > 2.0)
                    {
                        timee = (int)time_taken;
                        flag_2_seconds = 1;
                        strcpy(command_more_than_2_seconds, command);
                    }
                }
            }
        }
    }
    if (saved_stdin != -1)
    {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }

    if (fd_in != -1)
    {
        close(fd_in);
    }
    if (fd_out != -1)
    {
        close(fd_out);
    }
}

void execute_pipe_commands(char **commands, int num_pipes, char *cwd, char *shell_home_relative, char *input, int pointer2)
{
    int pipefds[2 * num_pipes];

    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(pipefds + i * 2) < 0)
        {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    int cmd_num = 0;
    for (cmd_num = 0; cmd_num <= num_pipes; cmd_num++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            if (cmd_num != 0)
            {
                dup2(pipefds[(cmd_num - 1) * 2], STDIN_FILENO);
            }

            if (cmd_num != num_pipes)
            {
                dup2(pipefds[cmd_num * 2 + 1], STDOUT_FILENO);
            }

            for (int i = 0; i < 2 * num_pipes; i++)
            {
                close(pipefds[i]);
            }

            execute_command(commands[cmd_num], cwd, shell_home_relative, input, pointer2);

            exit(0);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 2 * num_pipes; i++)
    {
        close(pipefds[i]);
    }

    for (int i = 0; i <= num_pipes; i++)
    {
        wait(NULL);
    }
}

void process_command_with_pipes(char *command, char *cwd, char *shell_home_relative, char *input, int pointer2)
{
    char *pipe_commands[MAX_PIPE_COMMANDS];
    int num_pipes = 0;

    char *token = strtok(command, "|");
    while (token != NULL && num_pipes < MAX_PIPE_COMMANDS)
    {
        pipe_commands[num_pipes++] = trim_spaces(token);
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < num_pipes; i++)
    {
        if (strlen(pipe_commands[i]) == 0)
        {
            fprintf(stderr, "Invalid use of pipe\n");
            return;
        }
    }

    if (num_pipes > 1)
    {
        execute_pipe_commands(pipe_commands, num_pipes - 1, cwd, shell_home_relative, input, pointer2);
    }
    else
    {
        execute_command(pipe_commands[0], cwd, shell_home_relative, input, pointer2);
    }
}