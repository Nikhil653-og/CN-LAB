#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7000

int main(){
    struct sockaddr_in server_address;
    char message[2000];
    char buffer[2000];
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sock < 0){
        printf("\n Socket Creation Failed\n");
        return 1;
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    
    printf("Enter the message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';
    
    sendto(sock, message, strlen(message), 0, (struct sockaddr*)&server_address, sizeof(server_address));
    
    int n = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&server_address, &(socklen_t){sizeof(server_address)});
    buffer[n] = '\0';
    
    printf("Translated message: %s\n", buffer);
    close(sock);
    return 0;
}