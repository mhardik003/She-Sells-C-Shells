#include "headers.h"

/*
 File to initialize all the global variables
*/

char USER_NAME[MAX_LEN];
char SYSTEM_NAME[MAX_LEN];
char CURR_PWD[LEN_PWD];
char INIT_PWD[LEN_PWD];
char PREV_PWD[LEN_PWD];
int isWindowsBool = 0;
int isLinuxBool = 0;

int exit_call_bool = 0; // set to 1 if the input is exit (along with some other input)

bg_process bg_procs[1024];
int bg_count = 0;