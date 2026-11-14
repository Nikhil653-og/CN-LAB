//server 10
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 7000
#define MAX_CLIENTS 3
#define buffer_size 1024
int main () {
	int server_fd, new_socket,client_count=0;
	fd_set readfds;
	int client_socket[MAX_CLIENTS],client_id[MAX_CLIENTS];
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[buffer_size] = {0};
	char message[buffer_size];
	for(int i = 0;i<MAX_CLIENTS;i++){
		client_socket[i]=0;
		client_id[i]=0;
	}
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		perror ("Socket failed");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT); 
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 5) < 0) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}
	printf("Server is listening and can accept Multiple Clients...\n");
	while(1){
		FD_ZERO(&readfds);
		FD_SET(server_fd,&readfds);
		int maxfd = server_fd;
		for(int i = 0;i < MAX_CLIENTS;i++){
			if(client_socket[i]>0){
				FD_SET(client_socket[i],&readfds);
				if(client_socket[i]>maxfd){
					maxfd = client_socket[i];
				}	
			}
		}
		select(maxfd+1,&readfds,NULL,NULL,NULL);
		if(FD_ISSET(server_fd,&readfds)){
			new_socket = accept(server_fd,(struct sockaddr*)&address, (socklen_t*)&addrlen);
			if (new_socket < 0) {
				perror("Error in accepting the client");
				continue;
			}
			for(int i = 0;i<MAX_CLIENTS;i++){
				if(client_socket[i] == 0){
					client_socket[i] = new_socket;
					client_count++;
					client_id[i] = client_count;
					printf("Client %d connected \n",client_id[i]);
					char welcome[buffer_size];
					sprintf(welcome,"You are Client %d\n",client_id[i]);
					send(new_socket,welcome,strlen(welcome),0);
					break;
				}
				if(i == MAX_CLIENTS){
					char msg[] = "Server is Full!!\n";
					send(new_socket,msg,strlen(msg),0);
					close(new_socket);
				}
			}
		}
		for(int i = 0;i<MAX_CLIENTS;i++){
			int sd = client_socket[i];
			if(sd>0 && FD_ISSET(sd,&readfds)){
				int n = read(sd,buffer,buffer_size - 1);
				if(n<=0){
					printf("Client %d Disconnected\n",client_id[i]);
					close(sd);
					client_socket[i] = 0;
					client_id[i] = 0;
				}
				else{
					buffer[n] = '\0';
					printf("Client %d: %s",client_id[i],buffer);
					for(int j = 0;j<MAX_CLIENTS;j++){
						if(client_socket[j]>0 && client_socket[j] != sd){
							char message[buffer_size + 50];
							sprintf(message,"Client %d: %s",client_id[i],buffer);
							send(client_socket[j],message,strlen(message),0);
						}
					}
				}
			}
		}
	}
	close(server_fd);
}
//server 10 end

