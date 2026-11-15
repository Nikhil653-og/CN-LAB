#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7000

int main(){
    struct sockaddr_in server_address, client_address;
    char message[2000];
    char buffer[2000];
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sockfd < 0){
        printf("\n socket creation failed");
        return 1;
    }
    
    printf("Listening...\n");
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if(bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    socklen_t client_len = sizeof(client_address);
    recvfrom(sockfd, message, sizeof(message)-1, 0, (struct sockaddr*)&client_address, &client_len);
    message[sizeof(message)-1] = '\0';
    
    printf("Received: %s\n", message);
    buffer[0] = '\0';
    char delimiter[] = " ";
    char* word = strtok(message, delimiter);
    
    while(word != NULL){
        if(strncmp(word, "tbh", 3) == 0){
            strcat(buffer, "to be honest ");
        }
        else if(strncmp(word, "ig", 2) == 0){
            strcat(buffer, "i guess ");
        }
        else if(strncmp(word, "tbf", 3) == 0){
            strcat(buffer, "to be fair ");
        }
        else if(strncmp(word, "atm", 3) == 0){
            strcat(buffer, "at the moment ");
        }
        else if(strncmp(word, "irl", 3) == 0){
            strcat(buffer, "in real life ");
        }
        else if(strncmp(word, "lol", 3) == 0){
            strcat(buffer, "laughing out loud ");
        }
        else if(strncmp(word, "asap", 4) == 0){
            strcat(buffer, "as soon as possible ");
        }
        else if(strncmp(word, "omg", 3) == 0){
            strcat(buffer, "oh my god ");
        }
        else if(strncmp(word, "ttyl", 4) == 0){
            strcat(buffer, "talking to you later ");
        }
        else if(strncmp(word, "idk", 3) == 0){
            strcat(buffer, "I don't know ");
        }
        else if(strncmp(word, "nvm", 2) == 0){
            strcat(buffer, "never mind ");
        }
        else if(strncmp(word, "idc", 3) == 0){
            strcat(buffer, "i don't care ");
        }
        else {
            strcat(buffer, word);
            strcat(buffer, " ");
        }
        word = strtok(NULL, delimiter);
    }
    
    printf("Translated: %s\n", buffer);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_address, client_len);
    close(sockfd);
    return 0;
}