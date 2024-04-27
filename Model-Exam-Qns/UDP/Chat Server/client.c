#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    char ip[] = "127.0.0.1";
    int port = 5566;
    int client_sock;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    int n;
    char buffer[1024];

    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock < 0)
    {
        perror("[-] Socket Error");
        exit(1);
    }
    printf("[+] UDP client socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    while (1)
    {

        bzero(buffer, 1024);

        printf("Enter message for server: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (strstr(buffer, "$") != NULL)
        {
            printf("Chat ended by client.\n");
            break;
        }

        n = sendto(client_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (n < 0)
        {
            perror("[-] Send Error");
            exit(1);
        }

        addr_size = sizeof(server_addr);
        bzero(buffer, 1024);

        n = recvfrom(client_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_size);
        if (n < 0)
        {
            perror("[-] Receive Error");
            exit(1);
        }
        printf("Received from server: %s", buffer);
    }

    close(client_sock);
    printf("[+] Client closed.\n");

    return 0;
}
