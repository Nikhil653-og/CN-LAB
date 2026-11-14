//server 9
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000
int main() {
    int sockfd, newsock;
    char buffer[2000];
    char filename[20];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("Bind");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 3);
    printf("Server listening...\n");
     while(1){
		newsock = accept(sockfd, (struct sockaddr *)&address,(socklen_t *)&addrlen);
		if(newsock<0){
			perror("Sock Error\n");
			continue;
		}
		if(fork() == 0){
			close(sockfd);
			int n = read(newsock,filename,sizeof(filename)-1);
			filename[n]='\0';
			printf("Client Requested : %s\n",filename);
			pid_t pid = getpid();
			FILE *fp = fopen(filename,"r");
			if(fp == NULL){
				sprintf(buffer," process id %d not found %s",pid,filename);
				write(newsock,buffer,strlen(buffer));
			}
			else{
				sprintf(buffer,"process id %d found %s",pid,filename);
				write(newsock,buffer,strlen(buffer));
				while(fgets(buffer,sizeof(buffer),fp)!=NULL){
					write(newsock,buffer,strlen(buffer));
				}
				fclose(fp);
			}
			close(newsock);
			exit(0);
		}
		close(newsock);
	}
	close(sockfd);
	return 0;
}
//server 9 end
