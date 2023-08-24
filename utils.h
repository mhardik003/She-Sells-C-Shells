#ifndef UTILS_H
#define UTILS_H

#define MAX_LEN 1024
#define LEN_PWD 1024
#define HISTORY_FILE ".history.txt"

#define clear() printf("\033[2J\033[H"); // Send control codes to clear the screen

// init.c
void find_os(int *is_Linux, int *is_Windows);
void get_user_and_sys_name(char *user, char *system, int is_Linux, int is_Windows);
void get_pwd(char *pwd_name);

//  main.c
void handler(int sig);
void init_shell();
void exit_shell();

// prompt.c
void replaceSubstring(char *str, const char *old, const char *new_str);
void print_pwd();
void print_prompt();

// background.c
void check_bg_processes();

// input_handler.c
int findWord(char *word, char *line);
void trimString(char *str);
void remove_first_element_from_array(char *args[], int num_args); // since the first word in the command is the function itslef but we just want the arguements in the args array
void execute_command(char *input, int bg);
void input_handler(char *input);

// function_handler.c
void function_handler(char *function_name, char *args[], int num_args, int is_bg);

// warp.c
char *checkSlashes(char *path);
int isValidDirectory(char *path);
char *get_prev_directory_string(char *path);
void change_directory(char *path);
void warp(char *args[], int num_args);

// peek.c
char *get_path(char *path);
int is_directory(const char *path);
void list_directory(const char *path, int show_all, int long_format);
void peek(char *args[], int num_args);

// pastevents.c
void checkHistoryFile();
char *read_lastLine();
void addCommandToHistory(char *input);
void execute_pastevent(int command_number);
void pastevents_purge();
void pastevents();
void pastevents_driver(int arg_count, char *args[]);

// proclore.c
void print_virtualMemUsage(int pid);
void print_Process_info(int pid);
void proclore_driver(int num_args, char *argv[]);

// seek.c
void seek_recursive(const char *name, const char *search, int file_flag, int dir_flag, int exact_flag, int *count, char *exactMatch);
void seek(int num_args, char *args[]);

#endif // #UTILS_H
