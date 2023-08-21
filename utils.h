#ifndef UTILS_H
#define UTILS_H

void find_os(int *is_Linux, int *is_Windows);
void isLinux(int *is_Linux);
void isWindows(int *is_Windows);
void get_user_and_sys_name(char *user, char *system, char *pwd, int is_Linux, int is_Windows);
void print_prompt();

void input_handler(char *input);

// Function prototypes





#endif // #UTILS_H
