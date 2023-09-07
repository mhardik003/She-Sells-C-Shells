#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define HOST "man.he.net"
#define PORT 80
#define BUFFER_SIZE 4096

void fetch_man_page(const char *command_name)
{
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
        // TODO: Properly parse the HTML and extract the relevant sections.
    }
    char *end;
    end = strstr(buffer, "AUTHOR");
    if (end == NULL)
    {
        printf("Connection Error : Please try again \n");
    }
    else
    {
        *end = '\0';

        printf("%s", buffer);
    }
    // Handle the situation when the man page is not found
    if (strstr(buffer, "<title>404 Not Found</title>") != NULL)
    {
        printf("ERROR\n\tNo such command\n");
    }

    // Close socket
    close(sockfd);
    freeaddrinfo(res);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <command_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fetch_man_page(argv[1]);

    return EXIT_SUCCESS;
}
