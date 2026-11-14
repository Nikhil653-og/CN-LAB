#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 7000

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[2048];
    fd_set readfds;

    // 2. Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 4. Connect to server
    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Connect failed");
        exit(1);
    }

    // 5. Display connected
    printf("Connected to chat server. Type messages...\n");

    // 7. Repeat continuously
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds); // keyboard = STDIN_FILENO = 0
        FD_SET(sockfd, &readfds);

        select(sockfd+1, &readfds, NULL, NULL, NULL);

        // 8. If keyboard is ready
        if(FD_ISSET(0, &readfds)){
            fgets(buffer, sizeof(buffer), stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }

        // 9. If client socket is ready
        if(FD_ISSET(sockfd, &readfds)){
            int valread = read(sockfd, buffer, sizeof(buffer)-1);
            if(valread <= 0){
                printf("Server disconnected\n");
                break;
            } else {
                buffer[valread] = '\0';
                printf("%s", buffer);
            }
        }
    }

    // 11. Close
    close(sockfd);
    return 0;
}