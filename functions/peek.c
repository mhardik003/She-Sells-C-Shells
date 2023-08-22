#include "../headers.h"

// Comparator function for qsort
int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int is_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return 0;
}

void list_directory(const char *path, int show_all, int long_format)
{
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    char filepath[1024];
    char *entries[2048]; // assuming max 2048 entries in a directory for simplicity
    int count = 0;

    d = opendir(path);
    if (d == NULL)
    {
        perror("Unable to read directory");
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

void warp(int argc, char **argv)
{
    int show_all = 0;
    int long_format = 0;
    const char *path = ".";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            show_all = 1;
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            long_format = 1;
        }
        else
        {
            path = argv[i];
        }
    }

    list_directory(path, show_all, long_format);

}
