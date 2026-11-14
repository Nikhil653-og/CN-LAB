//Client 10
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define buffer_size 1024
#define PORT 7000
int main() {
	int server_fd = 0;
	struct sockaddr_in serv_addr;
	fd_set readfds;
	char message[buffer_size];
	char buffer[buffer_size] = {0};
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd< 0) {
		printf("\nSocket creation error\n");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(server_fd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed\n");
		return -1;
	}
	while(1){
		FD_ZERO(&readfds);
		FD_SET(server_fd,&readfds);
		FD_SET(STDIN_FILENO,&readfds);
		select(server_fd+1,&readfds,NULL,NULL,NULL);
		if(FD_ISSET(STDIN_FILENO,&readfds)){
			fgets(message,sizeof(message),stdin);
			if(strncmp(message,"exit",4)==0){
				printf("Disconnected\n");
				break;
			}
			send(server_fd,message,strlen(message),0);
		}
		if(FD_ISSET(server_fd,&readfds)){
			int n = read(server_fd, buffer, buffer_size);
			buffer[n] = '\0';
			printf("%s",buffer);
		}
	}
	close(server_fd);
}
//Client 10 end

