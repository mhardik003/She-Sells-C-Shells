#ifndef HEADERS_H_
#define HEADERS_H_


#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "prompt.h"

#define MAX_LEN 1024
#define clear() printf("\033[2J\033[H"); // Send control codes to clear the screen

#endif