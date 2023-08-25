#ifndef _GLOBAL_VARS_H_
#define _GLOBAL_VARS_H_

// #define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

#include <unistd.h>


extern pid_t backgroundProcesses[100];
extern char *backgroundProcessNames[100];
extern int bg_count;

extern char USER_NAME[];
extern char SYSTEM_NAME[];
extern char HOME_DIR[];
extern char PREV_PWD[];
extern char CURR_PWD[];
extern char  HISTORY_FILE [];


extern char LAST_COMMAND[];


extern int isWindowsBool;
extern int exit_call_bool;
extern int isLinuxBool;
extern int prevElapsedTime;

#endif