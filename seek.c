#include "headers.h"
int found_flag = 0;
char out_e_command[BUFFER_SIZE];

void seek_no_flags(char* search, char* target_path, char* call_dir) {
    char* temp_file_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_path, '\0', BUFFER_SIZE);
    char* temp_dir_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_dir_path, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(target_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        return;
    }
    char* result_strstr;
    for (int i = 0; i < names_number; i++) {
        result_strstr = strstr(names_list[i]->d_name, search);
        if (result_strstr != NULL) {
            found_flag = 1;
            sprintf(temp_file_path, "%s/%s", target_path, names_list[i]->d_name);
            char* relative_path = calc_relative_path(call_dir, temp_file_path);
            printf("%s\n", relative_path);
            free(relative_path);
        }
        if (names_list[i]->d_type == 4) {
            if (names_list[i]->d_name[0] == '.' && strlen(names_list[i]->d_name) == 1) {
                continue;
            }
            if (names_list[i]->d_name[0] == '.' && names_list[i]->d_name[1] == '.' && strlen(names_list[i]->d_name) == 2) {
                continue;
            }
            sprintf(temp_dir_path, "%s/%s", target_path, names_list[i]->d_name);
            seek_no_flags(search, temp_dir_path, call_dir);
        }
    }
    free(temp_file_path);
    free(temp_dir_path);
    return;
}

void seek_d_flags(char* search, char* target_path, char* call_dir) {
    char* temp_file_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_path, '\0', BUFFER_SIZE);
    char* temp_dir_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_dir_path, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(target_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        return;
    }
    char* result_strstr;
    for (int i = 0; i < names_number; i++) {
        result_strstr = strstr(names_list[i]->d_name, search);
        if (result_strstr != NULL) {
            if (names_list[i]->d_type == 4) {
                found_flag = 1;
                sprintf(temp_file_path, "%s/%s", target_path, names_list[i]->d_name);
                char* relative_path = calc_relative_path(call_dir, temp_file_path);
                printf("%s\n", relative_path);
                free(relative_path);
            }
        }
        if (names_list[i]->d_type == 4) {
            if (names_list[i]->d_name[0] == '.' && strlen(names_list[i]->d_name) == 1) {
                continue;
            }
            if (names_list[i]->d_name[0] == '.' && names_list[i]->d_name[1] == '.' && strlen(names_list[i]->d_name) == 2) {
                continue;
            }
            sprintf(temp_dir_path, "%s/%s", target_path, names_list[i]->d_name);
            seek_d_flags(search, temp_dir_path, call_dir);
        }
    }
    free(temp_file_path);
    free(temp_dir_path);
    return;
}

void seek_f_flags(char* search, char* target_path, char* call_dir) {
    char* temp_file_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_path, '\0', BUFFER_SIZE);
    char* temp_dir_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_dir_path, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(target_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        return;
    }
    char* result_strstr;
    for (int i = 0; i < names_number; i++) {
        result_strstr = strstr(names_list[i]->d_name, search);
        if (result_strstr != NULL) {
            if (names_list[i]->d_type != 4) {
                found_flag = 1;
                sprintf(temp_file_path, "%s/%s", target_path, names_list[i]->d_name);
                char* relative_path = calc_relative_path(call_dir, temp_file_path);
                printf("%s\n", relative_path);
                free(relative_path);
            }
        }
        if (names_list[i]->d_type == 4) {
            if (names_list[i]->d_name[0] == '.' && strlen(names_list[i]->d_name) == 1) {
                continue;
            }
            if (names_list[i]->d_name[0] == '.' && names_list[i]->d_name[1] == '.' && strlen(names_list[i]->d_name) == 2) {
                continue;
            }
            sprintf(temp_dir_path, "%s/%s", target_path, names_list[i]->d_name);
            seek_f_flags(search, temp_dir_path, call_dir);
        }
    }
    free(temp_file_path);
    free(temp_dir_path);
    return;
}

void seek_de_flags(char* search, char* target_path, char* call_dir) {
    char* temp_file_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_path, '\0', BUFFER_SIZE);
    char* temp_dir_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_dir_path, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(target_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        return;
    }
    char* result_strstr;
    for (int i = 0; i < names_number; i++) {
        result_strstr = strstr(names_list[i]->d_name, search);
        if (result_strstr != NULL) {
            if (names_list[i]->d_type == 4) {
                found_flag++;
                sprintf(temp_file_path, "%s/%s", target_path, names_list[i]->d_name);
                strcpy(out_e_command, temp_file_path);
                char* relative_path = calc_relative_path(call_dir, temp_file_path);
                printf("%s\n", relative_path);
                free(relative_path);
            }
        }
        if (names_list[i]->d_type == 4) {
            if (names_list[i]->d_name[0] == '.' && strlen(names_list[i]->d_name) == 1) {
                continue;
            }
            if (names_list[i]->d_name[0] == '.' && names_list[i]->d_name[1] == '.' && strlen(names_list[i]->d_name) == 2) {
                continue;
            }
            sprintf(temp_dir_path, "%s/%s", target_path, names_list[i]->d_name);
            seek_de_flags(search, temp_dir_path, call_dir);
        }
    }
    free(temp_file_path);
    free(temp_dir_path);
    return;
}

void seek_fe_flags(char* search, char* target_path, char* call_dir) {
    char* temp_file_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_path, '\0', BUFFER_SIZE);
    char* temp_dir_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_dir_path, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(target_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        return;
    }
    char* result_strstr;
    for (int i = 0; i < names_number; i++) {
        result_strstr = strstr(names_list[i]->d_name, search);
        if (result_strstr != NULL) {
            if (names_list[i]->d_type != 4) {
                found_flag++;
                sprintf(temp_file_path, "%s/%s", target_path, names_list[i]->d_name);
                strcpy(out_e_command, temp_file_path);
                // printf("%s\n", out_e_command);
                char* relative_path = calc_relative_path(call_dir, temp_file_path);
                printf("%s\n", relative_path);
                free(relative_path);
            }
        }
        if (names_list[i]->d_type == 4) {
            if (names_list[i]->d_name[0] == '.' && strlen(names_list[i]->d_name) == 1) {
                continue;
            }
            if (names_list[i]->d_name[0] == '.' && names_list[i]->d_name[1] == '.' && strlen(names_list[i]->d_name) == 2) {
                continue;
            }
            sprintf(temp_dir_path, "%s/%s", target_path, names_list[i]->d_name);
            seek_fe_flags(search, temp_dir_path, call_dir);
        }
    }
    free(temp_file_path);
    free(temp_dir_path);
    return;
}

void seek_e_flags(char* search, char* target_path, char* call_dir) {
    char* temp_file_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_path, '\0', BUFFER_SIZE);
    char* temp_dir_path = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_dir_path, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(target_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        return;
    }
    char* result_strstr;
    for (int i = 0; i < names_number; i++) {
        result_strstr = strstr(names_list[i]->d_name, search);
        if (result_strstr != NULL) {
            found_flag++;
            sprintf(temp_file_path, "%s/%s", target_path, names_list[i]->d_name);
            strcpy(out_e_command, temp_file_path);
            char* relative_path = calc_relative_path(call_dir, temp_file_path);
            printf("%s\n", relative_path);
            free(relative_path);
        }
        if (names_list[i]->d_type == 4) {
            if (names_list[i]->d_name[0] == '.' && strlen(names_list[i]->d_name) == 1) {
                continue;
            }
            if (names_list[i]->d_name[0] == '.' && names_list[i]->d_name[1] == '.' && strlen(names_list[i]->d_name) == 2) {
                continue;
            }
            sprintf(temp_dir_path, "%s/%s", target_path, names_list[i]->d_name);
            seek_e_flags(search, temp_dir_path, call_dir);
        }
    }
    free(temp_file_path);
    free(temp_dir_path);
    return;
}

int seek(int argc, char* argv[]) {
    int flag_d = 0;
    int flag_f = 0;
    int flag_e = 0;
    if (argc == 1) {
        printf(RED_COLOR "seek has minimum 1 argument\n" RESET_COLOR);
        return -1;
    } else if (argc == 2) {
        getcwd(present_dir, BUFFER_SIZE);
        if (strcmp(argv[1], "-") != 0) {
            if (strcmp(argv[1], "~") == 0) {
                found_flag = 0;
                seek_no_flags(global_init_home, present_dir, present_dir);
                if (found_flag != 1) {
                    printf(BLUE_COLOR "No matching file or directory found\n" RESET_COLOR);
                }
            } else {
                found_flag = 0;
                seek_no_flags(argv[1], present_dir, present_dir);
                if (found_flag != 1) {
                    printf(BLUE_COLOR "No matching file or directory found\n" RESET_COLOR);
                }
            }
            return 0;
        } else {
            if (strcmp(previous_dir, "") != 0) {
                found_flag = 0;
                // printf("%s\n", previous_dir);
                const char* last_slash = strrchr(previous_dir, '/');
                char* prev_dir_name = strdup(last_slash + 1);
                seek_no_flags(prev_dir_name, present_dir, present_dir);
                if (found_flag != 1) {
                    printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                }
            } else {
                printf(RED_COLOR "Previous Directory not Defined" RESET_COLOR);
            }
        }
    } else if (argc > 2) {
        int num_flags = 0;
        int flag_flags = 0; // will be 0 till flag processing is not completed.
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) == 1 && argv[i][0] == '-') {
                printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
                return -1;
            }
            if (argv[i][0] == '-' && flag_flags == 0) {
                num_flags++;
                for (int j = 1; j < strlen(argv[i]); j++) {
                    if (argv[i][j] == 'd') {
                        flag_d = 1;
                        continue;
                    } else if (argv[i][j] == 'f') {
                        flag_f = 1;
                        continue;
                    } else if (argv[i][j] == 'e') {
                        flag_e = 1;
                    } else {
                        printf("%c\n", argv[i][j]);
                        printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
                        return -1;
                    }
                }
            } else if (argv[i][0] != '-' && flag_flags == 0) {
                flag_flags = 1;
            } else if (argv[i][0] == '-' && flag_flags == 1) {
                printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
                return -1;
            } else if (argv[i][0] != '-' && flag_flags == 1) {
                continue;
            }
        }
        // printf("NUMFLAGS: %d ARGC: %d\n", num_flags, argc);
        if (flag_f == 1 && flag_d == 1) {
            printf(RED_COLOR "Flag f and d Cannot be set together\n" RESET_COLOR);
            return -1;
        }
        getcwd(present_dir, BUFFER_SIZE);
        if (argc - num_flags == 2) {
            if (flag_e == 1) {
                if (flag_d == 1) {
                    found_flag = 0;
                    seek_de_flags(argv[num_flags + 1], present_dir, present_dir);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "Did not find exactly 1 files while e flag was enabled\n" RESET_COLOR);
                    } else {
                        getcwd(previous_dir, BUFFER_SIZE);
                        int e_result = chdir(out_e_command);
                        if (e_result < 0) {
                            printf(RED_COLOR "Cannot change directroy to: " RESET_COLOR "%s\n", out_e_command);
                        }
                    }
                    return 0;
                } else if (flag_f == 1) {
                    found_flag = 0;
                    seek_fe_flags(argv[num_flags + 1], present_dir, present_dir);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "Did not find exactly 1 files while e flag was enabled\n" RESET_COLOR);
                    } else {
                        int dummy_argc = 2;
                        char **dummy_arguments = malloc(2 * sizeof(char *));
                        for (int i = 0; i < 2; i++) {
                            dummy_arguments[i] = malloc(BUFFER_SIZE * sizeof(char));
                        }
                        strcpy(dummy_arguments[0], "cat");
                        strcpy(dummy_arguments[1], out_e_command);
                        execute(dummy_argc, dummy_arguments, ';');
                        for (int i = 0; i < 2; i++) {
                            free(dummy_arguments[i]);
                        }
                        free(dummy_arguments);
                    }
                    return 0;
                } else if (flag_d == 0 && flag_f == 0) {
                    found_flag = 0;
                    seek_e_flags(argv[num_flags + 1], present_dir, present_dir);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "Did not find exactly 1 files while e flag was enabled\n" RESET_COLOR);
                    } else {
                        struct stat path_stat;
                        stat(out_e_command, &path_stat);
                        if (S_ISDIR(path_stat.st_mode)) {
                            getcwd(previous_dir, BUFFER_SIZE);
                            int e_result = chdir(out_e_command);
                            if (e_result < 0) {
                                printf(RED_COLOR "Cannot change directroy to: " RESET_COLOR "%s\n", out_e_command);
                            }
                            return 0;
                        } else {
                            int dummy_argc = 2;
                            char **dummy_arguments = malloc(2 * sizeof(char *));
                            for (int i = 0; i < 2; i++) {
                                dummy_arguments[i] = malloc(BUFFER_SIZE * sizeof(char));
                            }
                            strcpy(dummy_arguments[0], "cat");
                            strcpy(dummy_arguments[1], out_e_command);
                            execute(dummy_argc, dummy_arguments, ';');
                            for (int i = 0; i < 2; i++) {
                                free(dummy_arguments[i]);
                            }
                            free(dummy_arguments);
                            return 0;
                        }
                    }
                    return 0;
                }
            } else {
                if (flag_d == 0 && flag_f == 0) {
                    found_flag = 0;
                    seek_no_flags(argv[num_flags + 1], present_dir, present_dir);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                    }
                    return 0;
                    // free(temp_path);
                } else if (flag_d == 1 && flag_f == 0) {
                    found_flag = 0;
                    seek_d_flags(argv[num_flags + 1], present_dir, present_dir);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                    }
                    return 0;
                } else if (flag_d == 0 && flag_f == 1) {
                    found_flag = 0;
                    seek_f_flags(argv[num_flags + 1], present_dir, present_dir);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                    }
                }
            }
        } else if (argc - num_flags == 3) {
            // printf("TWO COMMANDS TO PROCESS\n");
            if (flag_e == 1) {
                char* temp_path = malloc(BUFFER_SIZE * sizeof(char));
                memset(temp_path, '\0', BUFFER_SIZE);
                sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 2]);
                if (flag_d == 1) {
                    found_flag = 0;
                    seek_de_flags(argv[num_flags + 1], temp_path, temp_path);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "Did not find exactly 1 files while e flag was enabled\n" RESET_COLOR);
                    } else {
                        getcwd(previous_dir, BUFFER_SIZE);
                        int e_result = chdir(out_e_command);
                        if (e_result < 0) {
                            printf(RED_COLOR "Cannot change directroy to: " RESET_COLOR "%s\n", out_e_command);
                        }
                    }
                    return 0;
                } else if (flag_f == 1) {
                    found_flag = 0;
                    seek_fe_flags(argv[num_flags + 1], temp_path, temp_path);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "Did not find exactly 1 files while e flag was enabled\n" RESET_COLOR);
                    } else {
                        int dummy_argc = 2;
                        char **dummy_arguments = malloc(2 * sizeof(char *));
                        for (int i = 0; i < 2; i++) {
                            dummy_arguments[i] = malloc(BUFFER_SIZE * sizeof(char));
                        }
                        strcpy(dummy_arguments[0], "cat");
                        strcpy(dummy_arguments[1], out_e_command);
                        execute(dummy_argc, dummy_arguments, ';');
                        for (int i = 0; i < 2; i++) {
                            free(dummy_arguments[i]);
                        }
                        free(dummy_arguments);
                    }
                    return 0;
                } else if (flag_d == 0 && flag_f == 0) {
                    found_flag = 0;
                    seek_e_flags(argv[num_flags + 1], temp_path, temp_path);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "Did not find exactly 1 files while e flag was enabled\n" RESET_COLOR);
                    } else {
                        struct stat path_stat;
                        stat(out_e_command, &path_stat);
                        if (S_ISDIR(path_stat.st_mode)) {
                            getcwd(previous_dir, BUFFER_SIZE);
                            int e_result = chdir(out_e_command);
                            if (e_result < 0) {
                                printf(RED_COLOR "Cannot change directroy to: " RESET_COLOR "%s\n", out_e_command);
                            }
                            return 0;
                        } else {
                            int dummy_argc = 2;
                            char **dummy_arguments = malloc(2 * sizeof(char *));
                            for (int i = 0; i < 2; i++) {
                                dummy_arguments[i] = malloc(BUFFER_SIZE * sizeof(char));
                            }
                            strcpy(dummy_arguments[0], "cat");
                            strcpy(dummy_arguments[1], out_e_command);
                            execute(dummy_argc, dummy_arguments, ';');
                            for (int i = 0; i < 2; i++) {
                                free(dummy_arguments[i]);
                            }
                            free(dummy_arguments);
                            return 0;
                        }
                    }
                    return 0;
                }
            } else {
                if (flag_d == 0 && flag_f == 0) {
                    char* temp_path = malloc(BUFFER_SIZE * sizeof(char));
                    memset(temp_path, '\0', BUFFER_SIZE);
                    sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 2]);
                    found_flag = 0;
                    seek_no_flags(argv[num_flags + 1], temp_path, temp_path);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                    }
                    return 0;
                    // free(temp_path);
                } else if (flag_d == 1 && flag_f == 0) {
                    char* temp_path = malloc(BUFFER_SIZE * sizeof(char));
                    memset(temp_path, '\0', BUFFER_SIZE);
                    sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 2]);
                    found_flag = 0;
                    seek_d_flags(argv[num_flags + 1], temp_path, temp_path);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                    }
                    return 0;
                } else if (flag_d == 0 && flag_f == 1) {
                    char* temp_path = malloc(BUFFER_SIZE * sizeof(char));
                    memset(temp_path, '\0', BUFFER_SIZE);
                    sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 2]);
                    found_flag = 0;
                    seek_f_flags(argv[num_flags + 1], temp_path, temp_path);
                    if (found_flag != 1) {
                        printf(BLUE_COLOR "No match found!\n" RESET_COLOR);
                    }
                }
            }
        } else {
            printf(RED_COLOR "INVALID ARGUMENTS\n" RESET_COLOR);
            return -1;
        }
    }
}