#include <stdio.h>

#include <windows.h>
#include <winsock.h>

#ifndef WINVER
	#define WINVER 0x0500
#endif

char buffer[0x10000];
int length;

int main(void) {
	FILE *f;
	
	int server, client;
	struct sockaddr_in serverAddress, clientAddress;
	int clientLength = sizeof(clientAddress);
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(9023);
	
	bind(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	listen(server, SOMAXCONN);
	
	while(1) {
		client = accept(server,(struct sockaddr *)&clientAddress, &clientLength);
		
		length = recvfrom(client, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, &clientLength);
		//sendto(client, buffer, length, 0, (struct sockaddr *)&clientAddress, sizeof(clientAddress));
		
		closesocket(client);
		
		f = fopen("dump.bin", "wb");
		fwrite(buffer, length, 1, f);
		fclose(f);
		
		printf("Dumped\n");
	}
	
	WSACleanup();
	
	return 0;
}
