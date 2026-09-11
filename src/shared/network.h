#ifndef NETWORK_H
#define NETWORK_H

// REMOVE LATER
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int create_server(int port);
int accept_client(int sockfd);
int create_client();
void connect_to_server(int sockfd, int port);




#endif

