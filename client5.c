//Client 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    char buffer[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    // Server details
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed\n");
        return -1;
    }

    printf("Connected to server.\n\n");

    while (1)
    {
        // Send message
        printf("Enter the message for server: ");
        fgets(message, sizeof(message), stdin);

        send(sock, message, strlen(message), 0);
        printf("Message sent to server\n");

        if (strncmp(message, "quit", 4) == 0)
            break;

        // Receive reply
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));

        printf("Server: %s\n", buffer);

        if (strncmp(buffer, "quit", 4) == 0)
            break;
    }

    close(sock);

    return 0;
}
//Client 5 end
