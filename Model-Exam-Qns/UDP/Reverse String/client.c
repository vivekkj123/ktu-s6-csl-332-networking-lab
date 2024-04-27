#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char ip[] = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    int n;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]UDP client socket created.\n");
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); 
    server_addr.sin_addr.s_addr = inet_addr(ip);

    char buffer[1024];
    printf("Enter a string to reverse: ");
    fgets(buffer, sizeof(buffer), stdin);
    
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("String sent to server.\n");

    addr_size = sizeof(server_addr);
    
    recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_size);
    printf("Reversed string: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}
