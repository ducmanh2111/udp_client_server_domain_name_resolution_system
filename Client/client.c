// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Library of socket
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024

int main(int argc , char* argv[]) {
	int sockfd;
	if (argc != 3) {
		printf("Syntax Error.\n");
		printf("Syntax: ./client IPAddress PortNumber\n");
		return 0;
	}
	printf("Server IP Address is: %s\n",argv[1]);
	printf("Port access is: %s\n", argv[2]);
	return 0;
}
