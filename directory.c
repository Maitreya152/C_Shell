#include "headers.h"

char *calc_relative_path (char *base, char *frontal) {
    char *relative_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(relative_path, '\0', BUFFER_SIZE);
    size_t base_len = strlen(base);
    if (strcmp(base, frontal) == 0) {
        // paths are identical
        strcpy(relative_path, "./");
    } else if (frontal[base_len] == '/') {
        // frontal is a subdirectory of base
        strcpy(relative_path, frontal + base_len + 1);
    } else {
        // frontal is the same as base but with additional path components
        strcpy(relative_path, frontal + base_len);
    }

    // if the relative path is empty, return "." to indicate current directory
    if (strlen(relative_path) == 0) {
        strcpy(relative_path, ".");
    }

    return relative_path;
    return relative_path;
}

void home_check(char* present_dir, char* home_dir) {
    int dir_position = strcmp(present_dir, home_dir);
    if (dir_position == 0) {
        snprintf(present_dir, BUFFER_SIZE, "~");
        return;
    } else {
        int result = strncmp(present_dir, home_dir, strlen(home_dir));
        if (result == 0) {
            char *extra_part = present_dir + strlen(home_dir);
            snprintf(present_dir, BUFFER_SIZE, "~%s", extra_part);
            return;
        } else {
            return;
        }
    }
}
