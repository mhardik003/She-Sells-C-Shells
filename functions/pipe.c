#include "../headers.h"

void pipe_handler(char *cmd)
{
    char *commands[100];
    char *command;
    int n = 0, p[2], fd_in = 0;

    command = strtok(cmd, "|");
    while (command)
    {
        commands[n] = command;
        n++;
        command = strtok(NULL, "|");
    }
    commands[n] = NULL;

    for (int i = 0; i < n; i++)
    {
        pipe(p);
        if (fork() == 0)
        {
            dup2(fd_in, 0);
            if (commands[i + 1])
            {
                dup2(p[1], 1);
            }
            close(p[0]);

            // char *input_file = strstr(commands[i], "<");
            // char *output_file = strstr(commands[i], ">");
            // char *args[100];

            // if (input_file)
            // {
            //     *input_file = '\0';
            //     input_file++;
            // }

            // if (output_file)
            // {
            //     *output_file = '\0';
            //     output_file++;
            // }

            // split(commands[i], args);
            // execute_command(args, input_file, output_file);

            input_handler(commands[i]);

            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
            close(p[1]);
            fd_in = p[0];
        }
    }
}
