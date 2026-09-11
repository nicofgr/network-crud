#ifndef NETWORK_H
#define NETWORK_H

// REMOVE LATER
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdint.h>

#define PORT 8082
#define TRUE  1
#define FALSE 0
#define u8 uint8_t


typedef struct star_data{
        char  nome[20];
        float massa;
        float temperatura;
        float luminosidade;
        float raio;
}star_data;

int create_server(int port);
int accept_client(int sockfd);
int create_client();
void connect_to_server(int sockfd, int port);

void print_stardata(star_data data);


#endif

