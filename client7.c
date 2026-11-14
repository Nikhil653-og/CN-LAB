//Client 7
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000
#define SIZE 1024

int main()
{
	int sockfd;
	struct sockaddr_in server;
	socklen_t server_len=sizeof(server);
	char sentence[SIZE];
	char result[SIZE];
	
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		perror("socket creation failed");
		return 1;
	}
	
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	int n;
	while(1)
	{
			printf("enter a new generation sentence:\n");
			fgets(sentence,SIZE,stdin);
			
			sentence[strcspn(sentence,"\n")]='\0';
			sendto(sockfd,sentence,strlen(sentence)+1,0,(struct sockaddr *)&server,server_len);
			if (strncmp(sentence,"exit",4)==0) break;
	
			memset(result,0,SIZE);
			recvfrom(sockfd,result,SIZE,0,(struct sockaddr *)&server,&server_len);
			printf("\nTranslated sentences:\n%s\n",result);
			
			
	}
	close(sockfd);
	return 0;
}
//Client 7 end
