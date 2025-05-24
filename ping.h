#ifndef PING_H
#define PING_H

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
#include "spec1.h"
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include "helper.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <termios.h>

void ping_original(char *command);

#endif