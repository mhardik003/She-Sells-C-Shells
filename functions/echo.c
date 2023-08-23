#include "../headers.h"

void echo(int num_args , char *args[] )
{
    if (num_args == 0)
    {
        printf("\n");
    }
    else
    {
        for (int i = 0; i < num_args; i++)
        {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
}