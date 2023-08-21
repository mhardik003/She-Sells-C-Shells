#ifndef UTILS_H
#define UTILS_H

#define MAX_LEN 1024
#define LEN_PWD 1024

void init_shell();

void find_os(int *is_Linux, int *is_Windows);
void isLinux(int *is_Linux);
void isWindows(int *is_Windows);
void get_user_and_sys_name(char *user, char *system, int is_Linux, int is_Windows);
void get_pwd(char *pwd);
void print_prompt();

void input_handler(char *input);

void function_handler(char *function_name, char *args[], int num_args);

// Function prototypes
void warp(char *args[]);
void peek();
void peek_a();
void peek_l();
void peek_al();
void pastevents();
void proclore_shell();
void proclore(int pid);

#endif // #UTILS_H
