#ifndef SEEK_H
#define SEEK_H

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

#define MAX_RESULTS 1024
#define MAX_PATH_LENGTH 1024

void seek_original(char *command);
char *expand_path(const char *path);
void seek_in_directory(const char *base_dir, const char *target_name, int only_directories, int only_files, char results[][MAX_PATH_LENGTH], int *match_count);

#endif