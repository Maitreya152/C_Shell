#ifndef __EXECUTE_H_
#define __EXECUTE_H_

int execute(int argc, char* argv[], char seperator);
char* find_command_by_pid(const char *filename, int p_id);

#endif