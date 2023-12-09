#include "../headers.h"

// Original terminal settings
struct termios orig_termios;

void die(const char *s)
{
    perror(s);
    exit(1);
}

// Restore terminal settings to original on exit
void disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

// Enable raw mode for terminal
void enable_raw_mode()
{

    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);

    
    // raw.c_cflag &= ~(CSIZE | PARENB);
    // raw.c_oflag &= ~(OPOST);
    // raw.c_iflag &= ~(IXON | ICRNL);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

// // Simulate getting the PID of the latest created process
// pid_t get_latest_pid() {
//     static pid_t pid = 11809; // Sample starting pid, increment it for simulation
//     return ++pid;
// }

int get_latest_pid()
{
    DIR *dir;
    struct dirent *entry;
    struct stat dir_stat;
    time_t latest_time = 0;
    int latest_pid = -1;

    if ((dir = opendir("/proc")) == NULL)
    {
        perror("Could not open /proc");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        int pid = atoi(entry->d_name); // Convert directory name to PID
        if (pid > 0)
        { // if directory name is a number (i.e., it's a process ID)
            char path[256];
            snprintf(path, sizeof(path), "/proc/%d", pid);

            // Get the directory creation (inode change) time
            if (stat(path, &dir_stat) == 0)
            {
                if (dir_stat.st_ctime > latest_time)
                {
                    latest_time = dir_stat.st_ctime;
                    latest_pid = pid;
                }
            }
        }
    }

    closedir(dir);

    return latest_pid;
}

void handle_neonate_command(int time_arg)
{
    enable_raw_mode();

    char c;
    while (1)
    {
        printf("%d\n", get_latest_pid());
        fflush(stdout);

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        struct timeval timeout;
        timeout.tv_sec = time_arg;
        timeout.tv_usec = 0;

        int sel = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

        if (sel > 0 && FD_ISSET(STDIN_FILENO, &read_fds))
        {
            read(STDIN_FILENO, &c, 1);
            if (c == 'x')
            {
                break;
            }
            else
            {
                ;
            }
        }
    }

    disable_raw_mode();
}

void neonate(int num_args, char *args[])
{

    if (num_args < 2 || strcmp(args[0], "-n") != 0)
    {
        printf("Usage : neonate -n <time>\n");
        return;
    }

    if (strchr(args[1], '-') == NULL && strchr(args[1], '.') == NULL)
    {

        int time_arg = atoi(args[1]);
        if (time_arg >= 0)
        {
            handle_neonate_command(time_arg);
        }
    }

    else
    {
        printf("neonate: Invalid time argument\n");
        return;
    }

    return;
}
