#ifndef __LOG_H_
#define __LOG_H_

void add_to_log(char* command);
void log_display();
void init_last_command();
void log_purge();
void log_execute(char* index);

#endif