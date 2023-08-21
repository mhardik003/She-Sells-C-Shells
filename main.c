
#include "utils.h"
#include "headers.h"

char USER_NAME[MAX_LEN];
char SYSTEM_NAME[MAX_LEN];
char INIT_PWD[MAX_LEN];
int isWindowsBool;
int isLinuxBool;

void init_shell()
{
    clear();
}

int main()
{
    // Keep accepting commands
    find_os(&isLinuxBool, &isWindowsBool);
    get_user_and_sys_name(USER_NAME, SYSTEM_NAME, INIT_PWD, isLinuxBool, isWindowsBool);
    init_shell();
    while (1)
    {

        // Print appropriate prompt with username, systemname and directory before accepting input
        print_prompt();
        printf("Current working directory: %s\n", INIT_PWD);
        
        char input[4096];
        fgets(input, 4096, stdin);
        
        if (strcmp(input, "exit\n") == 0)
        {
            printf("breaking the loop");
            break;
        }

        input_handler(input);
        
        
    }
    
    return 0;
}
