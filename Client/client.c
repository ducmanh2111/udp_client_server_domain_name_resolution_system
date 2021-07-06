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

#include <netdb.h>

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#define MAXLINE 1024

int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("Syntax Error.\n");
    printf("Syntax: ./client IPAddress PortNumber\n");
    return 0;
  }
  char str[100];
  struct sockaddr_in servaddr = {
    0
  };

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("failed to create socket");
    exit(EXIT_FAILURE);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(argv[2]);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  for (;;) {
    printf("Please input domain/ip_address: ");
    int n = 0;

    while ((str[n++] == getchar()) != '\n');
    int len = sendto(sockfd, (const char * ) str, strlen(str), 0, (const struct sockaddr * ) & servaddr, sizeof(servaddr));
    if (len == -1) {
      perror("failed to send");
    }
    recvfrom(sockfd, str, sizeof(str), 0, (const struct sockaddr * ) & servaddr, sizeof(servaddr));
    printf("Result is : %s \n", str);
  }
  close(sockfd);

  return 0;
}