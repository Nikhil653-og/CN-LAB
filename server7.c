//server 7
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000
#define SIZE 1024

void replaceword(char *sentence,const char *word,const char *replacement)
{
	char temp[SIZE];
	char *pos;
	while((pos=strstr(sentence,word))!=NULL)
	{
		temp[0]='\0';
		strncat(temp,sentence,pos-sentence);
		strcat(temp,replacement);
		strcat(temp,pos+strlen(word));
		strcpy(sentence,temp);
	}
}
int main()
{
	int sockfd;
	struct sockaddr_in server,client;
	socklen_t client_len=sizeof(client);
	char sentence[SIZE];
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		perror("socket creation failed");
		return 1;
	}
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=INADDR_ANY;
	
	if(bind(sockfd,(struct sockaddr*)&server,sizeof(server))<0)
	{
		perror("bind failed");
		close(sockfd);
		return 1;
	}
	printf("UDP Translation Server started...\n");
	while(1)
	{	
		memset(sentence,0,SIZE);
		recvfrom(sockfd,sentence,SIZE,0,(struct sockaddr*)&client,&client_len);
		if (strncmp(sentence,"exit",4)==0) break;
		printf("recieved:%s\n",sentence);
		replaceword(sentence,"tbh","to be honest");
		replaceword(sentence,"ig","i guess");
		replaceword(sentence,"tbf","to be fair");
		replaceword(sentence,"atm","at the moment");
		replaceword(sentence,"irl","in real life");
		replaceword(sentence,"lol","laugh out loud");
		replaceword(sentence,"asap","as soon as possible");
		replaceword(sentence,"omg","oh my god");
		replaceword(sentence,"ttyl","talk to you later");
		replaceword(sentence,"idk","i dont know");
		replaceword(sentence,"nvm","nevermind");
		replaceword(sentence,"idc","i dont care");
		
		printf("translated:%s\n",sentence);
		sendto(sockfd,sentence,strlen(sentence)+1,0,(struct sockaddr*)&client,client_len);
	}
	close(sockfd);
	return 1;	
}
//server 7 end
