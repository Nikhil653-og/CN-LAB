#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 7000
#define MAX_CLIENTS 30

int main() {
    int server_fd, new_socket, client_socket[MAX_CLIENTS], client_id[MAX_CLIENTS];
    int max_clients = MAX_CLIENTS;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[2048];
    fd_set readfds;
    int max_sd, sd, activity, next_id = 1;

    // 6. Initialize client socket array to 0
    for(int i=0; i<max_clients; i++){
        client_socket[i] = 0;
        client_id[i] = 0;
    }

    // 2. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 4. Bind
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 5. Listen
    listen(server_fd, 5);
    printf("Chat Server listening on port %d...\n", PORT);

    // 8. Repeat continuously
    while(1) {
        // 8a. Clear the set
        FD_ZERO(&readfds);

        // 8b. Add server socket
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // 8c. Add all connected client sockets
        for(int i=0; i<max_clients; i++){
            sd = client_socket[i];
            if(sd > 0) FD_SET(sd, &readfds);
            if(sd > max_sd) max_sd = sd;
        }

        // 8d. Call select() to wait
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        // 9. If server socket is ready -> new connection
        if(FD_ISSET(server_fd, &readfds)){
            new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
            printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Find empty slot
            for(int i=0; i<max_clients; i++){
                if(client_socket[i] == 0){
                    client_socket[i] = new_socket;
                    client_id[i] = next_id++;
                    printf("Client %d connected, socket fd %d\n", client_id[i], new_socket);

                    // Send client ID to new client
                    char id_msg[100];
                    sprintf(id_msg, "You are Client %d\n", client_id[i]);
                    send(new_socket, id_msg, strlen(id_msg), 0);
                    break;
                }
            }
        }

        // 10. For every connected client check if ready
        for(int i=0; i<max_clients; i++){
            sd = client_socket[i];
            if(sd > 0 && FD_ISSET(sd, &readfds)){
                memset(buffer, 0, sizeof(buffer));
                int valread = read(sd, buffer, sizeof(buffer)-1);

                // 11. If read() returns 0 -> disconnected
                if(valread <= 0){
                    printf("Client %d disconnected\n", client_id[i]);
                    close(sd);
                    client_socket[i] = 0;
                    client_id[i] = 0;
                } else {
                    // 12. Otherwise display and broadcast
                    buffer[valread] = '\0';
                    printf("Client %d: %s", client_id[i], buffer);

                    char broadcast[2200];
                    sprintf(broadcast, "Client %d: %s", client_id[i], buffer);

                    // Send to all other clients
                    for(int j=0; j<max_clients; j++){
                        if(client_socket[j]!= 0 && client_socket[j]!= sd){
                            send(client_socket[j], broadcast, strlen(broadcast), 0);
                        }
                    }
                }
            }
        }
    }
    // 14. Close
    close(server_fd);
    return 0;
}
