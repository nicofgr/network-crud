#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../shared/network.h"

#define TRUE  1
#define FALSE 0

int main(){

        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char buffer[1024] = {0};

        rc = sqlite3_open("test.db", &db);

        if(rc){
                fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(db));
                return 0;
        }else{
                fprintf(stderr, "Opened database successfully\n");
        }
        sqlite3_close(db);

        
        int sockfd = create_server(8080);

        while(1){
                int connfd = accept_client(sockfd);
                int connected = TRUE;

                int op;
                while(connected == TRUE){
                        ssize_t bytes = read(connfd, &op, 1);
                        if(bytes == 0){
                                close(connfd);
                                connected = FALSE;
                                break;
                        }
                        printf("Message received: %c\n", op);
                }
        }

        //close(connfd);
        close(sockfd);

        return 0;
}       
