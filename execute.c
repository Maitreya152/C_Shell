#include "headers.h"

char* find_command_by_pid(const char *filename, int p_id) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    char *command = NULL;
    int file_pid;
    ssize_t bytes_read;
    int i = 0, line_start = 0;

    while ((bytes_read = read(fd, buffer + i, 1)) > 0) {
        if (buffer[i] == '\n' || i == BUFFER_SIZE - 1) {
            buffer[i] = '\0';
            // parse the process ID from the line
            if (sscanf(buffer + line_start, "%d:", &file_pid) == 1) {
                if (file_pid == p_id) {
                    // find the command after the colon
                    command = strchr(buffer + line_start, ':');
                    if (command != NULL) {
                        command += 2;  // skips ": "
                        close(fd);
                        return strdup(command);
                    }
                }
            }
            i = 0;
        } else {
            i++;
        }
    }
    close(fd);
    return NULL;
}

void background_process_termination_handler(int sig) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
            memset(bg_history_fn, '\0', BUFFER_SIZE);
            char* temp_command = malloc(BUFFER_SIZE * sizeof(char));
            memset(temp_command, '\0', BUFFER_SIZE);
            sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
            temp_command = find_command_by_pid(bg_history_fn, pid);
            printf("Background process %s (%d) exited with status %d.\n", temp_command, pid, WEXITSTATUS(status));
            free(bg_history_fn);
            free(temp_command);
        } else if (WIFSIGNALED(status)) {
            char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
            memset(bg_history_fn, '\0', BUFFER_SIZE);
            char* temp_command = malloc(BUFFER_SIZE * sizeof(char));
            memset(temp_command, '\0', BUFFER_SIZE);
            sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
            temp_command = find_command_by_pid(bg_history_fn, pid);
            printf("Background process %s (%d) was terminated by signal %d.\n", previous_command, pid, WTERMSIG(status));
            free(bg_history_fn);
            free(temp_command);
        }
    }
}

int execute(int argc, char* argv[], char seperator) {
    if (argc == 0) {
        return 0;
    } else if (strcmp(argv[0], "hop") == 0) {
        int hop_result = hop(argc, argv);
    } else if (strcmp(argv[0], "reveal") == 0) {
        int reveal_result = reveal(argc, argv);
    } else if (strcmp(argv[0], "log") == 0) {
        if (argc > 3) {
            printf(RED_COLOR "Invalid arguments to log command\n" RESET_COLOR);
        }
        if (argc == 1) {
            log_display();
        } else if ((strcmp(argv[1], "purge") == 0) && (argc == 2)) {
            log_purge();
        } else if ((strcmp(argv[1], "execute") == 0) && (argc == 3)) {
            log_execute(argv[2]);
        }
    } else if (strcmp(argv[0], "proclore") == 0) {
        int proclore_result = proclore(argc, argv);
    } else if (strcmp(argv[0], "seek") == 0) {
        int seek_result = seek(argc, argv);
    } else if (strcmp(argv[0], "iMan") == 0) {
        int iman_result = iMan(argc, argv);
    } else if (strcmp(argv[0], "activities") == 0) {
        int activites_result = activities(argc, argv);
    } else if (strcmp(argv[0], "neonate") == 0) {
        int neonate_result = neonate(argc, argv);
    } else if (strcmp(argv[0], "fg") == 0) {
        int fg_result = fg(argc, argv);
    } else if (strcmp(argv[0], "bg") == 0) {
        int fg_result = bg(argc, argv);
    } else if (strcmp(argv[0], "ping") == 0) {
        int ping_result = ping(argc, argv);
    } else {
        char **exec_args = malloc((argc + 1) * sizeof(char *));
        for (int i = 0; i < (argc + 1); i++) {
            exec_args[i] = malloc(BUFFER_SIZE);
            memset(exec_args[i], '\0', BUFFER_SIZE);
        }
        strcpy(exec_args[0], argv[0]);
        for (int i = 1; i < argc; i++) {
            strcpy(exec_args[i], argv[i]);
        }
        exec_args[argc] = NULL;
        signal(SIGCHLD, background_process_termination_handler);
        pid_t fork_id = fork();
        if (fork_id == 0) {

            // Child process
            // redirect stdin, stdout, and stderr to /dev/null for background process
            if (seperator == '&') {
                int dev_null = open("/dev/null", O_RDWR);
                if (dev_null == -1) {
                    perror("Failed to open /dev/null");
                    exit(1);
                }
                dup2(dev_null, STDIN_FILENO);  // Redirect stdin
                dup2(dev_null, STDOUT_FILENO); // Redirect stdout
                dup2(dev_null, STDERR_FILENO); // Redirect stderr
                close(dev_null);
            }

            execvp(exec_args[0], exec_args);
            printf(RED_COLOR "INVALID COMMAND\n" RESET_COLOR);
            exit(1);
        } else if (fork_id > 0) {
            // fork_id will be the id of the child process.
            if (seperator != '&') {
                // foreground
                global_fg_pid = fork_id;
                time_t start_time = time(NULL);
                int status;
                waitpid(fork_id, &status, WUNTRACED);
                time_t end_time = time(NULL);
                global_fg_pid = 0;
                if (((end_time - start_time) / 1) > 2) {
                    for (int i = 0; i < (argc + 1); i++) {
                        free(exec_args[i]);
                    }
                    free(exec_args);
                    return ((end_time - start_time) / 1);
                }
            } else {
                // background
                char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
                memset(bg_history_fn, '\0', BUFFER_SIZE);
                sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
                int bg_file = open(bg_history_fn, O_WRONLY | O_CREAT | O_APPEND, 0644);
                sprintf(bg_history_fn, "%d: %s\n", fork_id, exec_args[0]);
                write(bg_file, bg_history_fn, strlen(bg_history_fn));
                printf("%d\n", fork_id); // fork_id in parent will be the id of the child process
                close(bg_file);
                free(bg_history_fn);
            }
        }
        for (int i = 0; i < (argc + 1); i++) {
            free(exec_args[i]);
        }
        free(exec_args);
        return 0;
    }
    return 0; // successful run
}
