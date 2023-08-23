#include "headers.h"

void init_shell()
{
    clear();
    get_pwd(INIT_PWD);
    get_pwd(CURR_PWD);
    get_pwd(PREV_PWD);
    exit_call_bool=0;
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

    // signal(SIGINT, handler);
    // signal(SIGTSTP, handler);
    // signal(SIGCHLD, bg_end_handler);

    init_shell();
    while (1)
    {

        // Print appropriate prompt with username, systemname and directory before accepting input
        // printf("Current working directory: %s\n", INIT_PWD);

        // prints the prompt
        print_prompt();

        // take input
        char input[4096];
        fgets(input, 4096, stdin);
        size_t len = strlen(input);

        // remove the trailing newline character
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }

        // handle the input
        input_handler(input);

        // exit_call_bool is set to 1 in the function handler if the input is exit (along with some other input)
        if (exit_call_bool)
        {
            printf("By!");
            break;
        }

        // check for background processes which are yet to complete
        check_background_processes();
    }
    exit_shell();

    return 0;
}
