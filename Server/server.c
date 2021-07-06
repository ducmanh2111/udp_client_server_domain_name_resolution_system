// Server side implementation of UDP client-server model
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

/* 
Kiểm tra dấu chấm trong địa chỉ ip
Output: 1 - dấu chấm hợp lệ, 0 - dấu chấm lỗi
*/
int check_period(char * string) {
  int count_period = 0, n = strlen(string);

  if (string[0] == '.') return 0;
  if (string[n - 1] == '.') return 0;
  for (int i = 0; i < n - 1; i++) {
    if (string[i] == '.')
      count_period++;
    if (string[i] == '.' && string[i + 1] == '.') //Kiểm tra 2 dấu chấm có cạnh nhau không
      return 0;
  }
  if (count_period != 3) //Số lượng dấu chấm khác 3 sẽ fail
    return 0;
  return 1;
}

/* 
Kiểm tra xem string có là địa chỉ IP không
Output: 1 - là địa chỉ IP, 0 - không là địa chỉ IP
*/
int check_IP(char * string) {
  int value = 0, n = strlen(string);
  if (check_period(string) == 0) {
    return 0;
  } else {
    for (int i = 0; i < n; i++) {
      if (string[i] == '.') {
        if (value < 0 || value > 255)
          return 0;
        value = 0;
      } else {
        if (string[i] >= '0' && string[i] <= '9') {
          value = value * 10 + (string[i] - '0');
          if (i == n - 1)
            if (value < 0 || value > 255)
              return 0;
        } else
          return 0;
      }
    }
    return 1;
  }
}

/*
	Get ip from domain name
 */

int hostname_to_ip(char * hostname, char * ip) {
  struct hostent * he;
  struct in_addr ** addr_list;
  int i;

  if ((he = gethostbyname(hostname)) == NULL) {
    // get the host info
    herror("-gethostbyname");
    return 1;
  }

  addr_list = (struct in_addr ** ) he -> h_addr_list;

  for (i = 0; addr_list[i] != NULL; i++) {
    //Return the first one;
    strcpy(ip, inet_ntoa( * addr_list[i]));
    return 0;
  }

  return 1;
}

// Returns hostname for the local computer
void checkHostName(int hostname) {
  if (hostname == -1) {
    perror("gethostname");
    exit(1);
  }
}

int main(int argc, char * argv[]) {
  char buffer[50] = {
    0
  };
  struct sockaddr_in servaddr = {
    0
  };

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("failed to create socket");
    exit(EXIT_FAILURE);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(argv[1]);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  int rc = bind(sockfd, (const struct sockaddr * ) & servaddr, sizeof(servaddr));

  if (rc == -1) {
    perror("failed to bind");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  socklen_t len = 0;
  int n = recvfrom(sockfd, (char * ) buffer, 50, MSG_WAITALL, 0, & len);
  buffer[n] = '\n';
  printf("%s", buffer);

  if (check_IP(buffer) == 0) {
    char ip[100];
    hostname_to_ip(buffer, ip);
    printf("+Remote address: %s\n", ip);
    sendto(sockfd, ip, sizeof(ip), 0, (const struct sockaddr * ) & servaddr, sizeof(servaddr));
  } else {
    struct sockaddr_in sa; /* input */
    socklen_t len; /* input */
    char hbuf[NI_MAXHOST];

    memset( & sa, 0, sizeof(struct sockaddr_in));

    /* For IPv4*/
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(hbuf);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr * ) & sa, len, hbuf, sizeof(hbuf),
        NULL, 0, NI_NAMEREQD)) {
      printf("-could not resolve hostname\n");
    } else {
      int len = sendto(sockfd, (const char * ) hbuf, strlen(hbuf), 0, (const struct sockaddr * ) & servaddr, sizeof(servaddr));
      if (len == -1) {
        perror("failed to send");
      }
      printf("+host=%s\n", hbuf);
    }
  }
  return 0;
}