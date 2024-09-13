#include "hop.h"
void hop_command(char *path, char *shell_home_relative)
{
    char *cwd = NULL;
    size_t size = 0;
    char resolved_path[MAX_PATH_LENGTH];

    if (strncmp(path, "~", 1) == 0)
    {
        snprintf(resolved_path, sizeof(resolved_path), "%s%s", shell_home_relative, path + 1);
        path = resolved_path;
    }
    else if (strcmp(path, ".") == 0)
    {
        cwd = getcwd(NULL, 0);
        if (cwd == NULL)
        {
            perror("getcwd");
            return;
        }
        path = cwd;
    }
    else if (strcmp(path, "..") == 0)
    {
        path = "..";
    }
    else if (strcmp(path, "-") == 0)
    {
        if (strlen(previous_dir) == 0)
        {
            printf("Previous directory not set.\n");
            return;
        }
        path = previous_dir;
    }

    if (chdir(path) != 0)
    {
        perror("chdir");
    }
    else
    {
        if (cwd == NULL)
        {
            cwd = getcwd(NULL, 0);
            if (cwd == NULL)
            {
                perror("getcwd");
                return;
            }
        }
        printf("%s\n", cwd);
    }

    free(cwd);
}

void hop_original(char *cwd, char *command, char *shell_home_relative)
{
    cwd = getcwd(NULL, 0); 
    if (cwd == NULL)
    {
        perror("getcwd");
        return;
    }

    command = trim_spaces(command);

    if (strlen(command) == 3)
    {
        char *path = cwd;
        path = strtok(path, " ");
        char *cwd_temp = cwd;
        while (path != NULL)
        {
            char *cwd_temp2 = cwd;
            hop_command(path, shell_home_relative);
            path = strtok(NULL, " ");
            strcpy(previous_dir, cwd_temp);
            cwd_temp = cwd_temp2;
        }
        strcpy(previous_dir, cwd_temp);
    }
    else
    {
        char *path = command + 4;
        path = strtok(path, " ");
        char *cwd_temp = cwd;
        while (path != NULL)
        {
            char *cwd_temp2 = cwd;
            hop_command(path, shell_home_relative);
            path = strtok(NULL, " ");
            strcpy(previous_dir, cwd_temp);
            cwd_temp = cwd_temp2;
        }
        strcpy(previous_dir, cwd_temp);
    }

    free(cwd);
}
