#ifndef NEONATE_H
#define NEONATE_H

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

void monitor_and_terminate(char trigger, int interval);
void set_raw_mode();
void restore_terminal();
void terminate_with_error(const char *message);

#endif