#include "headers.h"

void replaceSubstring(char *str, const char *old, const char *new_str)
{
    /*
    Function to replace all occurences of old in str with new_str
    */

    // Used to replace the home directory with ~ in the prompt

    char *pos, temp[1000];
    int index = 0;
    int old_len = strlen(old);

    while ((pos = strstr(str, old)) != NULL)
    {
        strcpy(temp, str);
        index = pos - str;
        str[index] = '\0';
        strcat(str, new_str);
        strcat(str, temp + index + old_len);
    }
}

void print_pwd()
{
    /*
    Function to print the current working directory for the prompt
    */

    if (strstr(CURR_PWD, INIT_PWD))
    {
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
    /*
    Function to print the prompt
    */

    // add green colour to the USER_NAME
    printf("\033[1;32m<%s", USER_NAME);
    printf("\033[0m");           // reset the colour
    printf("@%s:", SYSTEM_NAME); // print the system name
    print_pwd();                 // print the current working directory
    printf("\033[0m");           // reset the colour
}
