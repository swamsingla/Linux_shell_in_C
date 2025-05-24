#ifndef ACTIVITIES_H
#define ACTIVITIES_H

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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <termios.h>
#include "structs.h"

extern int process_count;


extern ProcessInfo processes[100];

int compare_processes(const void *a, const void *b);
void activities();
void update_process_states();
void remove_process(pid_t pid);
char proclore_command1(const char *pid_str);

#endif