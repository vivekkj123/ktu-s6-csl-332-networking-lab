#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<pthread.h>


main() {
  int sockfd, newsockfd, clilen, g = 0, n, m, p = 0, f = -1, k;
  char ca = 'a', can = 'c', ch;
  struct sockaddr_in serv_addr, cli_addr;
  serv_addr.sin_family = PF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(8084);
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  bind(sockfd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr));
  listen(sockfd, 5);
  printf("Server is waiting	\n");
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr * ) & cli_addr, & clilen);
  while (1) {
    n = read(newsockfd, & ch, 1);
    m = read(newsockfd, & g, 1);
    if (ch = 'd') {
      k = rand();
      if (k % 3 == 0) {
        if (g == f + 1) {
          printf("Data %d Corrupted\n", g);
          write(newsockfd, & can, 1);
          write(newsockfd, & g, 1);
          sleep(2);
        } else {
          printf("Discarded data %d\n", g);
        }
      } else {
        p = f + 1;
        if (g == p) {
          printf("Data %d received\n", g);
          write(newsockfd, & ca, 1);
          write(newsockfd, & g, 1);
          printf("Data %d acknowledged\n", g);
          f = g;
        } else
          printf("Discarded data %d\n", g);

        sleep(2);
      }
    }
  }
}

