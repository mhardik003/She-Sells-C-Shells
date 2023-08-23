#include "../headers.h"

char* checkSlashes( char *path)
{
    int len = strlen(path);
    int temp_loc=0;
    for (int i = 0; i < len; i++)
    {
        if (path[i] == '/')
        {
            temp_loc=i;
        }
        else
        {
            break;
        }
    }

    // printf("Number of forward slashes : %d", temp_loc);
    char *temp = (char *)malloc(LEN_PWD * sizeof(char));
    strncpy(temp,path+temp_loc,1024);
    // printf("temp : %s\n",temp);
    return temp;
}

int isValidDirectory( char *path)
{
    strcpy(path,checkSlashes(path));
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

char *get_prev_directory_string(char *path)
{
    char *last_slash = strrchr(path, '/');
    // printf(" PATH : '%s'\n", path);
    if (last_slash)
    {
        if (strcmp(path, "/") != 0)
        {
            *last_slash = '\0';
        }
    }
    else
    {
        strcpy(path, "/");
    }
    return path;
}

void change_directory(char *path)
{
    if (strcmp(path, ".") == 0)
    {
        // DO NOTHING
        printf("%s\n", CURR_PWD);

    }
    else if (strcmp(path, "..") == 0)
    {
        // Go to previous directory
        strcpy(PREV_PWD, CURR_PWD);
        strcpy(CURR_PWD, isValidDirectory(get_prev_directory_string(CURR_PWD)) ? get_prev_directory_string(CURR_PWD) : "/");
        printf("%s\n", CURR_PWD);

    }
    else if (strcmp(path, "~") == 0)
    {
        // Go to home directory
        strcpy(PREV_PWD, CURR_PWD);
        strcpy(CURR_PWD, INIT_PWD);
        printf("%s\n", CURR_PWD);

    }
    else if (strcmp(path, "-") == 0)
    {
        // Go to previous directory
        char *temp = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp, CURR_PWD);
        strcpy(CURR_PWD, PREV_PWD);
        strcpy(PREV_PWD, temp);
        printf("%s\n", CURR_PWD);

    }
    else if (path[0] == '/')
    {
        if (isValidDirectory(path))
        {
            strcpy(PREV_PWD, CURR_PWD);
            strcpy(CURR_PWD, path);
            printf("%s\n", CURR_PWD);
        }
        else
        {
            printf("warp: The directory “%s” does not exist\n", path);
            return;
        }
    }
    else
    {
        // Go to the specified directory
        if(path[0]=='.' && path[1]=='/')
        {
            path = path+2;
        }

        char *temp_dir = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp_dir, CURR_PWD);
        strcat(temp_dir, "/");
        strcat(temp_dir, path);

        // printf("temporary path to be check for validity : %s\n", temp_dir);
        if (isValidDirectory(temp_dir))
        {
            strcpy(PREV_PWD, CURR_PWD);
            if (CURR_PWD[strlen(CURR_PWD) - 1] != '/')
            {

                strcat(CURR_PWD, "/");
            }

            strcat(CURR_PWD, path);
            printf("%s\n", CURR_PWD);
        }
        else
        {
            printf("warp: The directory “%s” does not exist\n", path);
            return;
        }
    }
}

void warp(char *args[], int num_args)
{

    // printf("entering the warp function\n");
    // printf("number of args : %d",num_args );

    if (num_args == 0)
    {
        // Go to home directory
        strcpy(PREV_PWD, CURR_PWD);
        strcpy(CURR_PWD, INIT_PWD);
    }

    for (int i = 0; i < num_args; i++)
    {
        // printf("Going to %s", args[i]);
        change_directory(args[i]);
    }
}