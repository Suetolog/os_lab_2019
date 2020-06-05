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
  int fd;
  int nread;
  char buf[BUFSIZE];
  struct sockaddr_in servaddr;
  if (argc < 3) {
    printf("Too few arguments \n");
    exit(1);
  }

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(PORT);

  if (connect(fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, BUFSIZE)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
