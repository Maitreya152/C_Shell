#include "headers.h"

void no_flags(char* dir_path, char* file_name) {
    char* temp_file_name = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_name, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(dir_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        if (open(dir_path, O_RDONLY, 0644)) {
            struct stat sb;
            if (stat(dir_path, &sb) == 0) {
                if (sb.st_mode & S_IXUSR) {
                    printf(GREEN_COLOR "%s\n" RESET_COLOR, file_name);
                } else {
                    printf(WHITE_COLOR "%s\n" RESET_COLOR, file_name);
                }
            } else {
                printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
            }
        } else {
            printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
        }
        return;
    }
    for (int i = 0; i < names_number; i++) {
        if (names_list[i]->d_name[0] != '.') {
            if (names_list[i]->d_type == 4) {
                printf(BLUE_COLOR "%s\n" RESET_COLOR, names_list[i]->d_name);
            } else if (names_list[i]->d_type == 8) {
                struct stat sb;
                sprintf(temp_file_name, "%s/%s", dir_path, names_list[i]->d_name);
                if (stat(temp_file_name, &sb) == 0 && sb.st_mode & S_IXUSR) {
                    printf(GREEN_COLOR "%s\n" RESET_COLOR, names_list[i]->d_name);
                } else {
                    printf(WHITE_COLOR "%s\n" RESET_COLOR, names_list[i]->d_name);
                }
            }
        }
    }
    free(temp_file_name);
}

void a_flags(char* dir_path, char* file_name) {
    char* temp_file_name = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_name, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(dir_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        if (open(dir_path, O_RDONLY, 0644)) {
            struct stat sb;
            if (stat(dir_path, &sb) == 0) {
                if (sb.st_mode & S_IXUSR) {
                    printf(GREEN_COLOR "%s\n" RESET_COLOR, file_name);
                } else {
                    printf(WHITE_COLOR "%s\n" RESET_COLOR, file_name);
                }
            } else {
                printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
            }
        } else {
            printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
        }
        return;
    }
    for (int i = 0; i < names_number; i++) {
        if (names_list[i]->d_type == 4) {
            printf(BLUE_COLOR "%s\n" RESET_COLOR, names_list[i]->d_name);
        } else if (names_list[i]->d_type == 8) {
            struct stat sb;
            sprintf(temp_file_name, "%s/%s", dir_path, names_list[i]->d_name);
            if (stat(temp_file_name, &sb) == 0 && sb.st_mode & S_IXUSR) {
                printf(GREEN_COLOR "%s\n" RESET_COLOR, names_list[i]->d_name);
            } else {
                printf(WHITE_COLOR "%s\n" RESET_COLOR, names_list[i]->d_name);
            }
        }
    }
    free(temp_file_name);
}

void l_flags(char* dir_path, char* file_name) {
    char* temp_file_name = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_name, '\0', BUFFER_SIZE);
    char* owner = malloc(sizeof(char) * BUFFER_SIZE);
    memset(owner, '\0', BUFFER_SIZE);
    char* group = malloc(sizeof(char) * BUFFER_SIZE);
    memset(group, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(dir_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        if (open(dir_path, O_RDONLY, 0644)) {
            struct stat file_info;
            if (lstat(dir_path, &file_info) == 0) {
                owner = getpwuid(file_info.st_uid)->pw_name;
                group = getgrgid(file_info.st_gid)->gr_name;
                long int file_size = file_info.st_size;
                printf((S_ISDIR(file_info.st_mode)) ? "d" : "-");
                printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
                printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
                printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
                printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
                printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
                printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
                printf((file_info.st_mode & S_IROTH) ? "r" : "-");
                printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
                printf((file_info.st_mode & S_IXOTH) ? "x " : "- ");
                char* date = malloc(sizeof(char) * BUFFER_SIZE);
                memset(date, '\0', BUFFER_SIZE);
                strftime(date, BUFFER_SIZE, "%h %d %H:%M", localtime(&(file_info.st_mtime)));
                if (file_info.st_mode & S_IXUSR) {
                    printf(WHITE_COLOR "%ld %s %s %ld \t %s " RESET_COLOR GREEN_COLOR "%s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, file_name);
                } else {
                    printf(WHITE_COLOR "%ld %s %s %ld \t %s %s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, file_name);
                }
            } else {
                printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
            }
        } else {
            printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
        }
        return;
    }
    int link_num;
    long int total_blocks = 0;
    for (int i = 0; i < names_number; i++) {
        if (names_list[i]->d_name[0] != '.') {
            sprintf(temp_file_name, "%s/%s", dir_path, names_list[i]->d_name);
            struct stat file_info;
            lstat(temp_file_name, &file_info);
            total_blocks = total_blocks + file_info.st_blocks;
        }
    }
    printf("total %ld\n", total_blocks/2);
    for (int i = 0; i < names_number; i++) {
        if (names_list[i]->d_name[0] != '.') {
            sprintf(temp_file_name, "%s/%s", dir_path, names_list[i]->d_name);
            struct stat file_info;
            lstat(temp_file_name, &file_info);
            owner = getpwuid(file_info.st_uid)->pw_name;
            group = getgrgid(file_info.st_gid)->gr_name;
            long int file_size = file_info.st_size;
            printf((S_ISDIR(file_info.st_mode)) ? "d" : "-");
            printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_info.st_mode & S_IROTH) ? "r" : "-");
            printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_info.st_mode & S_IXOTH) ? "x " : "- ");
            char* date = malloc(sizeof(char) * BUFFER_SIZE);
            memset(date, '\0', BUFFER_SIZE);
            strftime(date, BUFFER_SIZE, "%h %d %H:%M", localtime(&(file_info.st_mtime)));
            if (names_list[i]->d_type == 4) {
                printf(WHITE_COLOR "%ld %s %s %ld \t %s " RESET_COLOR BLUE_COLOR "%s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, names_list[i]->d_name);
            } else if (names_list[i]->d_type == 8) {
                struct stat sb;
                if (stat(temp_file_name, &sb) == 0 && sb.st_mode & S_IXUSR) {
                    printf(WHITE_COLOR "%ld %s %s %ld \t %s " RESET_COLOR GREEN_COLOR "%s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, names_list[i]->d_name);
                } else {
                    printf(WHITE_COLOR "%ld %s %s %ld \t %s %s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, names_list[i]->d_name);
                }
            }
            free(date);
        }
    }
    free(temp_file_name);
    free(owner);
    free(group);
}

void al_flags(char* dir_path, char* file_name) {
    char* temp_file_name = malloc(sizeof(char) * BUFFER_SIZE);
    memset(temp_file_name, '\0', BUFFER_SIZE);
    char* owner = malloc(sizeof(char) * BUFFER_SIZE);
    memset(owner, '\0', BUFFER_SIZE);
    char* group = malloc(sizeof(char) * BUFFER_SIZE);
    memset(group, '\0', BUFFER_SIZE);
    struct dirent **names_list;
    int names_number = scandir(dir_path, &names_list, NULL, alphasort);
    if (names_number == -1) {
        if (open(dir_path, O_RDONLY, 0644)) {
            struct stat file_info;
            if (lstat(dir_path, &file_info) == 0) {
                owner = getpwuid(file_info.st_uid)->pw_name;
                group = getgrgid(file_info.st_gid)->gr_name;
                long int file_size = file_info.st_size;
                printf((S_ISDIR(file_info.st_mode)) ? "d" : "-");
                printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
                printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
                printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
                printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
                printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
                printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
                printf((file_info.st_mode & S_IROTH) ? "r" : "-");
                printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
                printf((file_info.st_mode & S_IXOTH) ? "x " : "- ");
                char* date = malloc(sizeof(char) * BUFFER_SIZE);
                memset(date, '\0', BUFFER_SIZE);
                strftime(date, BUFFER_SIZE, "%h %d %H:%M", localtime(&(file_info.st_mtime)));
                if (file_info.st_mode & S_IXUSR) {
                    printf(WHITE_COLOR "%ld %s %s %ld \t %s " RESET_COLOR GREEN_COLOR "%s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, file_name);
                } else {
                    printf(WHITE_COLOR "%ld %s %s %ld \t %s %s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, file_name);
                }
            } else {
                printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
            }
        } else {
            printf(RED_COLOR "Could not Scan the Directory or file: " WHITE_COLOR "%s\n" RESET_COLOR, dir_path);
        }
        return;
    }
    int link_num;
    long int total_blocks = 0;
    for (int i = 0; i < names_number; i++) {
        sprintf(temp_file_name, "%s/%s", dir_path, names_list[i]->d_name);
        struct stat file_info;
        lstat(temp_file_name, &file_info);
        total_blocks = total_blocks + file_info.st_blocks;
    }
    printf("total %ld\n", total_blocks/2);
    for (int i = 0; i < names_number; i++) {
        sprintf(temp_file_name, "%s/%s", dir_path, names_list[i]->d_name);
        struct stat file_info;
        lstat(temp_file_name, &file_info);
        owner = getpwuid(file_info.st_uid)->pw_name;
        group = getgrgid(file_info.st_gid)->gr_name;
        long int file_size = file_info.st_size;
        printf((S_ISDIR(file_info.st_mode)) ? "d" : "-");
        printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_info.st_mode & S_IROTH) ? "r" : "-");
        printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_info.st_mode & S_IXOTH) ? "x " : "- ");
        char* date = malloc(sizeof(char) * BUFFER_SIZE);
        memset(date, '\0', BUFFER_SIZE);
        strftime(date, BUFFER_SIZE, "%h %d %H:%M", localtime(&(file_info.st_mtime)));
        if (names_list[i]->d_type == 4) {
            printf(WHITE_COLOR "%ld %s %s %ld \t %s " RESET_COLOR BLUE_COLOR "%s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, names_list[i]->d_name);
        } else if (names_list[i]->d_type == 8) {
            struct stat sb;
            if (stat(temp_file_name, &sb) == 0 && sb.st_mode & S_IXUSR) {
                printf(WHITE_COLOR "%ld %s %s %ld \t %s " RESET_COLOR GREEN_COLOR "%s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, names_list[i]->d_name);
            } else {
                printf(WHITE_COLOR "%ld %s %s %ld \t %s %s\n" RESET_COLOR, file_info.st_nlink, owner, group, file_size, date, names_list[i]->d_name);
            }
        }
        free(date);
    }
    free(temp_file_name);
    free(owner);
    free(group);
}

int reveal(int argc, char* argv[]) {

    // if (argc > 3) {
    //     printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
    //     return 1;
    // }

    int flag_a = 0;
    int flag_l = 0;

    if (argc == 1) {
        getcwd(present_dir, BUFFER_SIZE);
        no_flags(present_dir, NULL);
        return 0;
    } else if (argv[1][0] != '-') {
        if (strcmp(argv[1], "~") == 0 && argc == 2) {
            no_flags(global_init_home, NULL);
            return 0;
        } else if (argc == 2) {
            char* temp_path = malloc(sizeof(char) * BUFFER_SIZE);
            memset(temp_path, '\0', BUFFER_SIZE);
            getcwd(present_dir, BUFFER_SIZE);
            sprintf(temp_path, "%s/%s", present_dir, argv[1]);
            no_flags(temp_path, argv[1]);
            free(temp_path);
            return 0;
        }
    } else if (strcmp(argv[1], "-") == 0 && argc == 2) {
        if (strcmp(previous_dir, "") != 0) {
            no_flags(previous_dir, NULL);
            return 0;
        } else {
            printf(RED_COLOR "Error: Previous Directory not Defined\n" RESET_COLOR);
            return 1;
        }
    } else {
        if (argc < 2) {
            printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
            return 1;
        }
        int num_flags = 0;
        int flag_flags = 0; // will be 0 till flag processing is not completed.
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) == 1 && argv[i][0] == '-') {
                printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
                return 1;
            }
            if (argv[i][0] == '-' && flag_flags == 0) {
                num_flags++;
                for (int j = 1; j < strlen(argv[i]); j++) {
                    if (argv[i][j] == 'l') {
                        flag_l = 1;
                        continue;
                    } else if (argv[i][j] == 'a') {
                        flag_a = 1;
                        continue;
                    } else {
                        printf("%c\n", argv[i][j]);
                        printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
                        return 1;
                    }
                }
            } else if (argv[i][0] != '-' && flag_flags == 0) {
                flag_flags = 1;
            } else if (argv[i][0] == '-' && flag_flags == 1) {
                printf(RED_COLOR "Error: Invalid Arguments\n" RESET_COLOR);
                return 1;
            } else if (argv[i][0] != '-' && flag_flags == 1) {
                continue;
            }
        }
        if (flag_a == 1 && flag_l == 0) {
            char* temp_path = malloc(sizeof(char) * BUFFER_SIZE);
            memset(temp_path, '\0', BUFFER_SIZE);
            getcwd(present_dir, BUFFER_SIZE);
            if (argv[num_flags + 1] == NULL) {
                a_flags(present_dir, NULL);
                free(temp_path);
                return 0;
            }
            sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 1]);
            a_flags(temp_path, argv[num_flags + 1]);
            free(temp_path);
            return 0;
        }
        if (flag_a == 0 && flag_l == 1) {
            char* temp_path = malloc(sizeof(char) * BUFFER_SIZE);
            memset(temp_path, '\0', BUFFER_SIZE);
            getcwd(present_dir, BUFFER_SIZE);
            if (argv[num_flags + 1] == NULL) {
                l_flags(present_dir, NULL);
                free(temp_path);
                return 0;
            }
            sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 1]);
            l_flags(temp_path, argv[num_flags + 1]);
            free(temp_path);
            return 0;
        }
        if (flag_a == 1 && flag_l == 1) {
            char* temp_path = malloc(sizeof(char) * BUFFER_SIZE);
            memset(temp_path, '\0', BUFFER_SIZE);
            getcwd(present_dir, BUFFER_SIZE);
            if (argv[num_flags + 1] == NULL) {
                al_flags(present_dir, NULL);
                free(temp_path);
                return 0;
            }
            sprintf(temp_path, "%s/%s", present_dir, argv[num_flags + 1]);
            al_flags(temp_path, argv[num_flags + 1]);
            free(temp_path);
            return 0;
        }
    }
}