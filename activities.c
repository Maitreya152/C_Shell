#include "headers.h"

#define MAX_PIDS 4096
int* get_all_pids(const char *filename, int *pid_count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int i = 0, line_start = 0;
    int *pid_list = (int *)malloc(MAX_PIDS * sizeof(int));
    memset(pid_list, '\0', MAX_PIDS * sizeof(int));
    *pid_count = 0;

    if (pid_list == NULL) {
        perror("Failed to allocate memory");
        close(fd);
        return NULL;
    }

    while ((bytes_read = read(fd, buffer + i, 1)) > 0) {
        if (buffer[i] == '\n' || i == BUFFER_SIZE - 1) {
            buffer[i] = '\0';
            int file_pid;

            if (sscanf(buffer + line_start, "%d:", &file_pid) == 1) {
                pid_list[*pid_count] = file_pid;
                (*pid_count)++;

                if (*pid_count >= MAX_PIDS) {
                    break;
                }
            }
            i = 0;
        } else {
            i++;
        }
    }

    close(fd);
    return pid_list;
}

void get_process_info(int pid, char *command, char *state) {
    char stat_path[BUFFER_SIZE];
    char stat_info[BUFFER_SIZE];
    sprintf(stat_path, "/proc/%d/stat", pid);
    
    // open /proc/[pid]/stat to read process info
    FILE *file = fopen(stat_path, "r");
    if (file == NULL) {
        strcpy(state, "stopped");
        strcpy(command, "N/A");
        return;
    }

    // Read the /proc/[pid]/stat file
    if (fgets(stat_info, sizeof(stat_info), file) != NULL) {
        // sscanf to extract the command and state from the stat_info
        // In /proc/[pid]/stat, the second field is the command, and the third field is the state
        sscanf(stat_info, "%*d (%[^)]) %c", command, state);  // %*d skips the first number (pid)
    }
    
    fclose(file);
}

int activities(int argc, char* argv[]) {
    if (argc != 1) {
        printf(RED_COLOR "INVALID USE OF ACTIVITIES\n" RESET_COLOR);
        return 1;
    }
    int pid_count = 0;
    char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
    memset(bg_history_fn, '\0', BUFFER_SIZE);
    sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
    int *pid_list = get_all_pids(bg_history_fn, &pid_count);
    char **commands = (char **)malloc(pid_count * sizeof(char *));
    char **states = (char **)malloc(pid_count * sizeof(char *));
    
    for (int i = 0; i < pid_count; i++) {
        commands[i] = (char *)malloc(BUFFER_SIZE * sizeof(char));
        states[i] = (char *)malloc(BUFFER_SIZE * sizeof(char));
        memset(states[i], '\0', BUFFER_SIZE);
        memset(commands[i], '\0', BUFFER_SIZE);
        get_process_info(pid_list[i], commands[i], states[i]);
        if (strcmp(states[i], "T") != 0) {
            strcpy(states[i], "Running");
        } else {
            strcpy(states[i], "Stopped");
        }
    }

    for (int i = 0; i < pid_count - 1; i++) {
        for (int j = i + 1; j < pid_count; j++) {
            if (strcmp(commands[i], commands[j]) > 0) {
                char *temp_command = commands[i];
                commands[i] = commands[j];
                commands[j] = temp_command;

                char *temp_state = states[i];
                states[i] = states[j];
                states[j] = temp_state;

                int temp_pid = pid_list[i];
                pid_list[i] = pid_list[j];
                pid_list[j] = temp_pid;
            }
        }
    }

    for (int i = 0; i < pid_count; i++) {
        if (strcmp(commands[i], "N/A") == 0) {
            continue;
        }
        printf("%d : %s - %s\n", pid_list[i], commands[i], states[i]);
    }

    for (int i = 0; i < pid_count; i++) {
        free(commands[i]);
        free(states[i]);
    }
    free(commands);
    free(states);
    free(bg_history_fn);
    free(pid_list);
}
