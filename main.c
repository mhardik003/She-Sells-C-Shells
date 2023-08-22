#include "headers.h"

void init_shell()
{
    clear();
    strcpy(CURR_PWD, "~");

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
    init_shell();
    find_os(&isLinuxBool, &isWindowsBool);
    get_user_and_sys_name(USER_NAME, SYSTEM_NAME, isLinuxBool, isWindowsBool);
    get_pwd(INIT_PWD);
    while (1)
    {

        // Print appropriate prompt with username, systemname and directory before accepting input
        // printf("Current working directory: %s\n", INIT_PWD);
        print_prompt(INIT_PWD);

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
    }
    exit_shell();

    return 0;
}
