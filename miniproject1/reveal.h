#ifndef REVEAL_H
#define REVEAL_H
#include <stdio.h>
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
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include "helper.h"

#define MAX_PATH_LENGTH 1024
#define COLOR_RESET "\033[0m"
#define COLOR_DIR "\033[34m"  // Blue
#define COLOR_EXEC "\033[32m" // Green
#define COLOR_FILE "\033[37m" // White

#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"
extern char previous_dir[MAX_PATH_LENGTH];
void reveal_original(char *command,char *shell_home_relative);
void reveal_command(char *path, int show_all, int long_format, char *shell_home_relative);
void print_file_info(struct dirent *entry, char *path);

#endif