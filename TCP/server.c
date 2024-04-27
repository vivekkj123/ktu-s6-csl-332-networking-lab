#include <stdio.h>
#include <stdlib.h>
#include<string.h>
// Header file that provides access to close() function / system functions
#include<unistd.h>
// Header file for internet functions like bind, socket, send, recv, inet_addr => ipaddr -> binary
#include<arpa/inet.h>

int main(){

  char *ip = "127.0.0.1";
  int port = 5566;
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;
  // Setup socket
  // AF_INET -> for IPV4
  // SOCK_STREAM -> TCP
  // 0 means 2 way commn. protocol
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  // if err, throw err
  if (server_sock < 0){
    // to print error message
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
  // to reset server_addr variable
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  // used for converting ip address to binary format. provided by arpa/inet.h
  server_addr.sin_addr.s_addr = inet_addr(ip);
 // Bind the socket descriptor to the server address:
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number: %d\n", port);
  // Listen for incoming connections, maximum size=5
  listen(server_sock, 5);
  printf("Listening...\n");
addr_size = sizeof(client_addr);
    // Accept connections from client
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Client connected.\n");
  while(1){
    
    // Clears data in buffer
    bzero(buffer, 1024);
    // Recieve message and store in buffer variable
    recv(client_sock, buffer, sizeof(buffer), 0);
    printf("Client: %s\n", buffer);
    bzero(buffer, 1024);
    printf("Enter message: ");
	fgets(buffer,1024,stdin);
    printf("Server: %s\n", buffer);
    send(client_sock, buffer, sizeof(buffer), 0);
  }
	  // Disconnect present client from server
  close(client_sock);
  printf("[+]Client disconnected.\n\n");
  return 0;
}

