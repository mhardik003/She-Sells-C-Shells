#include "headers.h"
#include "utils.h"

void function_handler(char *function_name, char *args[], int num_args, int is_bg)
{

    if (strcmp(function_name, "clear") == 0)
    {
        if (num_args != 0)
        {
            printf("clear: too many arguments\n");
            return;
        }
        clear();
    }
    else if ((strcmp(function_name, "cd") == 0) || (strcmp(function_name, "warp") == 0))
    {
        warp(args, num_args);
    }
    else if ((strcmp(function_name, "ls") == 0) || (strcmp(function_name, "peek") == 0))
    {
        peek(args, num_args);
    }
    else if (strcmp(function_name, "history") == 0 || strcmp(function_name, "pastevents") == 0)
    {
        pastevents_driver(num_args, args);
    }
    else if (strcmp(function_name, "ps") == 0 || strcmp(function_name, "proclore") == 0)
    {
        proclore_driver(num_args, args);
    }
    else if (strcmp(function_name, "find") == 0 || strcmp(function_name, "seek") == 0)
    {
        seek(num_args, args);
    }
    else if (strcmp(function_name, "pwd") == 0)
    {
        printf("%s\n", CURR_PWD);
    }
    else if (strcmp(function_name, "exit") == 0 || strcmp(function_name, "quit") == 0)
    {
        exit_call_bool = 1;
    }
    else
    {
        // Add the command to args[0] and shift all the other elements to the right
        // Since execvp requires the first arguement to be the command itself
        for (int i = num_args; i > 0; i--)
        {
            args[i] = args[i - 1];
        }
        args[0] = function_name;

        // Add NULL to the end of args
        args[num_args + 1] = NULL;

        pid_t child_pid = fork();
        if (child_pid == 0)
        { // In child
            if (execvp(args[0], args) == -1)
            {
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        }
        else if (child_pid > 0)
        { // In parent
            int status;
            waitpid(child_pid, &status, WUNTRACED);
        }
        else if (child_pid < 0)
        {
            perror("Error forking");
        }
    }
}
