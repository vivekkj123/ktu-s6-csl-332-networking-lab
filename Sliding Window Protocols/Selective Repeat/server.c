#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<pthread.h>

int main() {
  int g = 0, p, f = -1;
  char ch, ca = 'a', can = 'c';
  struct sockaddr_in server, client;
  int len = sizeof(client);
  server.sin_family = AF_INET;
  server.sin_port = 8888;
  server.sin_addr.s_addr = INADDR_ANY;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bind(sockfd, (struct sockaddr * ) & server, sizeof(server));
  listen(sockfd, 5);
  printf("Server is waiting...... \n");
  int newfd = accept(sockfd, (struct sockaddr * ) & client, & len);

  while (1) {
    read(newfd, & ch, 1);
    read(newfd, & g, 1);
    if (ch == 'd') {
      int k = rand();
      if (k % 3 == 0) {
        printf("Data %d corrupted\n", g);
        write(newfd, & can, 1);
        write(newfd, & g, 1);
        sleep(2);
      } else {
        printf("Data received : %d\n", g);
        write(newfd, & ca, 1);
        write(newfd, & g, 1);
        printf("Data acknowledged : %d\n", g);
        f = g;
        sleep(2);
      }
    }
  }
}

