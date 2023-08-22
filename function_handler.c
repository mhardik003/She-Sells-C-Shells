#include "headers.h"
#include "utils.h"

void function_handler(char *function_name, char *args[], int num_args)
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
        if (num_args == 0)
        {

            pastevents();
        }
        else if (num_args == 1)
        {
            if (strcmp(args[0], "purge") == 0)
            {
                pastevents_purge();
            }
            else if (atoi(args[0]) != 0)
            {
                execute_pastevent(atoi(args[0]));
            }
        }
    }
    else if (strcmp(function_name, "pinfo") == 0 || strcmp(function_name, "proclore") == 0)
    {
        if (num_args == 0)
        {
            proclore_shell();
        }

        if (num_args == 1)
        {
            int pid = atoi(args[0]);
            if (pid == 0)
            {
                printf("Enter a valid PID\n");
                return;
            }
            proclore(pid);
        }
    }
    else if (strcmp(function_name, "find") == 0 || strcmp(function_name, "seek") == 0)
    {
        printf("Find function\n");
    }
    else if (strcmp(function_name, "sleep") == 0)
    {
        if (num_args == 0)
        {
            printf("Enter a valid time\n");
            return;
        }
        int time = atoi(args[0]);
        if (time == 0)
        {
            printf("Enter a valid time\n");
            return;
        }
        sleep(time);
    }

    else
    {
        printf("ERROR : '%s' is not a valid command\n", function_name);
    }
}
