#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int factorial(int n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

int main()
{
    char ip[] = "127.0.0.1";
    int port = 5566;
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int n;
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0)
    {
        perror("[-] Socket Error");
        exit(1);
    }
    printf("[+]UDP server socket created.\n");

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

    addr_size = sizeof(client_addr);
    int number;

    recvfrom(server_sock, &number, sizeof(number), 0, (struct sockaddr *)&client_addr, &addr_size);
    printf("Client: %d\n", number);

    int result = factorial(number);

    sendto(server_sock, &result, sizeof(result), 0, (struct sockaddr *)&client_addr, addr_size);

    close(server_sock);
    printf("[+]Server closed.\n");

    return 0;
}
