#ifndef FGBG_H
#define FGBG_H

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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <termios.h>
#include "activities.h"

extern int process_count;

void bg_command(pid_t pid);

void fg_command(pid_t pid);

#endif