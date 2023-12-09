#include "headers.h"
#include "utils.h"

void function_handler(char *function_name, char *args[], int num_args, int is_bg)
{
    /*
        Function to handle the function calls
    */

    if ((strcmp(function_name, "warp") == 0))
    {
        warp(args, num_args);
    }
    else if ((strcmp(function_name, "peek") == 0))
    {
        peek(args, num_args);
    }
    else if (strcmp(function_name, "pastevents") == 0)
    {
        pastevents_driver(num_args, args);
    }
    else if (strcmp(function_name, "proclore") == 0)
    {
        proclore_driver(num_args, args);
    }
    else if (strcmp(function_name, "seek") == 0)
    {
        // printf("hi\n");
        seek(num_args, args);
    }
    else if (strcmp(function_name, "activities") == 0)
    {
        display_activities();
    }
    else if (strcmp(function_name, "ping") == 0)
    {
        ping_func(num_args, args);
    }
    else if (strcmp(function_name, "fg") == 0)
    {
        fg_command(num_args, args);
    }
    else if (strcmp(function_name, "bg") == 0)
    {
        bg_command(num_args, args);
    }
    else if (strcmp(function_name, "neonate") == 0)
    {
        neonate(num_args, args);
    }
    else if (strcmp(function_name, "iMan") == 0)
    {
        fetch_iMan_page(num_args, args);
    }

    else if (strcmp(function_name, "exit") == 0 || strcmp(function_name, "quit") == 0)
    {
        cleanup_and_exit();
        exit_call_bool = 1;
    }

    else
    {
        // Add the command to args[0] and shift all the other elements to the right
        // Since execvp requires the first arguement to be the command itself
        strcpy(LAST_COMMAND, function_name);
        syscalls(num_args, args, function_name, is_bg);
    }
}
