#ifndef __ACTIVITIES_H_
#define __ACTIVITIES_H_

int activities(int argc, char* argv[]);
int* get_all_pids(const char *filename, int *pid_count);
void get_process_info(int pid, char *command, char *state);

#endif
