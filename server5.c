//server 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000

int main()
{
    int server_sockfd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[1024];
    char message[1024];

    // Create socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        printf("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Bind failed\n");
        exit(EXIT_FAILURE);
    }

    // Listen for client
    if (listen(server_sockfd, 3) < 0)
    {
        printf("Listen failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");

    // Accept client
    new_socket = accept(server_sockfd,
                        (struct sockaddr *)&address,
                        &addrlen);

    if (new_socket < 0)
    {
        printf("Accept failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n\n");

    while (1)
    {
        // Receive message
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, sizeof(buffer));

        printf("Client: %s\n", buffer);

        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // Send reply
        printf("Enter the message for client: ");
        fgets(message, sizeof(message), stdin);

        send(new_socket, message, strlen(message), 0);
        printf("Message sent to client\n");

        if (strncmp(message, "quit", 4) == 0)
            break;
    }

    close(new_socket);
    close(server_sockfd);

    return 0;
}
//server 5 end
