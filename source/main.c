#include <stdio.h>
#include <string.h>

#ifdef __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#ifndef _WIN32
	#include <windows.h>
	#include <winsock2.h>
#endif

#ifndef WINVER
	#define WINVER 0x0500
#endif

#define PORT 9023
#define SIZE 0x1000
#define OUTPUT "dump.bin"

char buffer[SIZE];
int length;

int main(void) {
	FILE *f;
	
	int server, client;
	struct sockaddr_in serverAddress, clientAddress;
	socklen_t clientLength = sizeof(clientAddress);
	
	#ifndef _WIN32
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	#endif

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT);
	
	bind(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	listen(server, SOMAXCONN);
	
	printf("Listening\n");
	
	while(1) {
		client = accept(server, (struct sockaddr *)&clientAddress, &clientLength);
		printf("Connected\n");

		f = fopen(OUTPUT, "wb");
		
		while((length = recvfrom(client, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, &clientLength))) {
			fwrite(buffer, length, 1, f);
			fflush(f);
			
			if(length < 64) {
				printf("%.*s\n", length, buffer);
			}
			else {
				printf("Packet %d\n", length);
			}
		}
		
		#ifdef __linux__
			close(client);
		#endif

		#ifndef _WIN32
			closesocket(client);
		#endif

		fclose(f);
		
		printf("Done\n");
	}
	
	#ifndef _WIN32
		WSACleanup();
	#endif

	return 0;
}
