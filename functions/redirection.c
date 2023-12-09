#include "../headers.h"

// Function to execute the command with optional I/O redirection
void execute_command_redirection(char **args, char *input_file, char *output_file, int append_mode) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (input_file) {
            int in = open(input_file, O_RDONLY);
            if (in < 0) {
                printf("No such input file found!");
                exit(EXIT_FAILURE);
            }
            dup2(in, STDIN_FILENO);
            close(in);
        }
        if (output_file) {
            int out;
            if (append_mode) {
                out = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
            } else {
                out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            if (out < 0) {
                printf("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(out, STDOUT_FILENO);
            close(out);
        }

        if (execvp(args[0], args) == -1) {
            perror("Shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("Shell");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}


void redirection(char *input) {
    
    size_t len = strlen(input);

    char *input_file = NULL;
    char *output_file = NULL;
    int append_mode = 0;

    // Split the command and arguments
    char *args[64];
    int i = 0;
    char *token = strtok(input, " \t\r\n");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token) {
                input_file = strdup(token);
            }
        } else if (strcmp(token, ">") == 0) {
            append_mode = 0;
            token = strtok(NULL, " \t\r\n");
            if (token) {
                output_file = strdup(token);
            }
        } else if (strcmp(token, ">>") == 0) {
            append_mode = 1;
            token = strtok(NULL, " \t\r\n");
            if (token) {
                output_file = strdup(token);
            }
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \t\r\n");
    }

    args[i] = NULL;

    if (i > 0) {
        execute_command_redirection(args, input_file, output_file, append_mode);
    }

    free(input_file);
    free(output_file);


   return;
}