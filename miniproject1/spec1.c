#include "spec1.h"

void spec1(char *cwd, char *shell_home_relative, char *color_username, char *username, char *color_hostname, char *hostname, int flag_2_seconds, char *command_more_than_2_second, int timee)
{
    if (flag_2_seconds == 1)
    {
        if (strcmp(cwd, shell_home_relative) == 0)
        {
            printf("<%s%s@%s%s:~ %s : %ds> ", color_username, username, color_hostname, hostname, command_more_than_2_second, timee);
        }
        else if (strstr(cwd, shell_home_relative) == cwd)
        {
            printf("<%s%s@%s%s:~%s %s : %ds> ", color_username, username, color_hostname, hostname, cwd + strlen(shell_home_relative), command_more_than_2_second, timee);
        }
        else
        {
            printf("<%s%s@%s%s: %s  %s : %ds> ", color_username, username, color_hostname, hostname, cwd, command_more_than_2_second, timee);
        }
    }
    else
    {
        if (strcmp(cwd, shell_home_relative) == 0)
        {
            printf("<%s%s@%s%s:~> ", color_username, username, color_hostname, hostname);
        }
        else if (strstr(cwd, shell_home_relative) == cwd)
        {
            printf("<%s%s@%s%s:~%s> ", color_username, username, color_hostname, hostname, cwd + strlen(shell_home_relative));
        }
        else
        {
            printf("<%s%s@%s%s:%s> ", color_username, username, color_hostname, hostname, cwd);
        }
    }
}