#include "neonate.h"
struct termios original_settings;

void terminate_with_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE); 
}

void restore_terminal()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_settings) == -1)
    {
        terminate_with_error("Failed to reset terminal settings");
    }
}

void set_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &original_settings) == -1)
    {
        terminate_with_error("Unable to retrieve terminal settings");
    }

    atexit(restore_terminal);

    struct termios raw_settings = original_settings;
    raw_settings.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_settings) == -1)
    {
        terminate_with_error("Unable to apply raw terminal settings");
    }
}

void monitor_and_terminate(char trigger, int interval)
{
    setbuf(stdout, NULL);

    set_raw_mode();

    pid_t child_pid = fork();

    if (child_pid == 0)
    {
        while (1)
        {
            FILE *file = fopen("/proc/sys/kernel/ns_last_pid", "r");
            char pid_buffer[10];
            fgets(pid_buffer, sizeof(pid_buffer), file); // Read the last PID
            fclose(file);

            printf("%s", pid_buffer);
            sleep(interval);
        }
    }
    else if (child_pid > 0)
    {
        char input;
        while (read(STDIN_FILENO, &input, 1) == 1 && input != trigger)
        {
        }

        kill(child_pid, SIGKILL);
    }

    restore_terminal();
}