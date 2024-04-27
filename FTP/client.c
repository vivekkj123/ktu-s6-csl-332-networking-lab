#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char * argv[]) {
        int n, fd, i;
        int sock_ctrl, sock_data;
        char buffer[100], line[100], cmd[100];
        char name[100];
        char * p;
        struct sockaddr_in servaddr;
        if (argc != 3) {
            fprintf(stderr, "Usage: ./client IPaddress_of_server port\n");
            exit(1);
        }
        if ((sock_ctrl = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("Cannot create control socket\n");
            exit(1);
        }
        bzero((char * ) & servaddr, sizeof(servaddr));
        bzero(line, sizeof(line));
        bzero(buffer, sizeof(buffer));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
        inet_pton(AF_INET, argv[1], & servaddr.sin_addr);
        if (connect(sock_ctrl, (struct sockaddr * ) & servaddr, sizeof(servaddr)) < 0) {
            perror("Connection failed, control socket:");
            exit(1);
        }
        printf(" Enter \"get\" for receiving file from server\n");
        printf(" Enter \"close\" for closing connection\n");
        while (1) {
            i = 0;
            if ((sock_data = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("Creation of client data socket failed");
                    exit(1);
                }
                printf("Enter command:");
                scanf("%s", cmd);
                n = write(sock_ctrl, cmd, sizeof(cmd));
                if (strcmp(cmd, "close") == 0) {
                    close(sock_data);
                    close(sock_ctrl);
                    break;
                }
                printf("Enter filename:");
                scanf("%s", name);
                printf("name of file = %s\n", name);
                if ((fd = open(name, O_WRONLY | O_CREAT)) < 0) {
                    perror("Error in opening file ");
                    exit(1);
                }
                write(sock_ctrl, name, sizeof(name));
                servaddr.sin_port = htons(atoi(argv[2]) + 1);
                n = connect(sock_data, (struct sockaddr * ) & servaddr, sizeof(servaddr));
                while (n == -1) {
                    n = connect(sock_data, (struct sockaddr * ) & servaddr, sizeof(servaddr));
                    perror("cannot connect");
                }
                do {
                    n = read(sock_data, buffer, sizeof(buffer));
                    write(fd, buffer, n);
                    i += n;
                } while (n > 0);
                if (buffer[0] == '@') {
                    p = buffer;
                    printf("%s\n", p++);
                    remove(name);
                }
            }
            return 0;
        }
