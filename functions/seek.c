#include "../headers.h"

void get_name_without_extension(const char *filename, char *name)
{
    /*
    Return the name of the file without the extension
    */

    // Since we have to check for the file names without the extension

    strcpy(name, filename);
    char *dot = strrchr(name, '.');
    if (dot)
    {
        *dot = '\0'; // Truncate the name at the last dot
    }
}

void seek_recursive(const char *name, const char *search, int file_flag, int dir_flag, int exact_flag, int *count, char *exactMatch)
{
    /*
        Recursive function to search for the file in the directory
    */

    DIR *dir;
    struct dirent *entry;
    char *name_without_extension = (char *)malloc(1024 * sizeof(char));

    if (!(dir = opendir(name)))
    {
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        get_name_without_extension(entry->d_name, name_without_extension);
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            strcpy(path, name);
            strcat(path, "/");
            strcat(path, entry->d_name);

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            if (strcmp(name_without_extension, search) == 0 && (dir_flag))
            {
                // printf("DIR: %s\n", entry->d_name);
                (*count)++;
                if (*count == 1 && (exact_flag))
                {
                    strcpy(exactMatch, path);
                }

                printf("\033[1;34m%s/\033[0m\n", path);
            }

            seek_recursive(path, search, file_flag, dir_flag, exact_flag, count, exactMatch);
        }
        else
        {
            // printf("FILE: %s\n", entry->d_name);
            char filePath[1024];
            strcpy(filePath, name);
            strcat(filePath, "/");
            strcat(filePath, entry->d_name);

            if (strcmp(name_without_extension, search) == 0 && file_flag)
            {
                (*count)++;
                if (*count == 1 && (exact_flag))
                {
                    strcpy(exactMatch, filePath);
                }
                else
                {
                    printf("\033[1;32m%s\033[0m\n", filePath);
                }
            }
        }
    }
    closedir(dir);
}

void seek(int num_args, char *args[])
{
    /*
        Function to search for the file in the directory
    */

    int file_bool = 0;
    int dir_bool = 0;
    int file_flag = 1;
    int dir_flag = 1;
    int exact_flag = 0;
    char *search = NULL;
    char *target_dir = ".";
    int count = 0;
    char exactMatch[1024] = {0};

    for (int i = 0; i < num_args; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] == 'd')
                {
                    dir_bool = 1;
                }
                else if (args[i][j] == 'f')
                {
                    file_bool = 1;
                }
                else if (args[i][j] == 'e')
                {
                    exact_flag = 1;
                }
            }
        }
        else
        {
            if (search == NULL)
            {
                search = args[i];
            }
            else
            {
                target_dir = args[i];
                if (target_dir[0] == '.' && target_dir[1] == '/')
                {
                    target_dir = target_dir + 2;
                }
            }
        }
    }

    if (file_bool == 1 && dir_bool == 0)
    {
        dir_flag = 0;
    }
    else if (file_bool == 0 && dir_bool == 1)
    {
        file_flag = 0;
    }
    else if (file_bool == 1 && dir_bool == 1)
    {
        printf("Invalid Flags!\n");
        return;
    }

    // printf("File Flag : %d\n", file_flag);
    // printf("Dir Flag : %d\n", dir_flag);
    // printf("Exact Flag : %d\n", exact_flag);
    seek_recursive(target_dir, search, file_flag, dir_flag, exact_flag, &count, exactMatch);

    // printf("Count : %d\n", count);
    if (count == 1 && (exact_flag == 1))
    {
        // printf("yoo\n");
        struct stat st;
        stat(exactMatch, &st);

        if (S_ISDIR(st.st_mode))
        {
            printf("\033[1;34m%s/\033[0m\n", exactMatch);
            change_directory(exactMatch);
        }
        else
        {
            printf("\033[1;32m%s\033[0m\n", exactMatch);
            FILE *fp = fopen(exactMatch, "r");
            if (fp == NULL)
            {
                printf("Missing permissions for task!\n");
                return;
            }

            char line[256];
            while (fgets(line, sizeof(line), fp))
            {
                printf("%s", line);
            }
            fclose(fp);
        }
    }
    else if (count == 0)
    {
        printf("No match found!\n");
    }

    return;
}
