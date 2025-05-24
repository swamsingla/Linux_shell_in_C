#ifndef SIGNAL_H
#define SIGNAL_H
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
#include "helper.h"

extern ProcessInfo processes[100];
extern int process_count;

#define MAX_COMMAND_LENGTH 2048

extern char fore_to_back[MAX_COMMAND_LENGTH];

extern pid_t foreground_pid;
void sigint_handler(int sig);
void sigtstp_handler(int signum);
void sigchld_handler(int signo);
void setup_signal_handlers();

#endif