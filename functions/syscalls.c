#include "../headers.h"

void syscalls(int num_args, char *args[], char *function_name, int is_bg)
{
    for (int i = num_args; i > 0; i--)
    {
        args[i] = args[i - 1];
    }
    args[0] = function_name;

    // Add NULL to the end of args
    args[num_args + 1] = NULL;

    time_t start, end;
    time(&start);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        if (is_bg)
        {
            int devnull = open("/dev/null", O_WRONLY);
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        if (execvp(args[0], args) == -1)
        {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (is_bg)
        {
            printf("[%d]\n", pid);
            backgroundProcesses[bg_count] = pid;
            strcpy(backgroundProcessNames[bg_count], function_name);
            bg_count++;
        }
        else
        {
            int status;
            waitpid(pid, &status, WUNTRACED);
            time(&end);
            prevElapsedTime = (int)difftime(end, start);
            
        }
    }
}