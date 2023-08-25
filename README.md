# Description
Bash shell implemented in C with the following functionlaties
* Displays the current user and hostname of the system


* Handle the input while taking care of the edge cases


* **warp** : It changes the directory that the shell is currently in along with printing the full path of working directory after changing. 
    * Handles the case of changing to a directory that does not exist
    * Handles the case of  “.”, “..”, “~”, and “-” flags.
    * If more than one argument is present, execute warp sequentially   with all of them being the argument one by one (from left to right).
    * If no argument is present, then warp into the home directory.

<br>

* **peek** : It lists all the files and directories in the specified directories in lexicographic order
    * Has the option to take the -a and -l flags as normal bash ls command

<br>

* **pastevents** : Similar to the actual history command in bash
    * **pastevents purge** : Clears all the pastevents currently stored
    * **pastevents execute \<index\>** : Execute the command at position in pastevents (ordered from most recent to oldest).  
    * If no arguement is present, then print all the pastevents currently stored in the shell (upto 15 commands are stored in history file at once)

<br>

* **system commands** : The shell is able to execute the other system commands present in Bash as well like emacs, gedit etc. 
    * It is possible in both foreground and background processes by using the **&** symbol at the end of the command.

<br>

* **proclore** : It is used to obtain information regarding a process.
    * If an argument is missing, print the information of your shell. 

<br>

* **seek** : It looks for a file/directory in the specified target directory (or current if no directory is specified) and returns a list of relative paths (from target directory) of all matching files directories
    * Supports the following flags
        * -d : Only look for directories (ignore files even if name matches)
        * -f : Only look for files (ignore directories even if name matches)
        * -e : This flag is effective only when a single file or a single directory with the name is found. 
        If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it. Otherwise, the flag has no effect. This flag should work with -d and -f flags. If -e flag is enabled but the directory does not have access permission (execute) or file does not have read permission, then output “Missing permissions for task!”


        ```
        seek -d -e <directory> <name>
        ```


<br>

---

## Assumptions

- None 
- If doubtful, I have tried to mimic the functionality of bash shell

<Br>

---
## Program Flow

* The [main.c](main.c) file is the main file which contains 
    * the main function 
    * the loop which runs the shell
    * functions like init and exit code for the shell
    * signal handlers for Ctrl+C
    * the loop in the function keeps on printing the prompt (which is made in [prompt.c](prompt.c)), takes the input from the user and sends it to [input_handler.c](input_handler.c) for processing

<br>

* The [input_handler.c](input_handler.c) file contains the function which takes the input from the user and processes it
    * In the first function it takes the input, send it to [pastevents.c](pastevents.c) to add the command to the history file if the conditions are met and then parses it according to '&' and ';'.
    * After parsing it by '&' and ';', it sends the commands to another function in the file, which handles the individual commands in the input by seperating them by spaces and tabs, gettings the arguements for the functions and then sending it to the [function_handler.c]
    (function_handler.c) file for further processing

<br>

* The [function_handler.c](function_handler.c) file contains the function which handles the individual commands in the input by seperating them by spaces and tabs, gettings the arguements for the functions and then sending it to the respective functions in the [functions/](functions/) folder for further processing.
    * If no function is found, it sends it to the [syscall.c](syscall.c) file to execute the system commands by using execvp() function

<br>

* The [syscall.c](syscall.c) file contains the function which executes the system commands by using execvp() function
    * It also handles the case of '&' and ';' by using fork() and waitpid() functions

<br>

* The [functions/](functions/) folder contains the functions for the commands like warp, peek, pastevents, proclore and seek
    * Each function has its own file and the functions are called from the [function_handler.c](function_handler.c) file


<br>

---

<Br>

### **Disclaimer**

AI models like ChatGPT and Co-Pilot have been used to code the shell, but I have the full understading of how the shell works and can explain the code in detail.
