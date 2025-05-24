#include "headers.h"

int fg (int argc, char* argv[]) {
    if (argc != 2) {
        printf(RED_COLOR "INVALID FORMAT FOR FG\n" RESET_COLOR);
        return 1;
    }
    pid_t fg_pid = atoi(argv[1]);
    int pid_count = 0;
    char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
    memset(bg_history_fn, '\0', BUFFER_SIZE);
    sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
    int *pid_list = get_all_pids(bg_history_fn, &pid_count);
    char **commands = (char **)malloc(pid_count * sizeof(char *));
    char **states = (char **)malloc(pid_count * sizeof(char *));
    memset(commands, '\0', pid_count * sizeof(char*));
    memset(states, '\0', pid_count * sizeof(char*));
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
    int found_flag = 0;
    for (int i = 0; i < pid_count; i++) {
        if (fg_pid == pid_list[i]) {
            if (strcmp(states[i], "Stopped") == 0) {
                if (kill(fg_pid, SIGCONT) == -1) {
                    printf(RED_COLOR "No such process found\n" RESET_COLOR);
                    return 1;
                }
                printf("Process with pid %d brought to foreground\n", fg_pid);
                global_fg_pid = fg_pid;
                int status;
                if (waitpid(fg_pid, &status, WUNTRACED) == -1) {
                    perror("waitpid");
                    return 1;
                }
                global_fg_pid = 0;
                found_flag = 1;
            } else if (strcmp(states[i], "Running") == 0) {
                printf("Process with pid %d brought to foreground\n", fg_pid);
                global_fg_pid = fg_pid;
                int status;
                if (waitpid(fg_pid, &status, WUNTRACED) == -1) {
                    perror("waitpid");
                    return 1;
                }
                global_fg_pid = 0;
                found_flag = 1;
            } else {
                printf(RED_COLOR "No such process found\n" RESET_COLOR);
                return 0;
            }
        }
    }
    if (found_flag == 0) {
        printf(RED_COLOR "No such process found\n" RESET_COLOR);
    }
    for (int i = 0; i < pid_count; i++) {
        free(commands[i]);
        free(states[i]);
    }
    free(commands);
    free(states);
    free(bg_history_fn);
    free(pid_list);
    return 0;
}

int bg (int argc, char* argv[]) {
    if (argc != 2) {
        printf(RED_COLOR "INVALID FORMAT FOR BG\n" RESET_COLOR);
        return 1;
    }
    pid_t bg_pid = atoi(argv[1]);
    int pid_count = 0;
    char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
    memset(bg_history_fn, '\0', BUFFER_SIZE);
    sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
    int *pid_list = get_all_pids(bg_history_fn, &pid_count);
    char **commands = (char **)malloc(pid_count * sizeof(char *));
    char **states = (char **)malloc(pid_count * sizeof(char *));
    memset(commands, '\0', pid_count * sizeof(char*));
    memset(states, '\0', pid_count * sizeof(char*));
    for (int i = 0; i < pid_count; i++) {
        commands[i] = (char *)malloc(BUFFER_SIZE * sizeof(char));
        states[i] = (char *)malloc(BUFFER_SIZE * sizeof(char));
        memset(states[i], '\0', BUFFER_SIZE);
        memset(commands[i], '\0', BUFFER_SIZE);
        // Get process info
        get_process_info(pid_list[i], commands[i], states[i]);
        if (strcmp(states[i], "T") != 0) {
            strcpy(states[i], "Running");
        } else {
            strcpy(states[i], "Stopped");
        }
    }
    int found_flag = 0;
    for (int i = 0; i < pid_count; i++) {
        if (bg_pid == pid_list[i]) {
            if (strcmp(states[i], "Stopped") == 0) {
                if (kill(bg_pid, SIGCONT) == -1) {
                    printf(RED_COLOR "No such process found\n" RESET_COLOR);
                    return 1;
                }
                printf("Process with pid %d resumed in background\n", bg_pid);
                int status;
                found_flag = 1;
            } else if (strcmp(states[i], "Running") == 0) {
                printf("Process with pid %d is already running in background\n", bg_pid);
                int status;
                found_flag = 1;
            } else {
                printf(RED_COLOR "No such process found\n" RESET_COLOR);
                return 0;
            }
        }
    }
    if (found_flag == 0) {
        printf(RED_COLOR "No such process found\n" RESET_COLOR);
    }
    for (int i = 0; i < pid_count; i++) {
        free(commands[i]);
        free(states[i]);
    }
    free(commands);
    free(states);
    free(bg_history_fn);
    free(pid_list);
    return 0;
}
