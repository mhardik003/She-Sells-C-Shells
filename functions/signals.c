#include "../headers.h"

pid_t foreground_pid = -1;

void cleanup_and_exit()
{
    Process *current = processList;
    while (current != NULL)
    {
        kill(current->pid, SIGKILL);
        current = current->next;
    }

    exit_call_bool = 1;
}

void handle_sigint(int sig)
{
    // printf("meow\n");
    if (foreground_pid != -1)
    {
        
        printf("Foreground process killed with pid %d\n", foreground_pid);
        // remove_process(foreground_pid);
        kill(foreground_pid, SIGINT);
        remove_process(foreground_pid);
        foreground_pid = -1;
    }
    // printf("\n");
    fflush(stdout);
    // print_prompt();
}

void handle_sigz(int sig)
{
    if (foreground_pid != -1)
    {
        kill(foreground_pid, SIGSTOP);
        printf("Foreground process stopped with pid %d\n", foreground_pid);
        mark_process_stopped(foreground_pid);
        foreground_pid = -1;
    }
}

void setup_signal_handlers()
{

    struct sigaction sa_int, sa_z;

    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    // printf("meow\n");
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    sa_z.sa_handler = handle_sigz;
    sa_z.sa_flags = SA_RESTART;
    if (sigaction(SIGTSTP, &sa_z, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

void ping_func(int num_args, char *args[])
{

    pid_t pid;
    int signal_num;

    pid = atoi(args[0]);
    signal_num = atoi(args[1]) % 32;

    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
    }
    else
    {
        kill(pid, signal_num);
        if (signal_num == 9)
            remove_process(pid);
        printf("Sent signal %d to process with pid %d\n", signal_num, pid);
    }
}
