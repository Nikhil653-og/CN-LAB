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
	struct sockaddr_in servaddr;
	int sockfd;
	socklen_t addrlen = sizeof(servaddr);
	fd_set readfds;
	char buffer[2000],message[2000];

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if(connect(sockfd,(struct sockaddr*)&servaddr,addrlen)<0)
	{
		perror("Connection failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Connected to chat. Type exit, quit or bye to leave.\n");

	while(1)
	{
		FD_ZERO(&readfds);

		FD_SET(0,&readfds);
		FD_SET(sockfd,&readfds);

		if(select(sockfd+1, &readfds, NULL, NULL, NULL)<0)
		{
			perror("select failed");
			break;
		}

		if(FD_ISSET(0,&readfds))
		{
			printf("Type Message: ");
			if(fgets(buffer,sizeof(buffer),stdin)==NULL)
				break;

			if(strcmp(buffer,"exit\n")==0 ||
			   strcmp(buffer,"quit\n")==0 ||
			   strcmp(buffer,"bye\n")==0 ||
			   strcmp(buffer,"exit")==0 ||
			   strcmp(buffer,"quit")==0 ||
			   strcmp(buffer,"bye")==0)
			{
				printf("Leaving chat...\n");
				break;
			}

			send(sockfd,buffer,strlen(buffer),0);
		}

		if(FD_ISSET(sockfd,&readfds))
		{
			int val_read = read(sockfd,message,sizeof(message)-1);
			if(val_read<=0)
			{
				printf("Server disconnected...\n");
				break;
			}
			else
			{
				message[val_read]='\0';
				printf("%s",message);
				fflush(stdout);
			}
		}
	}

	close(sockfd);
	return 0;
}
