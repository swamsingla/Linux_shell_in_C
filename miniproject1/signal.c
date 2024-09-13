#include "signal.h"

void sigint_handler(int sig)
{
    if (foreground_pid != -1)
    {
        kill(foreground_pid, SIGINT);
    }
}
void sigtstp_handler(int signum)
{
    if (foreground_pid != -1)
    {
        pid_t fg_pgid = getpgid(foreground_pid);
        if (fg_pgid == -1)
        {
            perror("Error getting process group ID of foreground process");
            return;
        }

        if (killpg(fg_pgid, SIGTSTP) == -1)
        {
            perror("Error sending SIGTSTP to foreground process group");
        }
        else
        {
            printf("Process with pid %d stopped and moved to background",foreground_pid);
            strcpy(processes[process_count].command, fore_to_back);
            processes[process_count].pid = foreground_pid;
            strcpy(processes[process_count].state, "stopped");
            process_count++;

            foreground_pid = -1;
        }
    }
    else
    {
        fprintf(stderr, "No foreground process to stop\n");
    }
    //  sigtstp_handled = 0;
}

void sigchld_handler(int signo)
{
    (void)signo;

    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child process %d killed by signal %d\n", pid, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Child process %d stopped by signal %d\n", pid, WSTOPSIG(status));
        }
        remove_process(pid);
    }
}

void setup_signal_handlers()
{
    struct sigaction sa;

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("Error setting up SIGCHLD handler");
        exit(1);
    }

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("Error setting up SIGINT handler");
        exit(1);
    }

    // Handle Ctrl-Z
    sa.sa_handler = sigtstp_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
    {
        perror("Error setting up SIGTSTP handler");
        exit(1);
    }
}