#include "utils.h"
#include "headers.h"

void remove_first_element_from_array(char *args[], int num_args)
{

    for (int i = 0; i < num_args; i++)
    {
        args[i] = args[i + 1];
    }
    args[num_args] = "";

}

void input_handler(char *input)
{

    // The shell supports a ‘;’ or ‘&’ separated list of commands.
    // You can use ‘strtok’ to tokenize the input.
    // The shell accounts for random spaces and tabs when taking input.
    // The “;” command is used to give multiple commands at the same time. This works similar to how “;” works in Bash.
    // ‘&’ operator runs the command preceding it in the background after printing the process id of the newly created process.

    // USING STRTOK_R
    char *token;
    char *saveptr;
    char *command;
    char *args[100];
    int length_input = strlen(input);

    char *temp = (char *)malloc(length_input * sizeof(char));
    strcpy(temp, input);

    // getting all the arguements for the command
    int num_args = 0;
    while ((token = strtok_r(temp, " ", &saveptr)) != NULL)
    {
        args[num_args] = token;
        num_args++;
        temp = NULL;
    }
    // printf("num_args: %d\n", num_args);

    char *function_name = args[0];
    remove_first_element_from_array(args, num_args);
    num_args--;
    // printf("function_name: %s\n", function_name);

    // printf("num_args: %d\n The arguements are : \n", num_args);

    // for (int i = 0; i < num_args; i++)
    // {
    //     printf("%s\n", args[i]);
    // }
    function_handler(function_name, args, num_args);
}