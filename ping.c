#include "headers.h"

int ping(int argc, char* argv[]) {
    if (argc != 3) {
        printf(RED_COLOR "INVALID USE OF PING\n" RESET_COLOR);
        return 1;
    }
    pid_t pid = atoi(argv[1]);
    if (pid < 0) {
        printf(RED_COLOR "ENTER A VALID PID\n" RESET_COLOR);
        return 1;
    }
    int signal = atoi(argv[2]) % 32;
    if (signal <= 0) {
        printf(RED_COLOR "ENTER A VALID SIGNAL\n" RESET_COLOR);
        return 1;
    }
    if (kill(pid,signal) < 0) {
        printf(RED_COLOR "ERROR SENDIGN SIGNAL\n" RESET_COLOR);
        return 1;
    }
    printf("Sent signal %d to process with pid %d\n", signal, pid);
    return 0;
}
