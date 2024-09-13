#ifndef LOG_H
#define LOG_H
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

#define LOG_SIZE 15
#define MAX_PATH_LENGTH 1024

extern char log_file_path[MAX_PATH_LENGTH];

void init_log_file_path();


extern char log_buffer[LOG_SIZE][1024];
extern int log_index;

void load_log();
void save_log();
void add_to_log(char *command);
void print_log();
void purge_log();
void execute_log_command(int index);


#endif