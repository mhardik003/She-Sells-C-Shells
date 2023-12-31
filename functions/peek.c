#include "../headers.h"

char *get_path(char *path)
{
    /*
    Return the absolute path of the directory to be changed to
    */

    if (strcmp(path, ".") == 0)
    {
        // DO NOTHING
        return CURR_PWD;
    }
    else if (strcmp(path, "..") == 0)
    {
        // Go to previous directory
        char *temp = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp, CURR_PWD);
        if (isValidDirectory(get_prev_directory_string(temp)))
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
        return temp;
    }
    else if (strcmp(path, "~") == 0)
    {
        // Go to home directory
        return HOME_DIR;
    }
    else if (strcmp(path, "-") == 0)
    {
        // Go to previous directory
        return PREV_PWD;
    }
    else if (path[0] == '/')
    {

        return path;
    }
    else
    {
        // Go to the specified directory
        char *temp_dir = (char *)malloc(LEN_PWD * sizeof(char));
        strcpy(temp_dir, CURR_PWD);
        strcat(temp_dir, "/");
        strcat(temp_dir, path);

        return temp_dir;
    }
}

// Comparator function for qsort
int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int is_directory(const char *path)
{
    /*
    Return 1 if the path is a directory, 0 otherwise
    */

    struct stat st;
    if (stat(path, &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return 0;
}

void list_directory(const char *path, int show_all, int long_format)
{
    /*
    List the contents of the directory specified by path
    */

    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    char filepath[1024];
    char *entries[2048]; // assuming max 2048 entries in a directory for simplicity
    int count = 0;

    d = opendir(path);
    if (d == NULL)
    {
        perror("peek ");
        return;
    }

    while ((dir = readdir(d)) != NULL)
    {
        if (!show_all && dir->d_name[0] == '.')
        {
            continue;
        }
        entries[count] = strdup(dir->d_name);
        count++;
    }

    qsort(entries, count, sizeof(char *), compare);

    for (int i = 0; i < count; i++)
    {
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entries[i]);
        if (stat(filepath, &file_stat) == -1)
        {
            perror("stat");
            continue;
        }

        if (long_format)
        {
            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

            printf(" %ld", (long)file_stat.st_nlink);

            struct passwd *pw = getpwuid(file_stat.st_uid);
            struct group *gr = getgrgid(file_stat.st_gid);
            if (pw)
            {
                printf(" %s", pw->pw_name);
            }
            else
            {
                printf(" %d", file_stat.st_uid);
            }
            if (gr)
            {
                printf(" %s", gr->gr_name);
            }
            else
            {
                printf(" %d", file_stat.st_gid);
            }

            printf(" %5ld", (long)file_stat.st_size);

            char mtime[80];
            strftime(mtime, sizeof(mtime), "%b %d %H:%M", localtime(&file_stat.st_mtime));
            printf(" %s ", mtime);
            if (is_directory(filepath))
            {
                printf(" \033[34m%s/\033[0m\n", entries[i]); // Blue for directory
            }
            else if (file_stat.st_mode & S_IXUSR)
            {
                printf(" \033[32m%s*\033[0m\n", entries[i]); // Green for executable
            }
            else
            {
                printf(" %s\n", entries[i]);
            }
        }
        else
        {
            if (is_directory(filepath))
            {
                printf(" \033[34m%s/\033[0m\n", entries[i]); // Blue for directory
            }
            else if (file_stat.st_mode & S_IXUSR)
            {
                printf(" \033[32m%s*\033[0m\n", entries[i]); // Green for executable
            }
            else
            {
                printf(" %s\n", entries[i]);
            }
        }

        free(entries[i]);
    }

    closedir(d);
    if (!long_format)
    {
        printf("\n");
    }
}

void peek(char *args[], int num_args)
{
    int show_all = 0;
    int long_format = 0;
    char *path = (char *)malloc(1024 * sizeof(char));
    strcpy(path, CURR_PWD);

    // find the path and the flags
    for (int i = 0; i < num_args; i++)
    {
        // printf("i : %d", i);
        if (args[i][0] != '-' || (args[i][0] == '-' && args[i][1] == '\0'))
        {
            char *temp_path = (char *)malloc(1024 * sizeof(char));
            strcpy(path, get_path(args[i]));
        }
        else if (strcmp(args[i], "-a") == 0)
        {
            show_all = 1;
        }
        else if (strcmp(args[i], "-l") == 0)
        {
            long_format = 1;
        }
        else if (strcmp(args[i], "-al") == 0 || strcmp(args[i], "-la") == 0)
        {
            show_all = 1;
            long_format = 1;
        }
    }
    list_directory(path, show_all, long_format);
}
