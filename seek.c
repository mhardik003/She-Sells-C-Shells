#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void seek_recursive(const char *name, const char *search, int flags, int *count);

int main(int argc, char *argv[]) {
    int flags = 0;  // 1 for -d, 2 for -f, 4 for -e
    char *search = NULL;
    char *target_dir = ".";
    int count = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; j < strlen(argv[i]); j++) {
                switch (argv[i][j]) {
                    case 'd':
                        flags |= 1;
                        break;
                    case 'f':
                        flags |= 2;
                        break;
                    case 'e':
                        flags |= 4;
                        break;
                }
            }
        } else {
            if (search == NULL) {
                search = argv[i];
            } else {
                target_dir = argv[i];
            }
        }
    }

    if ((flags & 1) && (flags & 2)) {
        printf("Invalid flags!\n");
        return 1;
    }

    seek_recursive(target_dir, search, flags, &count);

    if (count == 0) {
        printf("No match found!\n");
    }

    return 0;
}

void seek_recursive(const char *name, const char *search, int flags, int *count) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            if (strcmp(entry->d_name, search) == 0 && (!(flags & 2))) {
                (*count)++;
                printf("\033[1;34m%s/\033[0m\n", path);
            }
            seek_recursive(path, search, flags, count);
        } else {
            if (strcmp(entry->d_name, search) == 0 && (!(flags & 1))) {
                (*count)++;
                printf("\033[1;32m%s/%s\033[0m\n", name, entry->d_name);
            }
        }
    }
    closedir(dir);
}
