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
        int n, m, fd, i;
        int sock_ctrl, sock_data, listen_control, listen_data;
        int file_present = 1;
        char name[100], buffer[100], cmd[100];
        struct sockaddr_in servaddr, cliaddr;
        int cli_len = sizeof(cliaddr);
        if (argc != 2) {
            fprintf(stderr, "Usage: ./server port\n");
            exit(1);
        }
        if ((listen_control = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("cannot create listening socket for control connection");
            exit(1);
        }
        bzero( & servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(atoi(argv[1]));
        if (bind(listen_control, (struct sockaddr * ) & servaddr, sizeof(servaddr)) < 0) {
            perror("server bind failed for control listening socket");
            exit(1);
        }
        listen(listen_control, 5);
        if ((listen_data = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("cannot create listening socket for data connection");
            exit(1);
        }
        servaddr.sin_port = htons(atoi(argv[1]) + 1);
        if (bind(listen_data, (struct sockaddr * ) & servaddr, sizeof(servaddr)) < 0) {
            perror("server bind failed for data listening socket");
            exit(1);
        }
        listen(listen_control, 5);
        listen(listen_data, 5);
        for (;;) {
            if ((sock_ctrl = accept(listen_control, (struct sockaddr * ) & servaddr, & cli_len)) < 0) {
                perror("accept failed");
                exit(1);
            }
            servaddr.sin_port = htons(atoi(argv[1] + 1));
            if (fork() == 0) {
                close(listen_control);
                while (1) {
                    i = 0;
                    n = read(sock_ctrl, cmd, 100);
                    if (strcmp(cmd, "close") == 0) {
                        break;
                    } else {
                        read(sock_ctrl, name, 100);
                        if ((fd = open(name, O_RDONLY)) < 0) {
                            sprintf(buffer, "@FILE NOT FOUND PROCESS ID = %d",
                                getpid());
                            perror("error in opening file");
                            file_present = 0;
                        }
                        if ((sock_data = accept(listen_data, (struct sockaddr * ) & servaddr, &
                                cli_len)) < 0) {
                            perror("accept failed");
                            exit(1);
                        }
                        if (file_present == 1) {
                            do {
                                n = read(fd, buffer, sizeof(buffer));
                                write(sock_data, buffer, n);
                                i = i + n;
                            } while (n > 0);
                            close(fd);
                            sprintf(buffer, "FILE %s RECEIVED FROM SERVER WITH PROCESS ID = % d ",name, getpid());
                                m = write(sock_data, buffer, strlen(buffer));
                            }
                            else {
                                m = write(sock_data, buffer, sizeof(buffer));
                                file_present = 1;
                                bzero(buffer, sizeof(buffer));
                            }
                            close(sock_data);
                        }
                    }
                    close(sock_ctrl);
                    close(listen_data);
                    exit(0);
                }
                close(sock_ctrl);
            }
            close(listen_control);
            close(listen_data);
        }
