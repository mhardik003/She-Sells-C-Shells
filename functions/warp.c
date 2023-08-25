#include "../headers.h"

int has_read_permissions(char *path)
{
    /*
        Function to check if the file has read permissions or not
    */

    struct stat st;
    stat(path, &st);
    return (st.st_mode & S_IRUSR);
}

char *checkSlashes(char *path)
{

    /*
        Function to check if the path has forward slashes at the beginning
    */

    int len = strlen(path);
    int temp_loc = 0;
    for (int i = 0; i < len; i++)
    {
        if (path[i] == '/')
        {
            temp_loc = i;
        }
        else
        {
            break;
        }
    }

    // printf("Number of forward slashes : %d", temp_loc);
    char *temp = (char *)malloc(LEN_PWD * sizeof(char));
    strncpy(temp, path + temp_loc, 1024);
    // printf("temp : %s\n",temp);
    return temp;
}

int isValidDirectory(char *path)
{
    /*
        Function to check if the directory is valid or not
    */

    strcpy(path, checkSlashes(path));
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

char *get_prev_directory_string(char *path)
{
    /*
        Function to get the previous directory string
    */

    char *last_slash = strrchr(path, '/');

    if (last_slash)
    {
        if (strcmp(path, "/") != 0)
        {

            *last_slash = '\0';
        }
        strcat(path, "/");
    }
    else
    {
        strcpy(path, "/");
    }
    // printf("path after cd .. : %s\n", path);
    return path;
}

void change_directory(char *path)
{

    /*
        Function to change the directory
    */

    if (strcmp(path, ".") == 0)
    {
        // DO NOTHING
        printf("%s\n", CURR_PWD);
    }
    else if (strcmp(path, "..") == 0)
    {
        // Go to previous directory
        char *temp = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp, CURR_PWD);
        if (isValidDirectory(get_prev_directory_string(CURR_PWD)))
        {
            if (has_read_permissions(CURR_PWD))
            {
                strcpy(PREV_PWD, temp);
                strcpy(CURR_PWD, CURR_PWD);
                chdir(CURR_PWD);
            }
            else
            {
                printf("Missing permissions for the task\n");
                strcpy(CURR_PWD, temp);
            }
        }
        printf("%s\n", CURR_PWD);
    }
    else if (path[0] == '~')
    {
        if (path[1] == '/')
        {
            path = path + 2;
            char *temp_dir = (char *)malloc(LEN_PWD * sizeof(char));
            strcpy(temp_dir, HOME_DIR);
            strcat(temp_dir, "/");
            strcat(temp_dir, path);
            if (isValidDirectory(temp_dir))
            {
                strcpy(PREV_PWD, CURR_PWD);
                strcpy(CURR_PWD, HOME_DIR);
                chdir(CURR_PWD);
                change_directory(path);
            }
        }
        else if (strcmp(path, "~") == 0)
        {
            // Go to home directory
            strcpy(PREV_PWD, CURR_PWD);
            strcpy(CURR_PWD, HOME_DIR);
            printf("%s\n", CURR_PWD);
            chdir(CURR_PWD);
        }
        else
        {
            printf("warp: The directory “%s” does not exist\n", path);
            return;
        }
    }
    else if (strcmp(path, "-") == 0)
    {

        if (strcmp(PREV_PWD, "NULL") == 0)
        {
            printf("warp: OLDPWD not set\n");
            return;
        }

        // Go to previous directory
        char *temp = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp, CURR_PWD);
        strcpy(CURR_PWD, PREV_PWD);
        strcpy(PREV_PWD, temp);
        printf("%s\n", CURR_PWD);
        chdir(CURR_PWD);
    }
    else if (path[0] == '/')
    {
        if (isValidDirectory(path))
        {
            if (has_read_permissions(path))
            {
                // printf("yo\n");
                strcpy(PREV_PWD, CURR_PWD);
                strcpy(CURR_PWD, path);
                printf("%s\n", CURR_PWD);
                chdir(CURR_PWD);
            }
            else
            {
                printf("Missing permissions for the task\n");
            }
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
        if (path[0] == '.' && path[1] == '/')
        {
            path = path + 2;
        }

        char *temp_dir = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp_dir, CURR_PWD);
        strcat(temp_dir, "/");
        strcat(temp_dir, path);

        // printf("temporary path to be check for validity : %s\n", temp_dir);
        if (isValidDirectory(temp_dir))
        {
            if (has_read_permissions(temp_dir))
            {
                strcpy(PREV_PWD, CURR_PWD);
                if (CURR_PWD[strlen(CURR_PWD) - 1] != '/')
                {

                    strcat(CURR_PWD, "/");
                }

                strcat(CURR_PWD, path);
                printf("%s\n", CURR_PWD);
                chdir(CURR_PWD);
            }
            else
            {
                printf("Missing permissions for the task\n");
            }
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
    /*
        Function to change the directory
    */

    if (num_args == 0)
    {
        // Go to home directory
        chdir(HOME_DIR);
        strcpy(PREV_PWD, CURR_PWD);
        strcpy(CURR_PWD, HOME_DIR);
    }

    for (int i = 0; i < num_args; i++)
    {
        // printf("Going to %s", args[i]);
        change_directory(args[i]);
    }
}