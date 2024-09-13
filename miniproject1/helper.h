#ifndef HELPER_H
#define HELPER_H

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
#include <string.h>
#include "structs.h"

// #define MAX_COMMAND_LENGTH 1024
// extern AliasList *aliases;
// extern ProcessInfo *processes;
// extern int process_count;
// extern char fore_to_back[MAX_COMMAND_LENGTH];
// extern char command_more_than_2_seconds[1024];
// extern int flag_2_seconds;
// extern int timee;
// extern int foreground_pid;

// #define MAX_PATH_LENGTH 1024

extern ProcessInfo processes[100];
extern int process_count;
char *trim_spaces(char *str);
void remove_process(pid_t pid);

#endif