#include "utils.h"
#include "headers.h"

int findWord(char *word, char *line)
{
    /*
    Breaks the line by spaces or tabs into tokens and check if one of the tokens is word
    */

    // Specially created to check for the history command (if the command is history, don't add it to the history file)

    char *tempcopy = (char *)malloc(strlen(line) * sizeof(char));
    strcpy(tempcopy, line);

    // Tokenize the line to check for the word
    char *token = strtok(tempcopy, " ;&");
    while (token != NULL)
    {
        // printf("token: %s\n", token);
        if (strcmp(token, word) == 0)
        {

            return 0;
        }
        token = strtok(NULL, " ;&");
    }

    // printf("Word not found in the line.\n");
    return 1;
}

void trimString(char *str)
{
    /*
    Trims the string by removing leading and trailing spaces
    */

    // Remove leading spaces
    while (isspace((unsigned char)*str))
    {
        str++;
    }

    if (*str == '\0')
    { // If the string only contained spaces
        return;
    }

    // Remove trailing spaces
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
    {
        end--;
    }
    *(end + 1) = '\0'; // Terminate the string after the last non-space character
}

void remove_first_element_from_array(char *args[], int num_args)
{

    /*
    Removes the first element from the array
    */

    //  Being used to seperate the commands from their arguements

    for (int i = 0; i < num_args; i++)
    {
        args[i] = args[i + 1];
    }
    args[num_args] = "";
}

void execute_command(char *input, int is_background)
{
    /*
        Recieves the command from the input handler and executes it
    */

    char *token;     // to store the tokens of the input one by one
    char *saveptr;   // to store the pointer to the next token for the strtok function
    char *args[100]; // to store the arguements of the command

    int length_input = strlen(input);

    char *temp = (char *)malloc(length_input * sizeof(char)); // to store the input temporarily
    strcpy(temp, input);

    // getting all the arguements for the command
    int num_args = 0;
    token = strtok_r(temp, " \t", &saveptr);
    while (token)
    {
        args[num_args] = token;
        num_args++;
        temp = NULL;
        token = strtok_r(NULL, " \t", &saveptr);
    }

    // printf("num_args: %d\n", num_args);

    char *function_name = args[0];
    // printf("function_name: %s\n", function_name);

    remove_first_element_from_array(args, num_args); // so that args only has the arguements and not the function name
    num_args--;                                      // since we removed the first element from the array

    args[num_args] = NULL;

    function_handler(function_name, args, num_args, is_background);
}

void input_handler(char *input)
{
    /*
        Handles the input from the user by taking in the input string and seperating based on &  and ;
        and then calling the execute_command function
    */
    int is_empty = 1;
    for (int i = 0; i < strlen(input); i++)
    {
        if (isalnum(input[i]))
        {
            is_empty = 0;
            break;
        }
    }
    if (is_empty)
    {
        return;
    }

    addCommandToHistory(input);

    // printf("The command to be executed : %s\n", input);

    char *input_copy = (char *)malloc(strlen(input) * sizeof(char));
    strcpy(input_copy, input);
    char *token = strtok(input_copy, ";&");

    int curr_index = 0;
    int first_token = 1;
    while (token)
    {
        // printf("INDIVIDUAL COMMAND : %s\n", token);
        char *cmd = token;
        int background = 0;

        // If command ends with '&', set the background flag
        curr_index = curr_index + strlen(token);

        if (curr_index < strlen(input) && !first_token )
        {
            curr_index++;
        }
        if (input[curr_index] == '&')
        {
            background = 1;
        }

        // printf("Index pos : %d\n", curr_index);
        // printf("Character at index pos : %c\n", input[curr_index]);

        trimString(cmd);
        // printf("Command to be executed: %s\n", cmd);
        // printf("Background flag : %d\n", background);
        execute_command(cmd, background);

        token = strtok(NULL, ";&");
        first_token = 0;
    }
}