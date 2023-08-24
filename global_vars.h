#ifndef _GLOBAL_VARS_H_
#define _GLOBAL_VARS_H_

// #define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

#include <unistd.h>

typedef struct
{
    pid_t pid;
    char cmd[50];
} bg_process;

extern bg_process bg_procs[1024];
extern int bg_count;

extern char USER_NAME[];
extern char SYSTEM_NAME[];
extern char INIT_PWD[];
extern char PREV_PWD[];
extern char CURR_PWD[];

extern int isWindowsBool;
extern int exit_call_bool;
extern int isLinuxBool;

#endif