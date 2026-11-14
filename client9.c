//Client 9
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>
#define PORT 7000
int main() {
	int sockfd = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
	char filename[200];
	// Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd< 0) {
		printf("\nSocket creation error\n");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed\n");
		return -1;
	}
	printf("Enter Filename: ");
	fgets(filename,sizeof(filename),stdin);
	filename[strcspn(filename,"\n")] = '\0';
	write(sockfd ,filename,strlen(filename));
	int n;
	while((n = read(sockfd,buffer,sizeof(buffer)-1))>0){
		buffer[n] = '\0';
		printf("%s\n",buffer);
	}
	close(sockfd);
}
//Client 9 end
