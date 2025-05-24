#include "headers.h"

char global_init_home[4096] = "";
char sys_name[4096] = "";
char present_dir[4096] = "";
char user_name[4096] = "";

void expand_alias(char *command) {
    Alias *current = alias_list;
    while (current != NULL) {
        char* pos = strstr(command, trim(current->name));
        if (pos != NULL) {
            size_t new_command_len = strlen(command) - strlen(current->name) + strlen(current->value) + 1;
            char *new_command = (char *)malloc(BUFFER_SIZE * sizeof(char));
            memset(new_command, '\0', BUFFER_SIZE);
            size_t before_name_len = pos - command;
            strncpy(new_command, command, before_name_len);
            new_command[before_name_len] = '\0';
            strcat(new_command, current->value);
            strcat(new_command, pos + strlen(current->name));
            strcpy(command, new_command);
            free(new_command);
        }
        current = current->next;
    }
}

void add_alias(const char *name, const char *value) {
    Alias *new_alias = malloc(sizeof(Alias));
    if (new_alias == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    strncpy(new_alias->name, name, BUFFER_SIZE);
    strncpy(new_alias->value, value, BUFFER_SIZE);
    new_alias->next = alias_list;
    alias_list = new_alias;
}

void load_aliases(const char *filename) {
    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (name && value) {
            add_alias(name, value);
        }
    }
    fclose(file);
}

void process_myshrc() {
    char* myshrc_fn = malloc(BUFFER_SIZE * sizeof(char));
    memset(myshrc_fn, '\0', BUFFER_SIZE);
    sprintf(myshrc_fn, "%s/%s", global_init_home, ".myshrc.txt");
    load_aliases(myshrc_fn);
    free(myshrc_fn);
}

int initiate_shell() {
    process_myshrc();
    char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
    memset(bg_history_fn, '\0', BUFFER_SIZE);
    sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
    int bg_file = open(bg_history_fn, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    free(bg_history_fn);
    close(bg_file);
    int host_status = gethostname(sys_name, BUFFER_SIZE);
    uid_t u_id = getuid();
    struct passwd *p = getpwuid(u_id);
    snprintf(user_name, BUFFER_SIZE, "%s", p->pw_name);
    return 0;
}
