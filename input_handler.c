#include "utils.h"
#include "headers.h"

// void handler() {
//     if (!fgRun) {
//         printf("\n");
//         prompt(user,hostname,cwd,root,fgTime);
//     }
//     fgRun = 0;
// }

void check_background_processes()
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("%s exited normally (%d)\n", "Command", pid);
        }
        else
        {
            printf("%s exited abnormally (%d)\n", "Command", pid);
        }
    }
}

int findWord(char *word, char *line)
{
    // break the line by spaces or tabs into tokens and check if one of the tokens is word

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

    for (int i = 0; i < num_args; i++)
    {
        args[i] = args[i + 1];
    }
    args[num_args] = "";
}

void single_input_handler(char *input, int is_background)
{
    char *token;
    char *saveptr;
    char *command;
    char *args[100];
    int length_input = strlen(input);

    char *temp = (char *)malloc(length_input * sizeof(char));
    strcpy(temp, input);

    // getting all the arguements for the command
    int num_args = 0;
    token = strtok_r(temp, " ", &saveptr);

    // trim the sides of the token
    // token = trim(token);
    // printf("token: %s\n", token);

    while (token)
    {
        args[num_args] = token;
        num_args++;
        temp = NULL;
        token = strtok_r(NULL, " ", &saveptr);
    }
    args[num_args] = NULL;
    // printf("Single output : %s\n", args[0]);
    // printf("num_args: %d\n", num_args);

    char *function_name = args[0];
    remove_first_element_from_array(args, num_args);
    num_args--;

    // printf("function_name: %s\n", function_name);
    // printf("num_args: %d\nThe arguements are : \n", num_args);
    // for (int i = 0; i < num_args; i++)
    // {
    //     printf("%s\n", args[i]);
    // }
    pid_t pid = fork();
    if (pid == 0)
    { // Child process
        execvp(args[0], args);
        exit(0);
    }
    else
    { // Parent process
        if (is_background)
        {
            printf("%d\n", pid);
        }
        else
        {
            time_t start_time = time(NULL);
            waitpid(pid, NULL, 0);
            time_t end_time = time(NULL);
            time_t duration = end_time - start_time;

            if (duration > 2)
            {
                printf("THE PREVIOUS COMMAND TOOK MORE THAN 2 SECONDS\n");
            }
            else
            {
                printf("PREVIOUS COMMAND TOOK LESSER THAN 2 SECONDS\n");
            }
        }
    }
    function_handler(function_name, args, num_args);
}

void input_handler(char *input)
{
    // The shell supports a ‘;’ or ‘&’ separated list of commands.
    // You can use ‘strtok’ to tokenize the input.
    // The shell accounts for random spaces and tabs when taking input.
    // The “;” command is used to give multiple commands at the same time. This works similar to how “;” works in Bash.
    // ‘&’ operator runs the command preceding it in the background after printing the process id of the newly created process.
    // Remove newline character from the line if present
    if (input[strlen(input) - 1] == '\n')
    {
        input[strlen(input) - 1] = '\0';
    }
    char *lastCommand = read_lastLine();

    lastCommand[strlen(lastCommand) - 1] = '\0';

    if (strcmp(lastCommand, input) != 0 && (findWord("history", input) && (findWord("pastevents", input))))
    {
        addLineToHistory(input);
    }

    // printf("The command to be exectued : %s\n", input);

    if (strlen(input) > 0 && input[strlen(input) - 1] == '\n')
    {
        input[strlen(input) - 1] = '\0';
    }
    char *token = strtok(input, ";");
    while (token)
    {
        // printf("INDIVIDUAL COMMAND : %s\n", token);
        char *cmd = token;
        int background = 0;

        // If command ends with '&', set the background flag
        if (cmd[strlen(cmd) - 1] == '&')
        {
            background = 1;
            cmd[strlen(cmd) - 1] = '\0'; // remove the '&' from the command
        }

        single_input_handler(cmd, background);

        token = strtok(NULL, ";");
    }
}