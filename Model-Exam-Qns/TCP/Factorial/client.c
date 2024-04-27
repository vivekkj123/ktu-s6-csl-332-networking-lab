#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    char ip[] = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    int n;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP client socket created.\n");
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server.\n");
    int num;
    printf("Enter a number to calculate its factorial: ");
    scanf("%d", &num);

    send(sock, &num, sizeof(num), 0);

    int result;
    recv(sock, &result, sizeof(result), 0);
    printf("Factorial of %d: %d\n", num, result);
    close(sock);
    printf("Disconnected from the server.\n");
    return 0;
}
