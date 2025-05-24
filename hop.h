#ifndef HOP_H
#define HOP_H

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
#include "helper.h"

#define MAX_PATH_LENGTH 1024
extern char previous_dir[MAX_PATH_LENGTH];

void hop_command(char *path, char *shell_home_relative);
void hop_original(char *cwd,char *command,char *shell_home_relative);

#endif