#include "headers.h"

char previous_dir[BUFFER_SIZE] = "";
char previous_command[BUFFER_SIZE] = "";
pid_t shell_pid = 0;
pid_t global_fg_pid = 0;
int log_stat = 1;
Alias *alias_list = NULL;
const char* space_delimeters = " \n\t\r\b\f\0";

char *trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int piping_handler(int argc1, char* argv1[], int argc2, char* argv2[]) {
    pid_t pid;
    pid_t pid2;
    int status;
    int fd[2];
    if(pipe(fd) < 0) {
        printf("ERROR: creating pipe failed\n");
        exit(1);
    }
    if ((pid = fork()) < 0) {
        printf("ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {
        // printf("EXECUTE 1\n");
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        // if (execvp(command1, argv1) < 0) {
        //     printf("ERROR: exec child process failed\n");
        //     exit(1);
        // }
        int result = execute(argc1, argv1, ';');
        exit(0);
        // execlp("ls", "ls", NULL);
    }
    else {
        if ((pid2 = fork()) < 0) {
            printf("ERROR: forking child process2 failed\n");
            exit(1);
        }
        else if (pid2 == 0) {
            // if (command2 != NULL) {
            // printf("EXECUTE 2\n");
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            // if (execvp(command2, argv2) < 0) {
            //     printf("ERROR: exec child process2 failed\n");
            //     exit(1);
            // }
            int result = execute(argc2, argv2, ';');
            exit(0);
            // execlp("grep", "grep", "txt", NULL);
            // }
        } else {
            // printf("END\n");
            close(fd[0]);
            close(fd[1]);
            while (wait(&status) != pid);
        }
    }
    fflush(stdout);
    // printf("END\n");
    return 0;
}

int inner_break = 0;
int tokenizer(char* command) {
    log_stat = 1;
    int start_command = 0;
    int end_command = 0;
    int result = 0;
    for (end_command = 0; end_command < strlen(command); end_command++) {
        int append_flag = 0;
        if (command[end_command] == '&' || command[end_command] == ';' || command[end_command] == '>' || command[end_command] == '<' || command[end_command] == '|') {
            if (command[end_command + 1] == '>' && command[end_command] == '>') {
                append_flag = 1;
                // end_command = end_command + 1;
                // printf("HI %s\n", command);
            }
            char *sub_command = malloc(sizeof(char) * BUFFER_SIZE);
            memset(sub_command, '\0', BUFFER_SIZE);
            strncpy(sub_command, command + start_command, end_command - start_command);

            int num_strings = strlen(sub_command);
            char **arguments = malloc(num_strings * sizeof(char *));
            memset(arguments, '\0', num_strings * sizeof(char *));
            int arg_count = 0;
            char* argument = strtok(sub_command, space_delimeters);
            while (argument != NULL) {
                arguments[arg_count] = malloc(strlen(argument) + 1);
                memset(arguments[arg_count], '\0', strlen(argument) + 1);
                strcpy(arguments[arg_count++], argument);
                argument = strtok(NULL, space_delimeters);
            }
            if (arg_count == 0) {
                return result;
            }
            if (strcmp(arguments[0], "log") == 0) {
                log_stat = 0;
            }
            if (command[end_command] == '&' || command[end_command] == ';') {
                result += execute(arg_count, arguments, command[end_command]);
            } else if (command[end_command] == '>' || command[end_command] == '<') {
                int end_command_dummy = 0;
                for (end_command_dummy = end_command + 2; end_command_dummy < strlen(command); end_command_dummy++) {
                    if (command[end_command_dummy] == '&' || command[end_command_dummy] == ';') {
                        if (command[end_command] == '>') {
                            char *file_name = malloc(sizeof(char) * BUFFER_SIZE);
                            memset(file_name, '\0', BUFFER_SIZE);
                            if (append_flag == 1) {
                                strncpy(file_name, command + end_command + 2, end_command_dummy - end_command - 2);
                            } else {
                                strncpy(file_name, command + end_command + 1, end_command_dummy - end_command - 1);
                            }
                            int save_out = dup(fileno(stdout));
                            if (save_out == -1) {
                                printf("Error duplicating stdout\n");
                            }
                            int fd_write = 0;
                            // printf("APPEND FLAG 1: %d\n", append_flag);
                            if (append_flag == 1) {
                                // printf("APPEND %s\n", command);
                                fd_write = open(trim(file_name), O_RDWR|O_CREAT|O_APPEND, 0644);
                            } else {
                                fd_write = open(trim(file_name), O_RDWR|O_CREAT|O_TRUNC, 0644);
                            }
                            if (-1 == fd_write) { 
                                printf("Error Opening File"); 
                            }
                            if (-1 == dup2(fd_write, fileno(stdout))) { 
                                perror("cannot redirect stdout"); return 255; 
                            }
                            result += execute(arg_count, arguments, command[end_command]);
                            if (-1 == dup2(save_out, fileno(stdout))) {
                                perror("cannot redirect stdout"); return 255;
                            }
                            free(file_name);
                            close(save_out);
                            close(fd_write);
                        } else if (command[end_command] == '<') {
                            char *file_name = malloc(sizeof(char) * BUFFER_SIZE);
                            memset(file_name, '\0', BUFFER_SIZE);
                            if (append_flag == 1) {
                                strncpy(file_name, command + end_command + 2, end_command_dummy - end_command - 2);
                            } else {
                                strncpy(file_name, command + end_command + 1, end_command_dummy - end_command - 1);
                            }
                            char character;
                            int fd_read = open(trim(file_name), O_RDONLY, 0644);
                            if (-1 == fd_read) {
                                printf("Error Opening File\n");
                            }
                            int save_in = dup(fileno(stdin));
                            if (save_in == -1) {
                                printf("Error Duplicating stdin");
                            }
                            if (-1 == dup2(fd_read, fileno(stdin))) { 
                                perror("cannot redirect stdin"); return 255; 
                            }
                            result += execute(arg_count, arguments, command[end_command]);
                            if (-1 == dup2(save_in, fileno(stdin))) {
                                perror("cannot redirect stdin"); return 255;
                            }
                            clearerr(stdin);
                            free(file_name);
                            close(save_in);
                            close(fd_read);
                        }
                        end_command = end_command_dummy + 1;
                        start_command = end_command_dummy;
                        break;
                    } else if (command[end_command_dummy] == '>') {
                        int append_flag2 = 0;
                        if (command[end_command_dummy + 1] == '>') {
                            append_flag2 = 1;
                        }
                        if (command[end_command] == '<') {
                            char *file_name = malloc(sizeof(char) * BUFFER_SIZE);
                            memset(file_name, '\0', BUFFER_SIZE);
                            if (append_flag == 1) {
                                strncpy(file_name, command + end_command + 2, end_command_dummy - end_command - 2);
                            } else {
                                strncpy(file_name, command + end_command + 1, end_command_dummy - end_command - 1);
                            }
                            char character;
                            int fd_read = open(trim(file_name), O_RDONLY, 0644);
                            if (-1 == fd_read) {
                                printf("Error Opening Filesadasd\n");
                            }
                            int save_in = dup(fileno(stdin));
                            if (save_in == -1) {
                                printf("Error Duplicating stdinasdasd");
                            }
                            if (-1 == dup2(fd_read, fileno(stdin))) { 
                                perror("cannot redirect stdin33"); return 255; 
                            }
                            // result += execute(arg_count, arguments, command[end_command]);
                            int end_command_dummy2 = 0;
                            for (end_command_dummy2 = end_command_dummy + 2; end_command_dummy2 < strlen(command); end_command_dummy2++) {
                                if (command[end_command_dummy2] == '&' || command[end_command_dummy2] == ';') {
                                    char *file_name2 = malloc(sizeof(char) * BUFFER_SIZE);
                                    memset(file_name2, '\0', BUFFER_SIZE);
                                    if (append_flag2 == 1) {
                                        strncpy(file_name2, command + end_command_dummy + 2, end_command_dummy2 - end_command_dummy - 2);
                                    } else {
                                        strncpy(file_name2, command + end_command_dummy + 1, end_command_dummy2 - end_command_dummy - 1);
                                    }
                                    // strncpy(file_name2, command + end_command_dummy + 1, end_command_dummy2 - end_command_dummy - 1);
                                    int save_out = dup(fileno(stdout));
                                    if (save_out == -1) {
                                        printf("Error duplicating stdout\n");
                                    }
                                    int fd_write = 0;
                                    // printf("APPEND FLAG 1: %d\n", append_flag);
                                    if (append_flag2 == 1) {
                                        // printf("APPEND %s\n", command);
                                        fd_write = open(trim(file_name2), O_RDWR|O_CREAT|O_APPEND, 0644);
                                    } else {
                                        fd_write = open(trim(file_name2), O_RDWR|O_CREAT|O_TRUNC, 0644);
                                    }
                                    if (-1 == fd_write) { 
                                        printf("Error Opening File"); 
                                    }
                                    if (-1 == dup2(fd_write, fileno(stdout))) { 
                                        perror("cannot redirect stdout"); return 255; 
                                    }
                                    result += execute(arg_count, arguments, command[end_command]);
                                    if (-1 == dup2(save_out, fileno(stdout))) {
                                        perror("cannot redirect stdout"); return 255;
                                    }
                                    free(file_name2);
                                    close(save_out);
                                    close(fd_write);
                                    start_command = end_command_dummy2 + 1;
                                    end_command = end_command_dummy2;
                                    inner_break = 1;
                                    if (-1 == dup2(save_in, fileno(stdin))) {
                                        perror("cannot redirect stdin"); return 255;
                                    }
                                    clearerr(stdin);
                                    free(file_name);
                                    close(save_in);
                                    close(fd_read);
                                    break;
                                } else if (command[end_command_dummy2] == '>' || command[end_command_dummy2] == '<' || command[end_command_dummy2] == '|') {
                                    printf(RED_COLOR"INVALID SEQUENCE OF I/O REDIRECTION\n" RESET_COLOR);
                                    return 0;
                                }
                            }
                            if (inner_break == 1) {
                                inner_break = 0;
                                break;
                            }
                            if (end_command_dummy < end_command_dummy2) {
                                char *file_name2 = malloc(sizeof(char) * BUFFER_SIZE);
                                memset(file_name2, '\0', BUFFER_SIZE);
                                if (append_flag2 == 1) {
                                    strncpy(file_name2, command + end_command_dummy + 2, end_command_dummy2 - end_command_dummy - 2);
                                } else {
                                    strncpy(file_name2, command + end_command_dummy + 1, end_command_dummy2 - end_command_dummy - 1);
                                }
                                // strncpy(file_name2, command + end_command_dummy + 1, end_command_dummy2 - end_command_dummy - 1);
                                int save_out = dup(fileno(stdout));
                                if (save_out == -1) {
                                    printf("Error duplicating stdout\n");
                                }
                                int fd_write = 0;
                                // printf("APPEND FLAG 1: %d\n", append_flag);
                                if (append_flag2 == 1) {
                                    // printf("APPEND %s\n", command);
                                    fd_write = open(trim(file_name2), O_RDWR|O_CREAT|O_APPEND, 0644);
                                } else {
                                    fd_write = open(trim(file_name2), O_RDWR|O_CREAT|O_TRUNC, 0644);
                                }
                                if (-1 == fd_write) { 
                                    printf("Error Opening File"); 
                                }
                                if (-1 == dup2(fd_write, fileno(stdout))) { 
                                    perror("cannot redirect stdout"); return 255; 
                                }
                                result += execute(arg_count, arguments, command[end_command]);
                                if (-1 == dup2(save_out, fileno(stdout))) {
                                    perror("cannot redirect stdout"); return 255;
                                }
                                free(file_name2);
                                close(save_out);
                                close(fd_write);
                                start_command = end_command_dummy2 + 1;
                                end_command = end_command_dummy2;
                                if (-1 == dup2(save_in, fileno(stdin))) {
                                    perror("cannot redirect stdin"); return 255;
                                }
                                clearerr(stdin);
                                free(file_name);
                                close(save_in);
                                close(fd_read);
                                return result;
                            }
                            if (-1 == dup2(save_in, fileno(stdin))) {
                                perror("cannot redirect stdin2"); return 255;
                            }
                            clearerr(stdin);
                            free(file_name);
                            close(save_in);
                            close(fd_read);
                        } else {
                            printf(RED_COLOR"INVALID SEQUENCE OF I/O REDIRECTION\n" RESET_COLOR);
                            return 0;
                        }
                    } else if (command[end_command_dummy] == '<') {
                        printf(RED_COLOR"INVALID SEQUENCE OF I/O REDIRECTION\n" RESET_COLOR);
                        return 0;
                    }
                    // end_command = end_command_dummy + 1;
                    // start_command = end_command_dummy + 1;
                }
                // if (inner_break == 1) {
                //     inner_break = 0;
                //     break;
                // }
                if (end_command < end_command_dummy) {
                    if (command[end_command] == '>') {
                        char *file_name = malloc(sizeof(char) * BUFFER_SIZE);
                        memset(file_name, '\0', BUFFER_SIZE);
                        if (append_flag == 1) {
                            strncpy(file_name, command + end_command + 2, end_command_dummy - end_command - 2);
                        } else {
                            strncpy(file_name, command + end_command + 1, end_command_dummy - end_command - 1);
                        }
                        int save_out = dup(fileno(stdout));
                        if (save_out == -1) {
                            printf("Error duplicating stdout\n");
                        }
                        int fd_write = 0;
                        if (append_flag == 1) {
                            fd_write = open(trim(file_name), O_RDWR|O_CREAT|O_APPEND, 0644);
                        } else {
                            fd_write = open(trim(file_name), O_RDWR|O_CREAT|O_TRUNC, 0644);
                        }
                        if (-1 == fd_write) { 
                            printf("Error Opening File"); 
                        }
                        if (-1 == dup2(fd_write, fileno(stdout))) {
                            perror("cannot redirect stdout"); return 255; 
                        }
                        result += execute(arg_count, arguments, command[end_command]);
                        if (-1 == dup2(save_out, fileno(stdout))) {
                            perror("cannot redirect stdout"); return 255;
                        }
                        free(file_name);
                        close(save_out);
                        close(fd_write);
                    } else if (command[end_command] == '<') {
                        char *file_name = malloc(sizeof(char) * BUFFER_SIZE);
                        memset(file_name, '\0', BUFFER_SIZE);
                        if (append_flag == 1) {
                            strncpy(file_name, command + end_command + 2, end_command_dummy - end_command - 2);
                        } else {
                            strncpy(file_name, command + end_command + 1, end_command_dummy - end_command - 1);
                        }
                        char character;
                        int fd_read = open(trim(file_name), O_RDONLY, 0644);
                        if (-1 == fd_read) {
                            printf("Error Opening File\n");
                        }
                        int save_in = dup(fileno(stdin));
                        if (save_in == -1) {
                            printf("Error Duplicating stdin");
                        }
                        if (-1 == dup2(fd_read, fileno(stdin))) { 
                            perror("cannot redirect stdin"); return 255; 
                        }
                        result += execute(arg_count, arguments, command[end_command]);
                        if (-1 == dup2(save_in, fileno(stdin))) {
                            perror("cannot redirect stdin"); return 255;
                        }
                        clearerr(stdin);
                        free(file_name);
                        close(save_in);
                        close(fd_read);
                    }
                    end_command = end_command_dummy;
                    start_command = end_command_dummy + 1;
                }
            } else if (command[end_command] == '|') {
                // printf("HIHI\n");
                int end_command_dummy = 0;
                for (end_command_dummy = end_command + 1; end_command_dummy < strlen(command); end_command_dummy++) {
                    // printf("%c hi\n", command[end_command_dummy]);
                    if (command[end_command_dummy] == '&' || command[end_command_dummy] == ';' || command[end_command_dummy] == '\n') {
                        // printf("HIHI NEW\n");
                        char *sub_command2 = malloc(sizeof(char) * BUFFER_SIZE);
                        memset(sub_command2, '\0', BUFFER_SIZE);
                        strncpy(sub_command2, command + end_command + 1, end_command_dummy - end_command - 1);

                        int num_strings2 = strlen(sub_command2);
                        char **arguments2 = malloc(num_strings2 * sizeof(char *));
                        memset(arguments2, '\0', num_strings2 * sizeof(char *));
                        int arg_count2 = 0;
                        char* argument2 = strtok(sub_command2, space_delimeters);
                        while (argument2 != NULL) {
                            arguments2[arg_count2] = malloc(strlen(argument2) + 1);
                            memset(arguments2[arg_count2], '\0', strlen(argument2) + 1);
                            strcpy(arguments2[arg_count2++], argument2);
                            argument2 = strtok(NULL, space_delimeters);
                        }
                        if (arg_count2 == 0) {
                            printf(RED_COLOR "INVALID USE OF PIPE\n" RESET_COLOR);
                            return 0;
                        }
                        if (strcmp(arguments2[0], "log") == 0) {
                            log_stat = 0;
                        }
                        // printf("HIHI\n");
                        int pipe_result = piping_handler(arg_count, arguments, arg_count2, arguments2);
                        for (int i = 0; i < num_strings2; i++) {
                            free(arguments2[i]);
                        }
                        free(arguments2);
                        free(sub_command2);
                        start_command = end_command_dummy;
                        end_command = end_command_dummy + 1;
                        for (int i = 0; i < num_strings; i++) {
                            free(arguments[i]);
                        }
                        free(arguments);
                        free(sub_command);
                        start_command = end_command + 1;
                        return 0;
                    }
                }
                if (end_command < end_command_dummy) {
                    // printf("HELLO\n");
                    char *sub_command2 = malloc(sizeof(char) * BUFFER_SIZE);
                    memset(sub_command2, '\0', BUFFER_SIZE);
                    strncpy(sub_command2, command + end_command + 1, end_command_dummy - end_command - 1);

                    int num_strings2 = strlen(sub_command2);
                    char **arguments2 = malloc(num_strings2 * sizeof(char *));
                    memset(arguments2, '\0', num_strings2 * sizeof(char *));
                    int arg_count2 = 0;
                    char* argument2 = strtok(sub_command2, space_delimeters);
                    while (argument2 != NULL) {
                        arguments2[arg_count2] = malloc(strlen(argument2) + 1);
                        memset(arguments2[arg_count2], '\0', strlen(argument2) + 1);
                        strcpy(arguments2[arg_count2++], argument2);
                        argument2 = strtok(NULL, space_delimeters);
                    }
                    if (arg_count2 == 0) {
                        printf(RED_COLOR "INVALID USE OF PIPE\n" RESET_COLOR);
                        return 0;
                    }
                    if (strcmp(arguments2[0], "log") == 0) {
                        log_stat = 0;
                    }
                    // printf("HIHI\n");
                    int pipe_result = piping_handler(arg_count, arguments, arg_count2, arguments2);

                    for (int i = 0; i < num_strings2; i++) {
                        free(arguments2[i]);
                    }
                    free(arguments2);
                    free(sub_command2);
                    start_command = end_command_dummy;
                    end_command = end_command_dummy + 1;
                    for (int i = 0; i < num_strings; i++) {
                        free(arguments[i]);
                    }
                    free(arguments);
                    free(sub_command);
                    start_command = end_command + 1;
                    return 0;
                }
            }
            // if (result > 2) {
            //     return result;
            // }
            for (int i = 0; i < num_strings; i++) {
                free(arguments[i]);
            }
            
            free(arguments);
            free(sub_command);
            start_command = end_command + 1;
        }
    }
    if (start_command < end_command) {
        char *sub_command = malloc(sizeof(char) * 4096);
        memset(sub_command, '\0', 4096);
        strncpy(sub_command, command + start_command, end_command - start_command);
        int num_strings = strlen(sub_command);
        char **arguments = malloc(num_strings * sizeof(char *));
        memset(arguments, '\0', num_strings * sizeof(char *));
        int arg_count = 0;
        char* argument = strtok(sub_command, space_delimeters);
        while (argument != NULL) {
            arguments[arg_count] = malloc(strlen(argument) + 1);
            memset(arguments[arg_count], '\0', strlen(argument) + 1);
            strcpy(arguments[arg_count++], argument);
            argument = strtok(NULL, space_delimeters);
        }
        if (arg_count == 0) {
            return result;
        }
        if (strcmp(arguments[0], "log") == 0) {
            log_stat = 0;
        }
        result += execute(arg_count, arguments, command[end_command]);
        // if (result > 2) {
        //     return result;
        // }
        for (int i = 0; i < num_strings; i++) {
            free(arguments[i]);
        }
        free(sub_command);
        free(arguments);
    }
    return result;
}

void print_prompt(int tok_result) {
    if (getcwd(present_dir, BUFFER_SIZE) == NULL) {
        printf(RED_COLOR "Error: Could not store present working directory\n" RESET_COLOR);
        return;
    }
    home_check(present_dir, global_init_home);
    if (tok_result > 2) {
        if (strstr(previous_command, "&") != NULL) {
            printf("<"GREEN_COLOR"%s" "@" "%s"RESET_COLOR":"BLUE_COLOR"%s"RESET_COLOR"> ", user_name, sys_name, present_dir);
        } else {
            printf("<"GREEN_COLOR"%s" "@" "%s"RESET_COLOR":"BLUE_COLOR"%s"RESET_COLOR" %s : %ds> ", user_name, sys_name, present_dir, previous_command, tok_result);
        }
    } else {
        // printf("HERHERHE2\n");
        // exit(0);
        printf("<"GREEN_COLOR"%s" "@" "%s"RESET_COLOR":"BLUE_COLOR"%s"RESET_COLOR"> ", user_name, sys_name, present_dir);
    }
}

void handle_sigint(int sig) {
    if (global_fg_pid > 0) {
        kill(global_fg_pid, SIGINT);
        printf("\n");
        global_fg_pid = 0;
        // print_prompt(0);
        // fflush(stdout);
    } else {
        printf("\n");
        print_prompt(0);
        fflush(stdout);
    }
}

void handle_sigtstp(int sig) {
    if (global_fg_pid > 0) {
        kill(global_fg_pid, SIGTSTP);
        printf("\n");
        char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
        memset(bg_history_fn, '\0', BUFFER_SIZE);
        sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
        int pid_count = 0;
        int* pid_list = get_all_pids(bg_history_fn, &pid_count);
        int found_flag = 0;
        for (int i = 0; i < pid_count; i++) {
            if (global_fg_pid == pid_list[i]) {
                found_flag = 1;
                break;
            }
        }
        if (found_flag == 0) {
            int bg_file = open(bg_history_fn, O_WRONLY | O_CREAT | O_APPEND, 0644);
            sprintf(bg_history_fn, "%d: %s\n", global_fg_pid, "Jon Doe");
            write(bg_file, bg_history_fn, strlen(bg_history_fn));
            close(bg_file);
        }
        free(bg_history_fn);
        global_fg_pid = 0;
    } else {
        printf("\n");
        print_prompt(0);
        fflush(stdout);
    }
}

void logout() {
    printf("\n");
    char* bg_history_fn = malloc(BUFFER_SIZE * sizeof(char));
    memset(bg_history_fn, '\0', BUFFER_SIZE);
    sprintf(bg_history_fn, "%s/%s", global_init_home, "bg_history.txt");
    int pid_count = 0;
    int* pid_list = get_all_pids(bg_history_fn, &pid_count);
    for (int i = 0; i < pid_count; i++) {
        kill(pid_list[i], SIGKILL);
    }
    free(bg_history_fn);
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    shell_pid = getpid();
    if (getcwd(global_init_home, BUFFER_SIZE) == NULL) {
        printf(RED_COLOR "Error: Could not store present working directory\n" RESET_COLOR);
        return 1;
    }
    // printf("YES\n");
    init_last_command();
    // printf("YES\n");
    int init_status = initiate_shell(); // add .myshrc file thing
    if (init_status != 0) {
        printf(RED_COLOR "Error initialising shell\n" RESET_COLOR);
        return 1;
    }
    int tok_result = 0;
    while(1) {
        print_prompt(tok_result);
        char *command = malloc(sizeof(char) * BUFFER_SIZE);
        memset(command, '\0', BUFFER_SIZE);
        if (fgets(command, BUFFER_SIZE, stdin) == NULL) {
            logout();
        }
        command[strlen(command) - 1] = '\0';
        expand_alias(command);
        // printf("Command: %s\n", command);
        // exit(0);
        tok_result = tokenizer(command);
        // printf("LOG STAT: %d\n", log_stat);
        if (log_stat == 1) {
            // printf("PREV CMD: %s\n", previous_command);
            if (strcmp(previous_command, "") == 0) {
                strcpy(previous_command, command);
                add_to_log(command);
            } else {
                if (strcmp(previous_command, command) != 0) {
                    strcpy(previous_command, command);
                    // printf("HI\n");
                    add_to_log(command);
                }
            }
        }
        free(command);
    }
    return 0;
}