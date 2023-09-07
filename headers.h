#ifndef HEADERS_H_
#define HEADERS_H_

#include "global_vars.h"
#include "utils.h"

#define MAX_LEN 1024
#define LEN_PWD 1024
#define MAX_HISTORY_LENGTH 15
#define TEMP_BG_OUTPUT_FILE "/tmp/bg_output_all"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>


#include <dirent.h> // for ls -al
#include <time.h>
#include <pwd.h>
#include <grp.h>

#endif