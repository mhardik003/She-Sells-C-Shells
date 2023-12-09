#include "../headers.h"
Process *find_process(pid_t pid)
{
    Process *current = processList;
    while (current != NULL)
    {
        if (current->pid == pid)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void fg_command(int num_args, char *args[])
{
    if (num_args >= 2)
    {
        printf("Usage: fg <pid>\n");
        return;
    }
    int pid = atoi(args[0]);
    Process *process = find_process(pid);
    if (process == NULL)
    {
        printf("No such process found\n");
        return;
    }

    // Move the process to the foreground
    tcsetpgrp(STDIN_FILENO, process->pid);

    // If the process is stopped, send SIGCONT
    if (process->state == 1)
    {
        kill(process->pid, SIGCONT);
        process->state = 0; // set the state to running
    }

    // Wait for the process to complete
    int status;
    waitpid(process->pid, &status, WUNTRACED);

    // Return control to the shell
    tcsetpgrp(STDIN_FILENO, getpid());
}

void bg_command(int num_args, char *args[])
{
    if (num_args > 1 || num_args == 0)
    {
        printf("Usage: bg <pid>\n");
        return;
    }
    int pid = atoi(args[0]);
    Process *process = find_process(pid);
    if (process == NULL)
    {
        printf("No such process found\n");
        return;
    }

    if (process->state == 1)
    { // if the process is stopped
        kill(process->pid, SIGCONT);
        process->state = 0; // set the state to running
    }
}