#include "headers.h"
int time_interval;

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        perror("tcgetattr");
        exit(1);
    }
}

void print_last_pid() {    
    FILE *file = fopen("/proc/sys/kernel/ns_last_pid", "r");
    if (file) {
        int last_pid;
        fscanf(file, "%d", &last_pid);
        printf("%d\n", last_pid);
        fclose(file);
    } 
    alarm(time_interval);
}

int neonate (int argc, char* argv[]) {
    if (argc != 3) {
        printf(RED_COLOR "INVALID FORMAT FOR NEONATE\n" RESET_COLOR);
        return 1;
    } else if (strcmp(argv[1], "-n") != 0) {
        printf(RED_COLOR "INVALID FORMAT FOR NEONATE\n" RESET_COLOR);
        return 1;
    }
    time_interval = atoi(argv[2]);
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcgetattr");
        exit(1);
    }   
    while (1) { 
        alarm(time_interval);
        signal(SIGALRM, print_last_pid);       
        char c = getchar();
        if(c == 'x') {
            alarm(0);
            break;
        }        
    }
    disableRawMode();
}
