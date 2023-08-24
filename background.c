#include "headers.h"

void check_bg_processes()
{
    /*
    Function to check if any background processes have ended
    */

    for (int i = 0; i < bg_count; i++)
    {
        int status;
        pid_t result = waitpid(bg_procs[i].pid, &status, WNOHANG);
        if (result != 0)
        {
            if (WIFEXITED(status))
                printf("%s exited normally (%d)\n", bg_procs[i].cmd, bg_procs[i].pid);
            else
                printf("%s exited abnormally (%d)\n", bg_procs[i].cmd, bg_procs[i].pid);

            for (int j = i; j < bg_count - 1; j++)
            {
                bg_procs[j] = bg_procs[j + 1];
            }
            i--;
            bg_count--;
        }
    }
}