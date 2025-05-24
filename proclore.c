#include "headers.h"

int process_info(pid_t pid) {
    int fg_flag = 0;
    char path[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    FILE *fp;
    char str_pid[BUFFER_SIZE];
    sprintf(str_pid, "%d", pid);
    char *state;
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (fp) {
        if (fgets(line, sizeof(line), fp)) {
            state = strchr(line, ')') + 2;
        }
        fclose(fp);
    } else {
        printf(RED_COLOR "INVALID USE OF PROCLORE\n" RESET_COLOR);
        return -1;
    }
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (fp) {
        if (fgets(line, sizeof(line), fp)) {
            char* token = strtok(line, " ");
            for (int i = 0; i < 4 && token != NULL; i++) {
                token = strtok(NULL, " ");
            }
            if (token != NULL) {
                if (strcmp(token, str_pid) == 0) {
                    fg_flag = 1;
                }
                printf("process status : %c", *state);
                if (fg_flag == 1) {
                    printf("+\n");
                } else {
                    printf("\n");
                }
                printf("Process Group : %s\n", token);
            }
        }
        fclose(fp);
    }

    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    fp = fopen(path, "r");
    if (fp) {
        if (fscanf(fp, "%s", line) == 1) {
            printf("Virtual memory : %s\n", line);
        }
        fclose(fp);
    }

    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, line, sizeof(line) - 1);
    if (len != -1) {
        line[len] = '\0';
        printf("executable path : %s\n", line);
    }

}

int proclore(int argc, char* argv[]) {
    if (argc > 2) {
        printf(RED_COLOR "Proclore only takes one argument.\n" RESET_COLOR);
        return -1;
    }
    if (argc == 1) {
        int status = process_info(shell_pid);
    } else {
        pid_t pid = atoi(argv[1]);
        if (pid > 0) {
            int status = process_info(pid);
        } else {
            printf(RED_COLOR "Process ID Format Invalid\n" RESET_COLOR);
            return -1;
        }
    }
}