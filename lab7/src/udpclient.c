#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

	char IP[255] = {'\0'};
    size_t PORT = -1;
	size_t BUFSIZE = -1;
		
	while (true) {

    static struct option options[] = {
        {"ip", required_argument},
        {"port", required_argument},
        {"bufsize", required_argument},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1)
    break;

    switch (c) {
    case 0: {
        switch (option_index) {
            case 0:
                memcpy(IP, optarg, strlen(optarg));
                break;
            case 1:
                PORT = atoi(optarg);
                break;
            case 2:
                BUFSIZE = atoi(optarg);
                break;
            }
        }
    }
    }
  int sockfd, n;
  char sendline[BUFSIZE], recvline[BUFSIZE + 1];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if (argc != 2) {
    printf("usage: client <IPaddress of server>\n");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, IP, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, BUFSIZE)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, BUFSIZE, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}
