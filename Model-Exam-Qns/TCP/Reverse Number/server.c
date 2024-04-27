#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int reverseNumber(int num)
{
  int reversed = 0;
  while (num != 0)
  {
    int digit = num % 10;
    reversed = reversed * 10 + digit;
    num /= 10;
  }
  return reversed;
}

int main()
{
  char ip[] = "127.0.0.1";
  int port = 5566;
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  int n;
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0)
  {
    perror("[-] Socket Error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (n < 0)
  {
    perror("[-] Bind Error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);
  listen(server_sock, 5);
  printf("Listening for Connections...\n");
  addr_size = sizeof(client_addr);

  client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
  printf("[+]Client connected.\n");

  int number;

  recv(client_sock, &number, sizeof(number), 0);
  printf("Client: %d\n", number);

  int reversed = reverseNumber(number);

  send(client_sock, &reversed, sizeof(reversed), 0);

  close(client_sock);
  printf("[+]Client disconnected.\n\n");
  close(server_sock);
  printf("[+]Server closed.\n");

  return 0;
}
