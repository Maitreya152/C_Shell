#include "headers.h"

int lines_in_file(int file_descriptor) {
    lseek(file_descriptor, 0, SEEK_SET);
    char character;
    int lines = 0;
    ssize_t bytes_read;
    while((bytes_read = read(file_descriptor, &character, sizeof(char))) > 0) {
        if (character == '\n') {
            lines++;
        }
    }
    return lines;
}

void del_first_line(char* log_file_path) {
    int file_descriptor = open(log_file_path, O_RDWR | O_CREAT | O_APPEND, 0644);
    char** file_content = malloc(14 * sizeof(char*));
    memset(file_content, '\0', 14 * sizeof(char*));
    for (int i = 0; i < 14; i++) {
        file_content[i] = malloc((BUFFER_SIZE + 1) * sizeof(char));
        memset(file_content[i], '\0', (BUFFER_SIZE + 1) * sizeof(char));
    }
    ssize_t bytes_read;
    char character;
    int lines = 0;
    int char_index = 0;
    while((bytes_read = read(file_descriptor, &character, sizeof(char))) > 0) {
        if (lines != 0) {
            file_content[lines - 1][char_index++] = character;
        }
        if (character == '\n') {
            if (lines != 0) {
                file_content[lines - 1][char_index] = '\0';
                char_index = 0;
            }
            lines++;
        }
    }
    close(file_descriptor);
    file_descriptor = open(log_file_path, O_WRONLY | O_TRUNC, 0644);
    for (int i = 0; i < 14; i++) {
        write(file_descriptor, file_content[i], strlen(file_content[i]));
    }
    close(file_descriptor);
    for (int i = 0; i < 14; i++) {
        free(file_content[i]);
    }
    free(file_content);
    return;
}

void add_to_log(char* command) {
    char* log_file_path = malloc(BUFFER_SIZE * sizeof(char));
    memset(log_file_path, '\0', BUFFER_SIZE);
    sprintf(log_file_path, "%s/%s", global_init_home, "log.txt");
    int file_descriptor_main = open(log_file_path, O_RDWR | O_CREAT | O_APPEND, 0644);
    int lines_no = lines_in_file(file_descriptor_main);
    char new_line = '\n';
    if (lines_no < 15) {
        write(file_descriptor_main, command, strlen(command));
        write(file_descriptor_main, &new_line, sizeof(char));
        close(file_descriptor_main);
        free(log_file_path);
        return;
    } else {
        del_first_line(log_file_path);
        write(file_descriptor_main, command, strlen(command));
        write(file_descriptor_main, &new_line, sizeof(char));
        close(file_descriptor_main);
        free(log_file_path);
        return;
    }
}

void log_display() {
    char* log_file_path = malloc(BUFFER_SIZE * sizeof(char));
    memset(log_file_path, '\0', BUFFER_SIZE);
    sprintf(log_file_path, "%s/%s", global_init_home, "log.txt");
    int file_descriptor = open(log_file_path, O_RDONLY, 0644);
    char** file_content = malloc(15 * sizeof(char*));
    memset(file_content, '\0', 15 * sizeof(char*));
    for (int i = 0; i < 15; i++) {
        file_content[i] = malloc((BUFFER_SIZE + 1) * sizeof(char));
        memset(file_content[i], '\0', (BUFFER_SIZE + 1) * sizeof(char));
    }
    ssize_t bytes_read;
    char character;
    int lines = 0;
    int char_index = 0;
    while((bytes_read = read(file_descriptor, &character, sizeof(char))) > 0) {
        file_content[lines][char_index++] = character;
        // printf("%c", character);
        if (character == '\n') {
            file_content[lines][char_index] = '\0';
            char_index = 0;
            lines++;
        }
    }
    for (int i = 0; i < 15; i++) {
        printf(GRAY_COLOR "%s" RESET_COLOR, file_content[i]);
    }
    for (int i = 0; i < 15; i++) {
        free(file_content[i]);
    }
    free(file_content);
    free(log_file_path);
    close(file_descriptor);
    return;
}

void init_last_command() {
    // printf("YES\n");
    char* log_file_path = malloc(BUFFER_SIZE * sizeof(char));
    memset(log_file_path, '\0', BUFFER_SIZE);
    sprintf(log_file_path, "%s/%s", global_init_home, "log.txt");
    // printf("YES\n");
    int file_descriptor = open(log_file_path, O_RDONLY, 0644);
    char** file_content = malloc(15 * sizeof(char*));
    memset(file_content, '\0', 15 * sizeof(char*));
    for (int i = 0; i < 15; i++) {
        file_content[i] = malloc((BUFFER_SIZE + 1) * sizeof(char));
        memset(file_content[i], '\0', (BUFFER_SIZE + 1) * sizeof(char));
    }
    ssize_t bytes_read;
    char character;
    int lines = 0;
    int char_index = 0;
    // printf("YES\n");
    while((bytes_read = read(file_descriptor, &character, sizeof(char))) > 0) {
        file_content[lines][char_index++] = character;
        if (character == '\n') {
            file_content[lines][char_index - 1] = '\0';
            char_index = 0;
            lines++;
        }
    }
    // printf("YES\n");
    if (file_content[0][0] != '\0') {
        strcpy(previous_command, file_content[lines - 1]);
    }
    for (int i = 0; i < 15; i++) {
        free(file_content[i]);
    }
    free(file_content);
    free(log_file_path);
    close(file_descriptor);
    return;
}

void log_purge() {
    char* log_file_path = malloc(BUFFER_SIZE * sizeof(char));
    memset(log_file_path, '\0', BUFFER_SIZE);
    sprintf(log_file_path, "%s/%s", global_init_home, "log.txt");
    int file_descriptor = open(log_file_path, O_WRONLY | O_TRUNC, 0644);
    close(file_descriptor);
    free(log_file_path);
}

void log_tokenizer(char* command) {
    const char* space_delimeters = " \n\t\r\b\f\0";
    int start_command = 0;
    int end_command = 0;
    for (end_command = 0; end_command < strlen(command); end_command++) {
        if (command[end_command] == '&' || command[end_command] == ';') {

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
            int result = execute(arg_count, arguments, command[end_command]);
            for (int i = 0; i < num_strings; i++) {
                free(arguments[i]);
            }
            free(arguments);
            free(sub_command);
            start_command = end_command + 1;
        }
    }
    if (start_command != end_command) {
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
        // printf("HELLO\n");
        int result = execute(arg_count, arguments, command[end_command]);
        for (int i = 0; i < num_strings; i++) {
            free(arguments[i]);
        }
        free(sub_command);
        free(arguments);
    }
}

void log_execute(char* index) {
    int int_idx = atoi(index);
    if (int_idx > 15) {
        printf(RED_COLOR "Log only stores 15 commands\n" RESET_COLOR);
        return;
    }
    char* log_file_path = malloc(BUFFER_SIZE * sizeof(char));
    memset(log_file_path, '\0', BUFFER_SIZE);
    sprintf(log_file_path, "%s/%s", global_init_home, "log.txt");
    int file_descriptor = open(log_file_path, O_RDONLY, 0644);
    char** file_content = malloc(15 * sizeof(char*));
    memset(file_content, '\0', 15 * sizeof(char*));
    for (int i = 0; i < 15; i++) {
        file_content[i] = malloc((BUFFER_SIZE + 1) * sizeof(char));
        memset(file_content[i], '\0', (BUFFER_SIZE + 1) * sizeof(char));
    }
    ssize_t bytes_read;
    char character;
    int lines = 0;
    int char_index = 0;
    while((bytes_read = read(file_descriptor, &character, sizeof(char))) > 0) {
        file_content[lines][char_index++] = character;
        // printf("%c", character);
        if (character == '\n') {
            file_content[lines][char_index] = '\0';
            char_index = 0;
            lines++;
        }
    }
    if (int_idx > lines) {
        printf(RED_COLOR "Log currently has only %d commands\n" RESET_COLOR, lines);
        return;
    }
    log_tokenizer(file_content[lines - int_idx]);
    for (int i = 0; i < 15; i++) {
        free(file_content[i]);
    }
    free(file_content);
    free(log_file_path);
    close(file_descriptor);
    return;
}