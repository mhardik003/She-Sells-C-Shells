#ifndef UTILS_H
#define UTILS_H

#define MAX_LEN 1024
#define LEN_PWD 1024
#define HISTORY_FILE ".history.txt"

void init_shell();
void find_os(int *is_Linux, int *is_Windows);
void isLinux(int *is_Linux);
void isWindows(int *is_Windows);
void get_user_and_sys_name(char *user, char *system, int is_Linux, int is_Windows);
void get_pwd(char *pwd);
void print_prompt();
void check_background_processes();

void input_handler(char *input);

void function_handler(char *function_name, char *args[], int num_args);



// Function prototypes
void peek();
void peek_a();
void peek_l();
void peek_al();
void pastevents();




// INPUT HANDLER
void input_handler(char *input);
void single_input_handler(char *input, int bg);
void remove_first_element_from_array(char *args[], int num_args); // since the first word in the command is the function itslef but we just want the arguements in the args array
int findWord(char *word, char *line);
void trimString(char *str);
void handler(int sig_num);



// WARP
void warp(char *args[], int num_args);
char* checkSlashes( char *path);
int isValidDirectory( char *path);
char *get_prev_directory_string(char *path);
void change_directory(char *path);


// PEEK
char *get_path(char *path);
int is_directory(const char *path);
void list_directory(const char *path, int show_all, int long_format);
void peek(char *args[], int num_args);



// PASTEVENTS
void pastevents();
void addLineToHistory(char *input);
char *read_lastLine();
void pastevents();
void pastevents_purge();
void execute_pastevent(int command_number);
void checkHistoryFile();



// PROCLORE
void proclore(int num_args, char *argv[]);


// SEEK
void seek(int num_args, char *args[]);


// ECHO
void echo(int num_args , char *args[] );




char *read_lastLine();

#endif // #UTILS_H
