#include "headers.h"



void init_shell()
{
    /*
        Function to initialize the shell
    */
    get_pwd(HOME_DIR);
    get_pwd(CURR_PWD);
    strcpy(HISTORY_FILE, HOME_DIR);
    strcat(HISTORY_FILE, "/.history.txt");
    strcpy(PREV_PWD, "NULL");
    exit_call_bool = 0;
    find_os(&isLinuxBool, &isWindowsBool);
    get_user_and_sys_name(USER_NAME, SYSTEM_NAME, isLinuxBool, isWindowsBool);
    initialize_bgNames();
    // check if history file exists
    checkHistoryFile();
    clear();
}

void exit_shell()
{
    /*
        Function to exit the shell
    */

    clear();
}

void handler(int sig)
{
    /*
        Function to handle the SIGINT (Ctrl+C) signal
    */
    if (sig == SIGINT)
    {
        printf("\n");
        print_prompt();
        fflush(stdout);
    }
}

void sigstp_handler(int sig)
{
    /*
        Function to handle the SIGTSTP (Ctrl+Z) signal
    */
    if (sig == SIGTSTP)
    {
        printf("\n");
        print_prompt();
        fflush(stdout);
    }
}

void sigchld_handler(int sig)
{
    /*
        Function to handle the SIGCHLD signal
    */
    if (sig == SIGCHLD)
    {
        // printf("Child process ended\n");
        displayOutputAndCleanup();
    }
}

int main()
{
    /*
        Main function
    */

    signal(SIGINT, handler);
    signal(SIGTSTP, sigstp_handler);
    signal(SIGCHLD, sigchld_handler);

    init_shell();
    while (1)
    {
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

        /*
        check for background processes which are yet to complete
        if they are complete, then remove them from the bg_procs array
        and print the message that the process has ended
        */
        check_bg_processes();

        // handle the input
        // printf("Input is '%s'\n", input);
        input_handler(input);

        // exit_call_bool is set to 1 in the function handler if the input is exit (along with some other input)
        if (exit_call_bool)
        {
            printf("By!");
            break;
        }
    }
    exit_shell();

    return 0;
}
