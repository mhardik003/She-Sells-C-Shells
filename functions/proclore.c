#include "../headers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define PATH_SIZE 4096

void print_virtualMemUsage(int pid)
{
    char path[PATH_SIZE];
    char buf[BUF_SIZE];
    FILE *f;

    sprintf(path, "/proc/%d/status", pid);
    f = fopen(path, "r");
    if (!f)
    {
        perror("Failed to open process status");
        return;
    }
    long int vm_kb = 0; // Initialize virtual memory size in KB
    while (fgets(buf, sizeof(buf), f))
    {
        if (sscanf(buf, "VmSize: %ld kB", &vm_kb) == 1)
        {
            break;
        }
    }
    fclose(f);

    // Convert the value to bytes
    long int vm_bytes = vm_kb * 1024;

    // ...

    printf("Virtual Memory: %ld bytes\n", vm_bytes);
}

void print_Process_info(int pid)
{

    char buf[BUF_SIZE];
    char path[PATH_SIZE];
    FILE *f;

    // Get process info from /proc/[pid]/stat
    sprintf(path, "/proc/%d/stat", pid);
    f = fopen(path, "r");
    if (!f)
    {
        perror("Failed to open process stat");
        return;
    }
    if (!fgets(buf, sizeof(buf), f))
    {
        perror("Failed to read process stat");
        fclose(f);
        return;
    }
    fclose(f);

    int proc_pid, proc_ppid, proc_pgrp, proc_session, proc_tty_nr, proc_tpgid;
    char proc_comm[PATH_SIZE], proc_state;
    long int proc_vsize;
    sscanf(buf,
           "%d %s %c %d %d %d %d %d %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %ld",
           &proc_pid, proc_comm, &proc_state, &proc_ppid, &proc_pgrp, &proc_session,
           &proc_tty_nr, &proc_tpgid, &proc_vsize);

    // Get process command line from /proc/[pid]/cmdline
    sprintf(path, "/proc/%d/cmdline", pid);
    f = fopen(path, "r");
    if (!f)
    {
        perror("Failed to open process cmdline");
        return;
    }
    if (!fgets(buf, sizeof(buf), f))
    {
        perror("Failed to read process cmdline");
        fclose(f);
        return;
    }
    fclose(f);

    char *foreground_background = "";
    if (proc_tpgid == proc_pgrp)
    {
        foreground_background = "+";
    }

    printf("PID: %d\n", proc_pid);
    printf("Status: %c%s\n", proc_state, foreground_background);
    printf("Process Group: %d\n", proc_pgrp);
    print_virtualMemUsage(pid);

    // Get full executable path from /proc/[pid]/exe
    sprintf(path, "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len == -1)
    {
        perror("Failed to get executable path");
        return;
    }
    buf[len] = '\0';

    printf("Executable Path: %s\n", buf);
}

void proclore_driver(int argc, char *argv[])
{
    pid_t pid;

    if (argc < 1)
    {
        pid = getpid(); // get the process ID of the current process
    }
    else
    {
        pid = atoi(argv[0]);
    }

    print_Process_info(pid);

    return;
}
