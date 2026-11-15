#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 7000
#define MAX_CLIENTS 30

int main()
{
	struct sockaddr_in servaddr,clientaddr;
	int sockfd,new_socket;
	socklen_t addrlen = sizeof(clientaddr);
	fd_set readfds;
	char buffer[2000];
	int activity,max_sd,sd,next_id=1,max_clients=MAX_CLIENTS;
	int client_sockets[MAX_CLIENTS],client_ids[MAX_CLIENTS];

	for(int i=0;i<max_clients;i++)
	{
		client_sockets[i]=0;
		client_ids[i]=0;
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if(listen(sockfd,5)<0)
	{
		perror("Listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Server is Listening on port %d...\n",PORT);

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(sockfd,&readfds);
		max_sd = sockfd;

		for(int i=0;i<max_clients;i++)
		{
			sd = client_sockets[i];
			if(sd>0)
			{
				FD_SET(sd,&readfds);
				if(sd>max_sd)
					max_sd=sd;
			}
		}

		activity = select(max_sd+1,&readfds,NULL,NULL,NULL);
		if(activity < 0)
		{
			perror("select failed");
			continue;
		}

		if(FD_ISSET(sockfd,&readfds))
		{
			addrlen = sizeof(clientaddr);
			new_socket = accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen);

			if(new_socket < 0)
			{
				perror("accept failed");
			}
			else
			{
				int slot_found = 0;

				for(int i=0;i<max_clients;i++)
				{
					if(client_sockets[i]==0)
					{
						client_sockets[i]=new_socket;
						client_ids[i]=next_id++;

						printf("Client %d connected\n",client_ids[i]);

						char id_msg[100];
						sprintf(id_msg,"You are Client %d\n",client_ids[i]);
						send(new_socket,id_msg,strlen(id_msg),0);

						char broadcast[200];
						sprintf(broadcast,"Client %d joined the chat\n",client_ids[i]);

						for(int j=0;j<max_clients;j++)
						{
							if(client_sockets[j]!=0 && client_sockets[j]!=new_socket)
							{
								send(client_sockets[j],broadcast,strlen(broadcast),0);
							}
						}

						slot_found = 1;
						break;
					}
				}

				if(!slot_found)
				{
					char full_msg[] = "Chat server is full. Try again later.\n";
					send(new_socket,full_msg,strlen(full_msg),0);
					close(new_socket);
				}
			}
		}

		for(int i=0;i<max_clients;i++)
		{
			sd = client_sockets[i];

			if(sd>0 && FD_ISSET(sd,&readfds))
			{
				int val_read = read(sd,buffer,sizeof(buffer)-1);

				if(val_read<=0)
				{
					char broadcast[200];
					sprintf(broadcast,"Client %d left the chat\n",client_ids[i]);
					printf("%s",broadcast);

					for(int j=0;j<max_clients;j++)
					{
						if(client_sockets[j]!=0 && client_sockets[j]!=sd)
						{
							send(client_sockets[j],broadcast,strlen(broadcast),0);
						}
					}

					close(sd);
					client_sockets[i]=0;
					client_ids[i]=0;
				}
				else
				{
					buffer[val_read]='\0';

					char broadcast[2200];
					sprintf(broadcast,"Client %d: %s",client_ids[i],buffer);

					printf("%s",broadcast);

					for(int j=0;j<max_clients;j++)
					{
						if(client_sockets[j]!=0 && client_sockets[j]!=sd)
						{
							send(client_sockets[j],broadcast,strlen(broadcast),0);
						}
					}
				}
			}
		}
	}

	close(sockfd);
	return 0;
}
