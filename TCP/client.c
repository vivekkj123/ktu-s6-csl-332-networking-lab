#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Header file that provides access to close() function / system functions
#include<unistd.h>
// Header file for internet functions like bind, socket, send, recv, inet_addr => ipaddr -> binary
#include<arpa/inet.h>
int main(){
  // Loopback address
  char *ip = "127.0.0.1";
  int port = 5566;

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;
  // Setup socket
  // AF_INET -> for IPV4
  // SOCK_STREAM -> TCP
  // 0 means 2 way commn. protocol
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP client socket created.\n");
  // to reset server_addr variable
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  // used for converting ip address to binary format. provided by arpa/inet.h
  addr.sin_addr.s_addr = inet_addr(ip);
  // Connect to server
  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");
  while(1){
	  bzero(buffer, 1024);
	  printf("Enter message: ");
	  fgets(buffer,1024,stdin);
	  printf("Client: %s\n", buffer);
	  send(sock, buffer, strlen(buffer), 0);
	  bzero(buffer, 1024);
	  recv(sock, buffer, sizeof(buffer), 0);
	  printf("Server: %s\n", buffer);
  }
  close(sock);
  printf("Disconnected from the server.\n");

  return 0;

}

