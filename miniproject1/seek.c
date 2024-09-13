#include "seek.h"

void seek_original(char *command)
{
    char *args = command + 5;
    args = trim_spaces(args);

    int only_directories = 0;
    int only_files = 0;
    int execute_flag = 0;
    char *target_name = NULL;
    char *search_directory = ".";

    while (args[0] == '-')
    {
        for (char *flag = args + 1; *flag && *flag != ' '; ++flag)
        {
            if (*flag == 'd')
            {
                only_directories = 1;
            }
            else if (*flag == 'f')
            {
                only_files = 1;
            }
            else if (*flag == 'e')
            {
                execute_flag = 1;
            }
            else
            {
                printf("Invalid flag: -%c\n", *flag);
                return;
            }
        }
        args = strchr(args, ' ');
        if (args != NULL)
        {
            args++;
            args = trim_spaces(args);
        }
        else
        {
            break;
        }
    }

    if (only_directories && only_files)
    {
        printf("Invalid flags! Cannot combine -d and -f.\n");
        return;
    }

    target_name = strtok(args, " ");
    if (!target_name)
    {
        printf("Usage: seek [flags] target [directory]\n");
        return;
    }

    char *dir_arg = strtok(NULL, " ");
    if (dir_arg)
    {
        search_directory = expand_path(dir_arg);
    }

    char results[MAX_RESULTS][MAX_PATH_LENGTH];
    int match_count = 0;
    seek_in_directory(search_directory, target_name, only_directories, only_files, results, &match_count);

    if (execute_flag)
    {
        if (match_count == 1)
        {
            struct stat path_stat;
            stat(results[0], &path_stat);

            if (S_ISDIR(path_stat.st_mode))
            {
                if (access(results[0], X_OK) != 0)
                {
                    printf("Missing permissions for task!\n");
                }
                else
                {
                    chdir(results[0]);
                    printf("%s/\n", results[0]);
                }
            }
            else if (S_ISREG(path_stat.st_mode))
            {
                if (access(results[0], R_OK) != 0)
                {
                    printf("Missing permissions for task!\n");
                }
                else
                {
                    FILE *file = fopen(results[0], "r");
                    if (file != NULL)
                    {
                        char line[MAX_PATH_LENGTH];
                        printf("%s\n", results[0]);
                        while (fgets(line, sizeof(line), file))
                        {
                            printf("%s", line);
                        }
                        fclose(file);
                    }
                }
            }
        }
        else
        {

            
            for (int i = 0; i < match_count; i++)
            {
                struct stat path_stat;
                stat(results[i], &path_stat);
                if (S_ISDIR(path_stat.st_mode))
                {
                    printf("\033[1;34m%s\033[0m\n", results[i]); // Blue for directories
                }
                else if (S_ISREG(path_stat.st_mode))
                {
                    printf("\033[1;32m%s\033[0m\n", results[i]); // Green for files
                }
            }
        }
    }
    else
    {
        if (match_count == 0)
        {
            printf("No match found!\n");
        }
        else
        {
            for (int i = 0; i < match_count; i++)
            {
                struct stat path_stat;
                stat(results[i], &path_stat);
                if (S_ISDIR(path_stat.st_mode))
                {
                    printf("\033[1;34m%s\033[0m\n", results[i]); // Blue for directories
                }
                else if (S_ISREG(path_stat.st_mode))
                {
                    printf("\033[1;32m%s\033[0m\n", results[i]); // Green for files
                }
            }
        }
    }
}


void seek_in_directory(const char *base_dir, const char *target_name, int only_directories, int only_files, char results[][MAX_PATH_LENGTH], int *match_count)
{
    struct dirent *entry;
    DIR *dp = opendir(base_dir);

    if (dp == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", base_dir, entry->d_name);

        if (strcmp(entry->d_name, target_name) == 0)
        {
            struct stat path_stat;
            stat(path, &path_stat);

            if ((only_directories && S_ISDIR(path_stat.st_mode)) ||
                (only_files && S_ISREG(path_stat.st_mode)) ||
                (!only_directories && !only_files))
            {
                strncpy(results[*match_count], path, MAX_PATH_LENGTH);
                (*match_count)++;
            }
        }

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            seek_in_directory(path, target_name, only_directories, only_files, results, match_count);
        }
    }

    closedir(dp);
}

char *expand_path(const char *path)
{
    static char expanded_path[MAX_PATH_LENGTH];
    if (path[0] == '~')
    {
        snprintf(expanded_path, sizeof(expanded_path), "%s%s", getenv("HOME"), path + 1);
    }
    else
    {
        realpath(path, expanded_path);
    }
    return expanded_path;
}