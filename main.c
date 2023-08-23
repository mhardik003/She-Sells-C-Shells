#include "headers.h"

void init_shell()
{
    clear();
    get_pwd(INIT_PWD);
    get_pwd(CURR_PWD);
    get_pwd(PREV_PWD);

    find_os(&isLinuxBool, &isWindowsBool);
    get_user_and_sys_name(USER_NAME, SYSTEM_NAME, isLinuxBool, isWindowsBool);

    // check if history file exists
    checkHistoryFile();
}

void exit_shell()
{
    clear();
}

int main()
{
    // Keep accepting commands
    
    // signal(SIGINT, handler);
    
    init_shell();
    while (1)
    {

        // Print appropriate prompt with username, systemname and directory before accepting input
        // printf("Current working directory: %s\n", INIT_PWD);
        print_prompt();

        char input[4096];
        fgets(input, 4096, stdin);
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }
        if (strcmp(input, "exit") == 0)
        {
            printf("By!");
            break;
        }

        input_handler(input);
        check_background_processes();
    }
    exit_shell();

    return 0;
}
