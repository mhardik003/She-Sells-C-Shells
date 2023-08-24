#include "headers.h"

void find_os(int *is_Linux, int *is_Windows)
{
    /*
   Function to find if the current system is Linux or Windows
    */

#ifdef __linux__
    *is_Linux = 1;
#else
    *is_Linux = 0;

#endif

#ifdef _WIN32
    *is_Windows = 1;
#else
    *is_Windows = 0;
#endif
}

void get_user_and_sys_name(char *user, char *system, int is_Linux, int is_Windows)
{
    /*
    Function to find the user name and the system name from the current device
    */

    // find if the system is Linux or Windows and then find the username according in USER_NAME global variable
    if (is_Linux)
    {
        if (getenv("USER"))
        {
            strcpy(user, getenv("USER"));
            // printf("%s", getenv("USER"));
        }
        else
        {
            perror("getenv");
            return;
        }
    }

    else if (is_Windows)
    {
        if (getenv("USERNAME"))
        {
            strcpy(user, getenv("USERNAME"));
        }
        else
        {
            perror("getenv");
            return;
        }
    }

    // find the system name and store it in SYSTEM_NAME
    if (gethostname(system, sizeof(system)) == 0)
    {
        // printf("Hostname: %s\n", system);
    }
    else
    {
        perror("gethostname");
        return;
    }
}

void get_pwd(char *pwd_name)
{
    /*
    Function to get the current working directory
    and store it in the function argument pwd_name
    */

    if (getcwd(pwd_name, LEN_PWD) == NULL)
    {
        perror("getcwd");
        return;
    }
}