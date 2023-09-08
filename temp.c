#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Function to execute a single command
void execute_command(char *command, int input_fd, int output_fd) {
    char *args[256];
    char *token = strtok(command, " ");
    int i = 0;

    while (token) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Fork and execute the command
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else {
        wait(NULL);
    }
}

// Function to parse and execute a pipelined command
void execute_pipeline(char *command) {
    char *commands[256];
    char *token = strtok(command, "|");
    int i = 0, input_fd = STDIN_FILENO, output_fd, fd[2];

    while (token) {
        commands[i++] = token;
        token = strtok(NULL, "|");
    }

    for (int j = 0; j < i; ++j) {
        // If it's the last command, set output to STDOUT
        if (j == i - 1) {
            output_fd = STDOUT_FILENO;
        } else {
            // Otherwise create a pipe
            if (pipe(fd) < 0) {
                perror("pipe");
                exit(1);
            }
            output_fd = fd[1];
        }

        // Check for I/O redirection
        char *input_file = strstr(commands[j], "<");
        char *output_file = strstr(commands[j], ">");
        
        if (input_file) {
            *input_file = '\0';
            input_file++;
            input_file = strtok(input_file, " \t\n");
            input_fd = open(input_file, O_RDONLY);
            if (input_fd < 0) {
                perror("open");
                exit(1);
            }
        }

        if (output_file) {
            *output_file = '\0';
            output_file++;
            output_file = strtok(output_file, " \t\n");
            output_fd = open(output_file, O_WRONLY | O_CREAT, 0644);
            if (output_fd < 0) {
                perror("open");
                exit(1);
            }
        }

        execute_command(commands[j], input_fd, output_fd);

        // Close output and make the read end of the pipe the new input
        if (j != i - 1) {
            close(fd[1]);
            input_fd = fd[0];
        }
    }
}

int main() {
    char command[1024];

    while (1) {
        printf("<JohnDoe@SYS:~> ");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0'; // Remove the trailing newline

        if (strcmp(command, "exit") == 0) {
            break;
        }

        execute_pipeline(command);
    }

    return 0;
}
