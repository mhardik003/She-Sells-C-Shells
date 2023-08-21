#include "headers.h"

void find_os(int *is_Linux, int *is_Windows)
{
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

void isLinux(int *is_Linux)
{
#ifdef __linux__
    *is_Linux = 1;
#else

    *is_Linux = 0;
#endif
}

void isWindows(int *is_Windows)
{
#ifdef _WIN32
    *is_Windows = 1;
#else

    *is_Windows = 0;
#endif
}

void get_user_and_sys_name(char *user, char *system, char *pwd, int is_Linux, int is_Windows)
{
    // function to find the user name and the system name from the current device

    // find the USER and store it in USER_NAME
    // find if the system is Linux or Windows and then find the username according

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

    // similarly check for windows
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

    // find the current working directory and store it in PWD
    if (getcwd(pwd, sizeof(pwd)) == NULL)
    {
        
        perror("getcwd");
        return;
    }

    // get the current working directory
}

void print_prompt()
{

    // printf("<%s@%s: %s>", USER_NAME, SYSTEM_NAME, PWD);

    // add green colour to the USER_NAME
    char username[MAX_LEN];
    char systemname[MAX_LEN];
    char pwd[MAX_LEN];

    int isLinux = 0;
    int isWindows = 0;
    get_user_and_sys_name(username, systemname, pwd, isLinux, isWindows);
    printf("\033[1;32m<%s", username);
    printf("\033[0m");
    printf("@%s: ", systemname);
    printf("\033[1;34m%s> ", pwd);
}
