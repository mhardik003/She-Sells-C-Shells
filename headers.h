#ifndef HEADERS_H_
#define HEADERS_H_

#include "global_vars.h"
#include "utils.h"

#define MAX_LEN 1024
#define LEN_PWD 1024
#define MAX_HISTORY_LENGTH 15


#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include <ctype.h>


#define clear() printf("\033[2J\033[H"); // Send control codes to clear the screen

#endif