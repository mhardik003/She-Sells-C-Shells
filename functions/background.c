#include "../headers.h"

void displayOutputAndCleanup()
{
    /*
         Display output of background processes and clean it up from the temporary file
    */
    char buffer[MAX_LEN];
    FILE *f = fopen("/tmp/bg_output_all", "r");

    if (f)
    {
        while (fgets(buffer, sizeof(buffer), f))
        {
            printf("%s", buffer);
        }
        fclose(f);
    }
    remove("/tmp/bg_output_all");
}

void check_bg_processes()
{
    /*
    Function to check if any background processes have ended
    */

    int anyCompleted = 0;

    for (int i = 0; i < bg_count; i++)
    {
        int status;
        pid_t result = waitpid(backgroundProcesses[i], &status, WNOHANG);

        if (result != 0)
        {

            displayOutputAndCleanup();

            if (WIFEXITED(status))
            {
                printf("'%s' exited normally (%d) \n", backgroundProcessNames[i], backgroundProcesses[i]);
                remove_process(backgroundProcesses[i]);
            }
            else if (WIFSIGNALED(status))
            {
                printf("'%s' exited abnormally (%d)\n", backgroundProcessNames[i], backgroundProcesses[i]);
                mark_process_stopped(backgroundProcesses[i]);
            }

            // modify the values in the activities struct as well
 

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

    // update the processList as well for all the processes
    Process *current = processList;

    while (current != NULL)
    {
        int status;
        pid_t result = waitpid(current->pid, &status, WNOHANG);

        if (result != 0)
        {
            if (WIFEXITED(status))
            {
                printf("'%s' exited normally (%d) \n", current->command, current->pid);
                remove_process(current->pid);
            }
            else if (WIFSIGNALED(status))
            {
                printf("'%s' exited abnormally (%d)\n", current->command, current->pid);
                mark_process_stopped(current->pid);
            }
        }
        current = current->next;
    }
}