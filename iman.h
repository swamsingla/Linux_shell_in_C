#ifndef IMAN_H
#define IMAN_H

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

#define BUFFER_SIZE 10000

void retrieve_man_page(const char *cmd_name);
int initialize_socket(const char *hostname, int port_number);

#endif