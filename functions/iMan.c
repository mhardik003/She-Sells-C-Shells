#include "../headers.h"

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
    char buffer[BUFFER_SIZE];
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
             "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
             command_name, HOST);

    if (send(sockfd, buffer, strlen(buffer), 0) == -1)
    {
        perror("Error sending GET request");
        exit(EXIT_FAILURE);
    }

    // Read the body of the website
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        // printf("%s", buffer);
        // TODO: Properly parse the HTML and extract the relevant sections.
    }

    char *end;
    end = strstr(buffer, "AUTHOR");
    if (end == NULL)
    {
        end = strstr(buffer, "SEE ALSO");
        if (end == NULL)
        {
            printf("Connection Error : Please try again \n");
        }
    }
    else
    {
        
        *end = '\0';

        int index = 0;
        int flag = 0;
        char new[1024];
        for (int i = 0; i < strlen(buffer); i++)
        {
            if (buffer[i] == 'N' && buffer[i + 1] == 'A' && buffer[i + 2] == 'M' && buffer[i + 3] == 'E')
            {
                if (flag == 0)
                {
                    flag = 1;
                    continue;
                }
                index = i;
                break;
            }
        }
        // printf("First occurence index : %d\n", index);
        strcpy(new, buffer + index);
        printf("%s", new);
    }

    // Close socket
    close(sockfd);
    freeaddrinfo(res);
    return;
}
