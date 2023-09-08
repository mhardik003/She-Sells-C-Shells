#include "../headers.h"

int check_unavailable(char *str)
{
    if (strstr(str, "No matches for "))
    {
        return 1;
    }
    return 0;
}

int check_ConnectionError(char *str)
{
    if (strstr(str, "NAME"))
    {
        return 0;
    }
    return 1;
}

void fetch_iMan_page(int num_args, char **args)
{
    if (num_args != 1)
    {
        fprintf(stderr, "Usage: %s <command_name>\n", args[0]);
        return;
    }

    char command_name[1024];
    strcpy(command_name, args[0]);

    struct addrinfo hints, *res;
    int sockfd;
    char buffer[16384];
    int bytes_received;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    // DNS resolution
    if (getaddrinfo(HOST, "http", &hints, &res) != 0)
    {
        perror("Error resolving DNS");
        exit(EXIT_FAILURE);
    }

    // Open a TCP socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Send GET request
    snprintf(buffer, sizeof(buffer),
             "GET /man1/%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
             command_name, HOST);

    if (send(sockfd, buffer, strlen(buffer), 0) == -1)
    {
        perror("Error sending GET request");
        close(sockfd);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    char html[16384];
    // Read the body of the website
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        // printf("%s", buffer);
        strcat(html, buffer);
        // TODO: Properly parse the HTML and extract the relevant sections.
    }

    // check if the page exists or not by checking if the string contains an alphanumeric character
    if (check_unavailable(html))
    {
        printf("ERROR : No such command\n");
        close(sockfd);
        freeaddrinfo(res);
        return;
    }

    if (check_ConnectionError(html))
    {
        // printf("%s", html);
        printf("Connection Error : Please try again \n");
        close(sockfd);
        freeaddrinfo(res);
        return;
    }

    // printf("\n\n\n\n\n=============================================================================================================================\n\n\n\n\n");

    // remove all the lines which have < and > tags in them and print the rest
    int check_htmlTag1 = 0;
    int check_htmlTag2 = 0;
    char temp_Line[512];
    char final_string[16384];
    for (int i = 0; i < strlen(html); i++)
    {
        // add the character to temp_Line
        temp_Line[strlen(temp_Line)] = html[i];
        if (html[i] == '<')
        {
            check_htmlTag1 = 1;
            // printf(" The line %s has a < tag \n", temp_Line);
        }
        if (html[i] == '>')
        {
            check_htmlTag2 = 1;
            // printf(" The line %s has a > tag \n", temp_Line);
        }
        if (html[i] == '\n' || i == strlen(html) - 1)
        {
            if (strstr(temp_Line, "</html>"))
            {
                break;
            }
            if ((check_htmlTag1 && check_htmlTag2) || strstr(temp_Line, "HTTP") || strstr(temp_Line, "Date") || strstr(temp_Line, "Server") || strstr(temp_Line, "Last-Modified") || strstr(temp_Line, "Content-Length") || strstr(temp_Line, "Content-Type"))
            {
                // printf("> The line %s has a < and > tag \n", temp_Line);
                ;
            }
            else
            {
                // printf("> The line %s does not have a < and > tag \n", temp_Line);
                strcat(final_string, temp_Line);
            }
            check_htmlTag1 = 0;
            check_htmlTag2 = 0;
            // printf("meow");
            memset(temp_Line, 0, sizeof(temp_Line));
        }
    }

    // printf("\n\n\n\n\n=============================================================================================================================\n\n\n\n\n");
    printf("%s", final_string);

    // char *end;
    // end = strstr(html, "AUTHOR");
    // if (end == NULL)
    // {
    //     end = strstr(html, "SEE ALSO");
    //     if (end == NULL)
    //     {
    //         printf("Connection Error : Please try again \n");
    //     }
    // }
    // else
    // {

    //     *end = '\0';

    //     int index = 0;
    //     int flag = 0;
    //     char new[1024];
    //     for (int i = 0; i < strlen(html); i++)
    //     {
    //         if (html[i] == 'N' && html[i + 1] == 'A' && html[i + 2] == 'M' && html[i + 3] == 'E')
    //         {
    //             if (flag == 0)
    //             {
    //                 flag = 1;
    //                 continue;
    //             }
    //             index = i;
    //             break;
    //         }
    //     }
    //     // printf("First occurence index : %d\n", index);
    //     strcpy(new, html + index);
    //     printf("%s", new);
    // }

    // Close socket
    close(sockfd);
    freeaddrinfo(res);
    return;
}
