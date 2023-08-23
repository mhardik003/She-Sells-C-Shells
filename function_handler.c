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
        }
        else if (num_args == 2)
        {
            if (atoi(args[1]) != 0)
            {
                execute_pastevent(atoi(args[1]));
            }
        }
    }
    else if (strcmp(function_name, "ps") == 0 || strcmp(function_name, "proclore") == 0)
    {
        proclore(num_args, args);
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
        for (int i = num_args; i > 0; i--)
        {
            args[i] = args[i - 1];
        }
        args[0] = function_name;


        for(int i=0; i<num_args+1; i++)
        {
            printf("%s\n", args[i]);
        }
        
        setpgid(0,0);

        // args contains both the command and the arguements
        if (execvp(args[0], args) == -1)
        {
            printf("ERROR : '%s' is not a valid command\n", function_name);
        }
    }
}
