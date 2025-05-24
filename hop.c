#include "headers.h"

int hop(int argc, char* argv[]) {
    if ((argc == 1) || (strcmp(argv[1], "~") == 0)) {
        if (getcwd(previous_dir, BUFFER_SIZE) == NULL) {
            printf(RED_COLOR "Error: Could not store present working directory\n" RESET_COLOR);
            return 1;
        }
        if (chdir(global_init_home) != 0) {
            printf(RED_COLOR "Error: Could not hop to directory: " WHITE_COLOR "%s\n" RESET_COLOR, global_init_home);
            return 1;
        }
        printf(CYAN_COLOR "%s\n" RESET_COLOR, global_init_home);
        return 0;
    } else {
        for (int i = 1; i < argc; i ++) {
            if (strcmp(argv[i], "-") == 0) {
                if (strcmp(previous_dir, "") == 0) {
                    printf(RED_COLOR "Error: Previous Directory not Defined\n" RESET_COLOR);
                    return 1;
                } else {
                    char *previous_dir_dummy = malloc(sizeof(char) * 4096);
                    memset(previous_dir_dummy, '\0', 4096);
                    if (getcwd(previous_dir_dummy, BUFFER_SIZE) == NULL) {
                        printf(RED_COLOR "Error: Could not store present working directory\n" RESET_COLOR);
                        return 1;
                    }
                    if (chdir(previous_dir) != 0) {
                        printf(RED_COLOR "Error: Could not hop to directory: " WHITE_COLOR "%s\n" RESET_COLOR, previous_dir);
                        return 1;
                    }
                    strcpy(previous_dir, previous_dir_dummy);
                    free(previous_dir_dummy);
                }
            } else {
                if (getcwd(previous_dir, BUFFER_SIZE) == NULL) {
                    printf(RED_COLOR "Error: Could not store present working directory\n" RESET_COLOR);
                    return 1;
                }
                char* result_strstr;
                result_strstr = strstr(argv[i], "~");
                if (result_strstr != NULL) {
                    size_t home_len = strlen(global_init_home);
                    size_t path_len = strlen(argv[i]);
                    char* new_path = malloc(path_len + home_len + 1);
                    strcpy(new_path, global_init_home);
                    strcat(new_path, argv[i] + 1);
                    if (chdir(new_path) != 0) {
                        printf(RED_COLOR "Error: Could not hop to directory: " WHITE_COLOR "%s\n" RESET_COLOR, argv[i]);
                        return 1;
                    }
                } else {
                    if (chdir(argv[i]) != 0) {
                        printf(RED_COLOR "Error: Could not hop to directory: " WHITE_COLOR "%s\n" RESET_COLOR, argv[i]);
                        return 1;
                    }
                }
            }
            if (getcwd(present_dir, BUFFER_SIZE) == NULL) {
                printf(RED_COLOR "Error: Could not store present working directory\n" RESET_COLOR);
                return 1;
            }
            printf(CYAN_COLOR "%s\n" RESET_COLOR, present_dir);
            home_check(present_dir, global_init_home);
        }
        return 0;
    }
    return 0;
}