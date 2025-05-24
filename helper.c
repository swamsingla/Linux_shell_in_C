#include "helper.h"

char *trim_spaces(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t')
    {
        str++;
    }

    if (*str == 0)
    {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
    {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

void remove_process(pid_t pid)
{
    for (int i = 0; i < process_count; i++)
    {
        if (pid == processes[i].pid)
        {
            for (int j = i; j < process_count - 1; j++)
            {
                processes[j] = processes[j + 1];
            }
            process_count--;
            break;
        }
    }
}