#ifndef PIPE_H
#define PIPE_H

#include <stdio.h>
#include "variables.h"
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
#include "spec1.h"
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include "log.h"
#include "helper.h"
#include "hop.h"
#include "reveal.h"
#include "proclore.h"
#include "seek.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <termios.h>
#include "neonate.h"
#include "activities.h"
#include "structs.h"
#include "ping.h"
#include "spec9.h"
#include "iman.h"
#include "fgbg.h"

extern AliasList *aliases;
#define MAX_PIPE_COMMANDS 20

#define MAX_COMMAND_LENGTH 2048

extern char fore_to_back[MAX_COMMAND_LENGTH];
extern char command_more_than_2_seconds[1024];
extern int flag_2_seconds;
extern int timee;
extern pid_t foreground_pid;

void execute_command(char *command, char *cwd, char *shell_home_relative, char *input, int pointer2);
void execute_pipe_commands(char **commands, int num_pipes, char *cwd, char *shell_home_relative, char *input, int pointer2);
void process_command_with_pipes(char *command, char *cwd, char *shell_home_relative, char *input, int pointer2);

#endif