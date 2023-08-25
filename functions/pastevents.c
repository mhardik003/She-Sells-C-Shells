#include "../headers.h"

void checkHistoryFile()
{
    // program to check the fie exists, if not then create it
    FILE *file = fopen(HISTORY_FILE, "r");

    if (file == NULL)
    {
        // File doesn't exist, create it
        file = fopen(HISTORY_FILE, "w");

        if (file == NULL)
        {
            printf("Error creating the file.\n");
        }

        // printf("File created successfully.\n");
        fclose(file);
    }
    else
    {
        // printf("File already exists.\n");
        fclose(file);
    }
}

char *read_lastLine()
{
    checkHistoryFile();

    // read the last line from the .history.txt file and store it in a string
    // return 0 if the file is empty
    // return if the file is not empty
    char *last_line = malloc(1000 * sizeof(char));
    FILE *fp = fopen(HISTORY_FILE, "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    int count = 0;
    while (fgets(last_line, 1000, fp) != NULL)
    {
        count++;
    }
    fclose(fp);

    // printf("%s", last_line);

    return last_line;
}

void addCommandToHistory(char *input)
{

    // check if the input is not empty by checking if has alpha numeric characters



    char *lastCommand = read_lastLine();

    lastCommand[strlen(lastCommand) - 1] = '\0';

    


    // If the last command is the same as the current command, don't add it to the history file
    // If the command is history or pastevents, don't add it to the history file
    if (strcmp(lastCommand, input) != 0 && (findWord("history", input) && (findWord("pastevents", input))))
    {

        checkHistoryFile();
        // count the number of lines in the .history.txt file
        // if the number of lines is greater than MAX_HISTORY_LEN, delete the first line

        int num_lines = 0;
        char *fileName = HISTORY_FILE;
        char tempFileName[1024] ;
         strcpy(tempFileName,HOME_DIR);
        strcat(tempFileName, "temp_file.txt");

        FILE *file = fopen(fileName, "r");
        if (file == NULL)
        {
            perror("Error opening input file");
            return;
        }

        // Count the number of lines in the input file
        int ch;
        while ((ch = fgetc(file)) != EOF)
        {
            if (ch == '\n')
            {
                num_lines++;
            }
        }
        // Count the last line if it doesn't end with a newline
        if (ftell(file) > 0)
        {
            num_lines++;
        }

        // Close the input file
        fclose(file);

        // printf("Number of lines in the file : %d \n", num_lines);
        // If line count is greater than maxLines, remove the first line
        if (num_lines > MAX_HISTORY_LENGTH)
        {
            file = fopen(fileName, "r");
            FILE *tempFile = fopen(tempFileName, "w");

            if (file == NULL || tempFile == NULL)
            {
                perror("Error opening files");
                return;
            }

            // Skip the first line in the input file
            int firstLineSkipped = 0;
            while ((ch = fgetc(file)) != EOF)
            {
                if (!firstLineSkipped)
                {
                    if (ch == '\n')
                    {
                        firstLineSkipped = 1;
                    }
                }
                else
                {
                    fputc(ch, tempFile);
                }
            }

            fclose(file);
            fclose(tempFile);

            // Replace the original file with the temporary file
            remove(fileName);
            rename(tempFileName, fileName);

            // printf("First line removed.\n");
        }
        else
        {
            // printf("No changes needed.\n");
        }

        // append the input to the .history.txt file
        FILE *fp;
        fp = fopen(HISTORY_FILE, "a");
        fprintf(fp, "%s\n", input);
        fclose(fp);
    }
}

void execute_pastevent(int command_number)
{
    checkHistoryFile();
    // printf("hiiii");

    // printf("Executing the command number %d\n", command_number);

    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Count the total number of lines in the file
    int totalLines = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            totalLines++;
        }
    }

    // printf("Total lines : %d\n", totalLines);

    // Validate the desired line number
    if (command_number <= 0 || command_number > totalLines)
    {
        printf("Invalid line number\n");
        fclose(file);
        return;
    }

    // Move the file pointer to the beginning of the desired line from the bottom
    fseek(file, 0, SEEK_SET);
    int targetLine = totalLines - command_number + 1;
    for (int j = 1; j < targetLine; j++)
    {
        while ((c = fgetc(file)) != EOF && c != '\n')
            ;
    }

    

    // Read and print the desired line
    char line[256];
    if (fgets(line, sizeof(line), file) != NULL)
    {
        // printf("Line %d from the bottom: %s", command_number, line);

        // Remove the newline character from the end of the line
        line[strlen(line) - 1] = '\0';
        input_handler(line);
    }

    fclose(file);
}

void pastevents_purge()
{
    checkHistoryFile();

    // delete the contents of the .history.txt file
    // printf("Clearing the history\n");
    FILE *fp = fopen(HISTORY_FILE, "w");
    fclose(fp);
}

void pastevents()
{
    checkHistoryFile();

    // read the contents of the .history.txt file and print them on the screen
    
    char buffer[256]; // Buffer to store each line

    // Open the file for reading
    FILE *file;
    file = fopen(HISTORY_FILE, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Read and print each line from the file
    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("%s", buffer);
    }

    // Close the file
    fclose(file);
    return;
}

void pastevents_driver(int arg_count, char *args[])
{
    if (arg_count == 0)
    {
        pastevents();
    }
    else if (arg_count == 1)
    {
        if (strcmp(args[0], "purge") == 0)
        {
            pastevents_purge();
        }
        else
        {
            printf("ERROR : Invalid argument\n");
        }
    }
    else if (arg_count == 2)
    {
        if (atoi(args[1]) != 0)
        {
            execute_pastevent(atoi(args[1]));
        }
        else
        {
            printf("ERROR : Invalid PID\n");
        }
    }
}