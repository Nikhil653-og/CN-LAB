//Client 8
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 5000
#define BUFFER_SIZE 1000

int main() {
	int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in serverAddress;
	char request[] = "TIME", timeString[BUFFER_SIZE];
	socklen_t serverLength = sizeof(serverAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	sendto(clientSocket, request, strlen(request) + 1, 0, (struct sockaddr *)&serverAddress, serverLength);
	printf("Time request send to server..\n");
	recvfrom(clientSocket, timeString, BUFFER_SIZE, 0, (struct sockaddr *)&serverAddress, &serverLength);
	printf("Server Time: %s", timeString);
	close(clientSocket);
	return 0;
}
//Client 8 end
