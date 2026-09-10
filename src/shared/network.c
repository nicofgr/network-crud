#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Network libs
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define print_check(fmt, ...) printf("\033[32m ✓\033[0m " fmt "\n", ##__VA_ARGS__)

int create_server(int port){
        int sockfd;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1){
                perror("Socket creation failed.\n");
                exit(0);
        }
        print_check("Socket creation sucessful.");

        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;  // ipv4 addrs
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);

        if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){
                perror("Socket bind failed.\n");
                exit(0);
        }
        char adr[INET_ADDRSTRLEN];
        print_check("Socket bind sucessful. [Addr: %s:%d]", inet_ntop(AF_INET, &(servaddr.sin_addr), adr, INET_ADDRSTRLEN), ntohs(servaddr.sin_port));

        if(listen(sockfd, 5) != 0){
                perror("Listen failed.\n");
                exit(0);
        }
        print_check("Socket listen sucessful.");
        
        return sockfd;
}

int accept_client(int sockfd){
        int connfd;
        struct sockaddr_in cli;
        unsigned int len;

        len = sizeof(cli);

        puts("Waiting for connection...");
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if(connfd < 0){
                perror("Server accept failed.");
                exit(0);
        }
        print_check("Socket accept sucessful.");
        return connfd;
}

int create_client(){
        int sockfd;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd == -1){
                perror("Socket creation failed\n");
                exit(0);
        }
        return sockfd;
}

void connect_to_server(int sockfd, int port){
        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(port);

        char adr[INET_ADDRSTRLEN];
        printf("Trying to connect to %s:%d\n", inet_ntop(AF_INET, &(servaddr.sin_addr), adr, INET_ADDRSTRLEN), ntohs(servaddr.sin_port));
        if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){
                perror("Connection with the server failed\n");
                exit(0);
        }
        print_check("Connected to server sucessfully.");
}
