//server 8
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

#define PORT 5000
#define BUFFER_SIZE 1000

int main() {
	int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in serverAddress, clientAddress;
	char buffer[BUFFER_SIZE], timeString[BUFFER_SIZE];
	socklen_t clientLength = sizeof(clientAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		perror("Bind creation failed!");
		close(serverSocket);
		exit(1);
	}
	printf("Time server started...\n");
	while(1) {
		recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientLength);
		printf("Parent Received Request\n");
		if (fork() == 0) {
			time_t currentTime = time(NULL);
			strcpy(timeString, ctime(&currentTime));
			sendto(serverSocket, timeString, strlen(timeString) + 1, 0, (struct sockaddr *)&clientAddress, clientLength);
			printf("Child Sent Time String\n\n");
			close(serverSocket);
			exit(0);
		}
	}
	close(serverSocket);
	return 0;
}
//server 8 end

