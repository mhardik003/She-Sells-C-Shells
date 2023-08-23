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
    /*
    Checks for background processes which are yet to complete
    */

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


// int runCommand(char* args[],int bg,int* fgTime){
//     char path[64];
//     int pid;
//     sprintf(path,"/usr/bin/%s",args[0]);
//     FILE* file = fopen(path,"r");
//     if(file){ // Checking if the given command exists or not
//         pid = fork();
//         fclose(file);
//     }else{
//         printf("Error: Command Not Found\n");
//         return 0;
//     }
//     if(pid < 0){
//         perror("Error");
//         return -1;
//     }else if(!pid){
//         setpgid(0,0);

//         if(execvp(path,args) < 0){
//             perror("Error");
//             return -1;
//         }
//     }else{
//         if(bg){
//             printf("%s process started with pid: %d\n",args[0],pid);
//             return pid;
//         }else{
//             int status;
//             signal(SIGTTIN, SIG_IGN);
//             signal(SIGTTOU, SIG_IGN);

//             tcsetpgrp(0, pid);

//             clock_t t;
//             t = time(NULL);

//             waitpid(pid, &status, WUNTRACED);

//             t = time(NULL) - t;
//             *fgTime = t;

//             tcsetpgrp(0, getpgid(0));

//             signal(SIGTTIN, SIG_DFL);
//             signal(SIGTTOU, SIG_DFL);

//             if (WIFSTOPPED(status)) return pid; // Ctrl+Z

//             if (WEXITSTATUS(status) == EXIT_FAILURE) return -1;            
//         }
//     }
// }

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
    token = strtok_r(temp, " ", &saveptr);
    while (token)
    {
        args[num_args] = token;
        num_args++;
        temp = NULL;
        token = strtok_r(NULL, " ", &saveptr);
    }
    args[num_args] = NULL;

    // printf("num_args: %d\n", num_args);

    char *function_name = args[0];
    // printf("function_name: %s\n", function_name);

    remove_first_element_from_array(args, num_args); // so that args only has the arguements and not the function name
    num_args--;                                      // since we removed the first element from the array

    // pid_t pid = fork();
    // if (pid == 0)
    // { // Child process
    //     execvp(args[0], args);
    //     exit(0);
    // }
    // else
    // { // Parent process
    //     if (is_background)
    //     {
    //         printf("Hiiii\n");
    //     }
    //     else
    //     {
    //         time_t start_time = time(NULL);
    //         waitpid(pid, NULL, 0);
    //         time_t end_time = time(NULL);
    //         time_t duration = end_time - start_time;

    //         // if (duration > 2)
    //         // {
    //         //     printf("THE PREVIOUS COMMAND TOOK MORE THAN 2 SECONDS\n");
    //         // }
    //         // else
    //         // {
    //         //     printf("PREVIOUS COMMAND TOOK LESSER THAN 2 SECONDS\n");
    //         // }
    //     }
    // }

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

        execute_command(cmd, background);

        token = strtok(NULL, ";");
    }
}