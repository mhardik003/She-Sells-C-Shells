#include "../headers.h"

void check_bg_processes()
{
    /*
    Function to check if any background processes have ended
    */

    for (int i = 0; i < bg_count; i++)
    {
        int status;
        pid_t result = waitpid(backgroundProcesses[i], &status, WNOHANG);

        if (result != 0)
        {
            if (WIFEXITED(status))
            {
                printf("'%s' exited normally (%d) \n", backgroundProcessNames[i], backgroundProcesses[i]);
            }
            else if (WIFSIGNALED(status))
            {
                printf("'%s' exited abnormally (%d)\n", backgroundProcessNames[i], backgroundProcesses[i]);
            }

            // Shift the array to remove the completed PID
            for (int j = i; j < bg_count - 1; j++)
            {
                backgroundProcesses[j] = backgroundProcesses[j + 1];
                strcpy(backgroundProcessNames[j], backgroundProcessNames[j + 1]);
            }
            bg_count--;
            i--; // Adjust index after shifting elements
        }
    }
}