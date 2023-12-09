# C - Shell : A Bash Shell Emulator in C

This project presents a C implementation of the Bash shell, designed to emulate the functionality of Bash in Linux environments.

For more details, refer to the [Specifications PDF ](./Specifications.pdf)

<br>

## Functionalities

- **Display Information** : Displays the current user and hostname of the system

- **Input Handling** : Handle the input while taking care of the edge cases

- **warp** : Changes the directory that the shell is currently in along with printing the full path of working directory after changing.
  - Handles non-existent directory cases
  - Supports ".", "..", "~", and "-" flags.
  - If multiple arguments are given, executes warp sequentially for each.
  - Defaults to the home directory if no argument is provided.

<br>

- **peek** : Lists all the files and directories in the specified directories in lexicographic order
  - Supports "-a" and "-l" flags, akin to the bash ls command.

<br>

- **pastevents** : Similar to the actual history command in bash
  - **pastevents purge** : Clears the command history.
  - **pastevents execute \<index\>** : Executes a specific command from history
  - Displays all stored commands (up to 15) if no argument is given

<br>

- **system commands** : The shell is capable of executing other Bash system commands like emacs, gedit etc.
  - Supports foreground and background processing with the "&" symbol.

<br>

- **proclore** : Retrieves information about processes.
  - If no argument is specified, it displays information about the shell process.

<br>

- **seek** : It looks for a file/directory in the specified target directory (or current if no directory is specified) and returns a list of relative paths (from target directory) of all matching files directories

  - Supports the following flags

    - -d : Only searches directories
    - -f : Only searches files
    - -e : Effective when exactly one file or directory is found. Prints the file output or changes the directory as appropriate. Requires appropriate permissions. If permissions are lacking, outputs "Missing permissions for task!"

    Example usage:

    ```
    seek -d -e <directory> <name>
    ```

<br>

## Program Structure

- The [main.c](main.c) file is the main file which contains
  - the main function
  - the loop which runs the shell
  - functions like init and exit code for the shell
  - signal handlers for Ctrl+C
  - the loop in the function keeps on printing the prompt (which is made in [prompt.c](prompt.c)), takes the input from the user and sends it to [input_handler.c](input_handler.c) for processing

<br>

- The [input_handler.c](input_handler.c) file contains the function which takes the input from the user and processes it
  - In the first function it takes the input, send it to [pastevents.c](pastevents.c) to add the command to the history file if the conditions are met and then parses it according to '&' and ';'.
  - After parsing it by '&' and ';', it sends the commands to another function in the file, which handles the individual commands in the input by seperating them by spaces and tabs, gettings the arguements for the functions and then sending it to the [function_handler.c]
    (function_handler.c) file for further processing

<br>

- The [function_handler.c](function_handler.c) file contains the function which handles the individual commands in the input by seperating them by spaces and tabs, gettings the arguements for the functions and then sending it to the respective functions in the [functions/](functions/) folder for further processing.
  - If no function is found, it sends it to the [syscall.c](syscall.c) file to execute the system commands by using execvp() function

<br>

- The [syscall.c](syscall.c) file contains the function which executes the system commands by using execvp() function
  - It also handles the case of '&' and ';' by using fork() and waitpid() functions

<br>

- The [functions/](functions/) folder contains the functions for the commands like warp, peek, pastevents, proclore and seek
  - Each function has its own file and the functions are called from the [function_handler.c](function_handler.c) file

<br>

## Assumptions

The implementation aims to closely mimic Bash functionality. In ambiguous situations, Bash behavior is the reference standard.

<br>

**Feel free to contribute to the project.**
