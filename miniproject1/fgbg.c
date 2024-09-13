#include "fgbg.h"

void bg_command(pid_t pid)
{
    update_process_states();

    int process_found = 0;
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].pid == pid)
        {
            process_found = 1;

            if (strcmp(processes[i].state, "stopped") == 0)
            {
                if (kill(pid, SIGCONT) == -1)
                {
                    perror("Failed to send SIGCONT");
                    return;
                }

                strcpy(processes[i].state, "running");
                printf("Process %d resumed and running in the background\n", pid);
            }
            else
            {
                printf("Process %d is already running\n", pid);
            }
            return;
        }
    }

    if (!process_found)
    {
        printf("No such process found\n");
    }
}

void fg_command(pid_t pid)
{
    kill(pid, 0);
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].pid == pid)
        {
            int r_val = kill(pid, SIGCONT);
            if (r_val == -1)
            {
                fprintf(stderr, "Couldn't resume the process!\n");
                return;
            }
            char pid_str[16];

            snprintf(pid_str, sizeof(pid_str), "%d", pid);
            int terminal_fd = open("/dev/tty", O_RDWR);
            if (terminal_fd == -1)
            {
                perror("open");
            }
            pid_t pgid = getpgid(pid);
            if (pgid == -1)
            {
                perror("getpgid");
            }

            if (tcsetpgrp(terminal_fd, pgid) == -1)
            {
                perror("tcsetpgrp");
            }

            int status;
            do
            {
                pid_t wpid = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                if (wpid == -1)
                {
                    perror("waitpid");
                    close(terminal_fd);
                    return;
                }

                if (WIFEXITED(status))
                {
                    printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
                    remove_process(pid);
                    break;
                }
                else if (WIFSIGNALED(status))
                {
                    printf("Process %d killed by signal %d\n", pid, WTERMSIG(status));
                    remove_process(pid);
                    break;
                }
                else if (WIFSTOPPED(status))
                {
                    printf("Process %d stopped\n", pid);
                    strcpy(processes[i].state, "stopped");
                    break;
                }
                else if (WIFCONTINUED(status))
                {
                    printf("Process %d continued\n", pid);
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

            if (tcsetpgrp(terminal_fd, getpgrp()) == -1)
            {
                perror("tcsetpgrp");
            }
            close(terminal_fd);
        }
    }
}