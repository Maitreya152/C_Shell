#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>// For socket(), connect()
#include <netinet/in.h> // For sockaddr_in
#include <curl/curl.h>
#include <termios.h>

#define BUFFER_SIZE 4096
typedef struct Alias {
    char name[BUFFER_SIZE];
    char value[BUFFER_SIZE];
    struct Alias *next;
} Alias;
extern char global_init_home[BUFFER_SIZE];
extern char sys_name[BUFFER_SIZE];
extern char present_dir[BUFFER_SIZE];
extern char user_name[BUFFER_SIZE];
extern char previous_dir[BUFFER_SIZE];
extern char previous_command[BUFFER_SIZE];
extern pid_t shell_pid;
extern pid_t global_fg_pid;
extern Alias *alias_list;

#include "directory.h"
#include "display.h"
#include "colors.h"
#include "hop.h"
#include "execute.h"
#include "reveal.h"
#include "log.h"
#include "main.h"
#include "proclore.h"
#include "seek.h"
#include "iMan.h"
#include "activities.h"
#include "neonate.h"
#include "fgbg.h"
#include "ping.h"

#endif
