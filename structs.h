#ifndef STRUCTS_H
#define STRUCTS_H

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

typedef struct
{
    char command[256];
    pid_t pid;
    char state[10];
} ProcessInfo;

typedef struct Alias
{
    char *name; 
    char *command;
    struct Alias *next;
} Alias;

typedef struct AliasList
{
    Alias *head;
} AliasList;

#endif