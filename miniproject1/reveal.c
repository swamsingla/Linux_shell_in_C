#include "reveal.h"

void reveal_original(char *command, char *shell_home_relative)
{
    char *args = command + 6;
    int show_all = 0;
    int long_format = 0;
    char cwd[MAX_PATH_LENGTH]; // Buffer to hold the current working directory
    char *path = getcwd(cwd, sizeof(cwd));
    args = trim_spaces(args);
    // printf("%s\n", args);
    while (*args == '-' || *args == ' ')
    {
        if (strncmp(args, "-a", 2) == 0)
        {
            show_all = 1;
            args += 2;
            if (strlen(args) == 0)
            {
                break;
            }
            while (*args != ' ')
            {
                if (*args == 'l')
                {
                    long_format = 1;
                }
                args++;
                if (strlen(args) == 0)
                {
                    break;
                }
            }
        }
        else if (strncmp(args, "-l", 2) == 0)
        {
            long_format = 1;
            args += 2;
            if (strlen(args) == 0)
            {
                break;
            }
            while (*args != ' ')
            {
                if (*args == 'a')
                {
                    show_all = 1;
                }
                args++;
                if (strlen(args) == 0)
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }

        args = trim_spaces(args);
    }

    args = trim_spaces(args);

    if (*args != '\0')
    {
        path = args;
    }

    reveal_command(path, show_all, long_format, shell_home_relative);
}
void print_file_info(struct dirent *entry, char *path)
{
    struct stat file_stat;
    char full_path[MAX_PATH_LENGTH];
    if (snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name) >= sizeof(full_path))
    {
        fprintf(stderr, "Warning: Path length exceeds buffer size\n");
        return;
    }

    if (stat(full_path, &file_stat) == -1)
    {
        perror("stat");
        return;
    }

    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %ld", (long)file_stat.st_nlink);

    printf(" %s %s", getpwuid(file_stat.st_uid)->pw_name, getgrgid(file_stat.st_gid)->gr_name);

    printf(" %5ld", (long)file_stat.st_size);

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat.st_mtime));
    printf(" %s", time_str);

    char *color = COLOR_FILE; 
    if (S_ISDIR(file_stat.st_mode))
    {
        color = COLOR_DIR;
    }
    else if (file_stat.st_mode & S_IXUSR)
    {
        color = COLOR_EXEC;
    }

    printf(" %s%s%s\n", color, entry->d_name, COLOR_RESET);
}

void reveal_command(char *path, int show_all, int long_format, char *shell_home_relative)
{
    char resolved_path[MAX_PATH_LENGTH];
    char full_path[MAX_PATH_LENGTH];
    DIR *dir;
    struct dirent *entry;
    struct dirent **namelist;
    int n;

    if (strncmp(path, "~", 1) == 0)
    {
        snprintf(resolved_path, sizeof(resolved_path), "%s%s", shell_home_relative, path + 1);
    }
    else if (strcmp(path, "-") == 0)
    {
        if (strlen(previous_dir) == 0)
        {
            printf("Previous directory not set.\n");
            return;
        }
        snprintf(resolved_path, sizeof(resolved_path), "%s", previous_dir);
    }
    else if (path[0] == '/') // Absolute path
    {
        snprintf(resolved_path, sizeof(resolved_path), "%s", path);
    }
    else // Relative path
    {
        snprintf(resolved_path, sizeof(resolved_path), "%s/%s", shell_home_relative, path);
    }

    printf("Resolved Path: %s\n", resolved_path);

    dir = opendir(resolved_path);
    if (!dir)
    {
        perror("opendir");
        return;
    }

    n = scandir(resolved_path, &namelist, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        closedir(dir);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        entry = namelist[i];

        if (snprintf(full_path, sizeof(full_path), "%s/%s", resolved_path, entry->d_name) >= sizeof(full_path))
        {
            fprintf(stderr, "Warning: Path length exceeds buffer size\n");
            free(entry);
            continue;
        }

        if (!show_all && entry->d_name[0] == '.')
        {
            free(entry);
            continue;
        }
        
        if (long_format)
        {
            print_file_info(entry, resolved_path);
        }
        else
        {
            char *color = (entry->d_type == DT_DIR) ? COLOR_DIR : COLOR_FILE;
            if (entry->d_type == DT_REG)
            {
                struct stat file_stat;
                if (stat(full_path, &file_stat) == 0 && (file_stat.st_mode & S_IXUSR))
                {
                    color = COLOR_EXEC;
                }
            }
            printf("%s%s%s\n", color, entry->d_name, COLOR_RESET);
        }

        free(entry);
    }

    free(namelist);
    closedir(dir);
}
