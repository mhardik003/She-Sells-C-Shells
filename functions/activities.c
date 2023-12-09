#include "../headers.h"

// typedef struct Process
// {
//     char command[BUFFER_SIZE];
//     pid_t pid;
//     int state; // 0: Running, 1: Stopped
//     struct Process *next;
// } Process;

// Process *processList = NULL;

// Finds a process in the linked list by its PID


void add_process(const char *command, pid_t pid)
{
    Process *new_process = (Process *)malloc(sizeof(Process));
    strcpy(new_process->command, command);
    new_process->pid = pid;
    new_process->state = 0; // running by default
    new_process->next = processList;
    processList = new_process;
}

void remove_process(pid_t pid)
{
    Process *current = processList, *previous = NULL;
    while (current != NULL)
    {
        if (current->pid == pid)
        {
            if (previous)
            {
                previous->next = current->next;
            }
            else
            {
                processList = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void mark_process_stopped(pid_t pid)
{
    Process *current = processList;
    while (current != NULL)
    {
        if (current->pid == pid)
        {
            current->state = 1; // stopped
            return;
        }
        current = current->next;
    }
}

void display_activities()
{
    Process *current = processList;
    while (current != NULL)
    {
        printf("%d : %s - %s\n", current->pid, current->command, current->state == 0 ? "Running" : "Stopped");
        current = current->next;
    }
}