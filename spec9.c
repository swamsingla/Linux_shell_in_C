#include "spec9.h"

AliasList *init_alias_list()
{
    AliasList *list = (AliasList *)malloc(sizeof(AliasList));
    list->head = NULL;
    return list;
}

void add_alias(AliasList *list, const char *name, const char *command)
{
    Alias *new_alias = (Alias *)malloc(sizeof(Alias));
    new_alias->name = strdup(name);
    new_alias->command = strdup(command);
    new_alias->next = list->head;
    list->head = new_alias;
}

char *get_alias(AliasList *list, char *name)
{
    Alias *current = list->head;
    while (current != NULL)
    {
        name = trim_spaces(name);
        char *temp = current->name;
        temp = trim_spaces(current->name);
        current->name = temp;
        if (strcmp(current->name, name) == 0)
        {
            return current->command;
        }
        current = current->next;
    }
    return NULL;
}

void parse_myshrc_line(AliasList *aliases, char *line)
{
    if (strncmp(line, "alias", 5) == 0)
    {
        char *alias_name = strtok(line + 6, "=");
        char *alias_command = strtok(NULL, "'");

        add_alias(aliases, alias_name, alias_command);
    }
}

void load_myshrc(AliasList *aliases)
{
    char home[256];
    getcwd(home, sizeof(home));
    char path[1000];
    snprintf(path, sizeof(path), "%s/myshrc", home);
    // printf("%s\n",path);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        parse_myshrc_line(aliases, line);
    }

    fclose(file);
}