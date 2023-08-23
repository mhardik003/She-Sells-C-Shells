#include "headers.h"

void replaceSubstring(char *str, const char *old, const char *new_str) {
    char *pos, temp[1000];
    int index = 0;
    int old_len = strlen(old);

    while ((pos = strstr(str, old)) != NULL) {
        strcpy(temp, str);
        index = pos - str;
        str[index] = '\0';
        strcat(str, new_str);
        strcat(str, temp + index + old_len);
    }
}

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

void get_user_and_sys_name(char *user, char *system, int is_Linux, int is_Windows)
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
}

void get_pwd(char *pwd_name)
{
    // get the current working directory
    if (getcwd(pwd_name, LEN_PWD) == NULL)
    {
        strcpy(CURR_PWD, pwd_name);
        perror("getcwd");
        return;
    }
    else
    {
        // printf("Current working directory: %s\n", pwd_name);
    }
}

void print_pwd()
{
    if(strstr(CURR_PWD, INIT_PWD)){
        char temp_pwd[LEN_PWD];
        strcpy(temp_pwd, CURR_PWD);
        replaceSubstring(temp_pwd, INIT_PWD, "~");
        printf("\033[1;34m%s> ", temp_pwd);
    }
    else
    {
        printf("\033[1;34m%s> ", CURR_PWD);

        // printf("meow");
    }
}

void print_prompt()
{

    // printf("<%s@%s: %s>", USER_NAME, SYSTEM_NAME, PWD);

    // add green colour to the USER_NAME
    char username[MAX_LEN];
    char systemname[MAX_LEN];
    char pwd[MAX_LEN];

    int isLinuxTemp = 0;
    int isWindowsTemp = 0;
    isLinux(&isLinuxTemp);
    isWindows(&isWindowsTemp);
    get_user_and_sys_name(username, systemname, isLinuxTemp, isWindowsTemp);
    get_pwd(pwd);
    printf("\033[1;32m<%s", username);
    printf("\033[0m");
    printf("@%s: ", systemname);
    print_pwd();

    printf("\033[0m");
}
