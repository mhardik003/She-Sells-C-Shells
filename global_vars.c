#include "headers.h"

/*
 File to initialize all the global variables
*/

char LAST_COMMAND[MAX_LEN];

char USER_NAME[MAX_LEN];
char SYSTEM_NAME[MAX_LEN];
char CURR_PWD[LEN_PWD];
char HOME_DIR[LEN_PWD];
char PREV_PWD[LEN_PWD];
char HISTORY_FILE[LEN_PWD];

int isWindowsBool = 0;
int isLinuxBool = 0;
int prevElapsedTime = 0;
int exit_call_bool = 0; // set to 1 if the input is exit (along with some other input)
char *backgroundProcessNames[100];
pid_t backgroundProcesses[100];


// malloc this array

int bg_count = 0;

void initialize_bgNames()
{
    for (int i = 0; i < 100; i++)
    {
        backgroundProcessNames[i] = (char *)malloc(100 * sizeof(char));
    }
}