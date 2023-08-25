#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100
#define MAX_BG_PROCESSES 100

pid_t backgroundProcesses[MAX_BG_PROCESSES];
int bg_count = 0;

void checkBackgroundProcesses() {
    for (int i = 0; i < bg_count; i++) {
        int status;
        pid_t result = waitpid(backgroundProcesses[i], &status, WNOHANG);

        if (result != 0) {
            if (WIFEXITED(status)) {
                printf("Background process with PID %d completed\n", backgroundProcesses[i]);
            } else if (WIFSIGNALED(status)) {
                printf("Background process with PID %d was terminated by a signal\n", backgroundProcesses[i]);
            }

            // Shift the array to remove the completed PID
            for (int j = i; j < bg_count - 1; j++) {
                backgroundProcesses[j] = backgroundProcesses[j + 1];
            }
            bg_count--;
            i--; // Adjust index after shifting elements
        }
    }
}

void executeCommand(char *command[], int isBackground) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        if (isBackground) {
            int devnull = open("/dev/null", O_WRONLY);
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        if (execvp(command[0], command) == -1) {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    } else {
        if (isBackground) {
            printf("Process started in background with PID: %d\n", pid);
            backgroundProcesses[bg_count++] = pid;
        } else {
            waitpid(pid, NULL, 0);
        }
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *command[MAX_ARG_SIZE];

    while (1) {
        int isBackground = 0;

        checkBackgroundProcesses();  // Check for completed background processes

        printf("my_terminal$ ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (input[len - 2] == '&') {
            isBackground = 1;
            input[len - 2] = '\0';
        }

        char *token = strtok(input, " ");
        int i = 0;
        while (token) {
            command[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        command[i] = NULL;

        if (i == 0) {
            continue;
        }

        if (strcmp(command[0], "exit") == 0) {
            break;
        }

        executeCommand(command, isBackground);
    }

    return 0;
}
