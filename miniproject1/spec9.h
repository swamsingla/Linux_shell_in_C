#ifndef SPEC9_H
#define SPEC_H

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
#include "structs.h"

AliasList *init_alias_list();

void add_alias(AliasList *list, const char *name, const char *command);

char *get_alias(AliasList *list, char *name);

void parse_myshrc_line(AliasList *aliases, char *line);

void load_myshrc(AliasList *aliases);


#endif