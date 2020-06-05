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

    size_t PORT = -1;
	size_t BUFSIZE = -1;
		
	while (true) {

    static struct option options[] = {
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
                PORT = atoi(optarg);
                break;
            case 1:
                BUFSIZE = atoi(optarg);
                break;
            }
        }
    }
    }
  int sockfd, n;
  char mesg[BUFSIZE], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts...\n");

  while (1) {
    unsigned int len = sizeof(struct sockaddr_in);

    if ((n = recvfrom(sockfd, mesg, BUFSIZE, 0, (struct sockaddr *)&cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    mesg[n] = 0;

    printf("REQUEST %s      FROM %s : %d\n", mesg,
           inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
           ntohs(cliaddr.sin_port));

    if (sendto(sockfd, mesg, n, 0, (struct sockaddr *)&cliaddr, len) < 0) {
      perror("sendto");
      exit(1);
    }
  }
}
