#include "../headers.h"

#define BUF_SIZE 1024
#define PATH_SIZE 4096

void proclore(int num_args, char *argv[])
{
    // pid_t pid;
    // char buf[BUF_SIZE];
    // char path[PATH_SIZE];
    // FILE *f;

    // if (num_args == 0)
    // {
    //     pid = getppid(); // get the process ID of the current process
    // }
    // else
    // {
    //     pid = atoi(argv[0]);
    // }

    int pid;
    if (num_args == 0)
        pid = getpid();
    else
        pid = atoi(argv[0]);
    char path[200];
    char pid_str[20];
    sprintf(pid_str, "%d", pid);

    strcpy(path, "/proc/");
    strcat(path, pid_str);
    strcat(path, "/stat");
    FILE *fd = fopen(path, "r");
    if (!fd)
    {
        printf("Error: Process with pid %d not found\n", pid);
        return;
    }
    int memory;
    int bg = 0;
    char state = 'R';
    int proc_pid, proc_ppid, proc_pgrp, proc_session, proc_tty_nr, proc_tpgid;
    char proc_comm[PATH_SIZE], proc_state;
    long int proc_vsize;
    sprintf(path, "/proc/%d/stat", pid);
    // fscanf(fd,
    //        "%d %s %c %d %d %d %d %d %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s "
    //        "%*s %*s %*s %*s %*s %*s %*s %*s %*s %ld",
    //        &proc_pid, proc_comm, &proc_state, &proc_ppid, &proc_pgrp, &proc_session,
    //        &proc_tty_nr, &proc_tpgid, &proc, );
    fscanf(fd,
           "%d %s %c %d %d %d %d %d %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %ld",
           &proc_pid, proc_comm, &proc_state, &proc_ppid, &proc_pgrp, &proc_session,
           &proc_tty_nr, &proc_tpgid, &proc_vsize);
    fclose(fd);
    char *foreground_background = "";
    if (proc_tpgid == proc_pgrp)
    {
        foreground_background = "+";
    }

    char exec[128];
    sprintf(path, "/proc/%d/exe", pid);
    realpath(path, exec);

    printf("PID: %d\n", proc_pid);
    printf("Status: %c%s\n", proc_state, foreground_background);
    printf("Process Group: %d\n", proc_pgrp);
    printf("Virtual Memory: %ld bytes\n", proc_vsize);
    printf("Executable Path: %s\n", exec);
}
