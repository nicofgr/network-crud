#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../shared/network.h"

void insert(sqlite3* db, star_data data){
        char buffer[512];
        snprintf(buffer, 512,"INSERT INTO STARS (NAME, MASS, SURFACE_TEMPERATURE, LUMINOSITY, RADIUS)"\
                            "VALUES('%s', %f, %f, %f, %f);", data.nome, data.massa, data.temperatura, data.luminosidade, data.raio);
        int rc;
        char* zErrMsg = 0;

        rc = sqlite3_exec(db, buffer, NULL, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
        } else {
                fprintf(stdout, "Records created successfully\n");
        }
}

void update(){
}

void delete(sqlite3* db, u8 id){
        char buffer[512];
        snprintf(buffer, 512,"DELETE from STARS where ID=%hhu;", id);

        int rc;
        char* zErrMsg = 0;

        rc = sqlite3_exec(db, buffer, NULL, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
        } else {
                fprintf(stdout, "Records deleted successfully\n");
        }
}

int main(){

        sqlite3 *db;
        char* zErrMsg = 0;
        int rc;
        char buffer[1024] = {0};

        rc = sqlite3_open("data.db", &db);

        if(rc){
                fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(db));
                return 0;
        }else{
                fprintf(stderr, "Opened database successfully\n");
        }

        char* sql = "CREATE TABLE STARS(" \
                    "ID                         INTEGER   PRIMARY KEY,"\
                    "NAME                       CHAR(20)  NOT NULL,"\
                    "MASS                       FLOAT     NOT NULL,"\
                    "SURFACE_TEMPERATURE        FLOAT     NOT NULL,"\
                    "LUMINOSITY                 FLOAT     NOT NULL,"\
                    "RADIUS                     FLOAT     NOT NULL );";

        rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

        if(rc != SQLITE_OK){
                fprintf(stderr,"SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
        } else {
                puts("Table created successfully\n");
        }


        // NETWORK 
        int sockfd = create_server(PORT);

        while(1){
                int connfd = accept_client(sockfd);
                int connected = TRUE;

                u8 op;
                u8 id;
                while(connected == TRUE){  // While a client is connected
                        ssize_t bytes = read(connfd, &op, 1);
                        if(bytes == 0){
                                close(connfd);
                                connected = FALSE;
                                break;
                        }
                        printf("%zu bytes received\n", bytes);
                        printf("op: %c (%zu bytes)\n", op, sizeof(op));
                        star_data data;
                        switch(op){
                                case 'C':
                                case 'c':
                                        puts("CREATE");
                                        read(connfd, &data, sizeof(star_data));
                                        print_stardata(data);
                                        insert(db, data);
                                        break;
                                case 'R':
                                case 'r':
                                        puts("READ");
                                        read(connfd, &id, 1);
                                        printf("id: %d (%zu bytes)\n", id, sizeof(id));
                                        break;
                                case 'U':
                                case 'u':
                                        puts("UPDATE");
                                        read(connfd, &id, 1);
                                        printf("id: %d (%zu bytes)\n", id, sizeof(id));
                                        read(connfd, &data, sizeof(star_data));
                                        print_stardata(data);
                                        break;
                                case 'D':
                                case 'd':
                                        puts("DELETE");
                                        read(connfd, &id, 1);
                                        printf("id: %d (%zu bytes)\n", id, sizeof(id));
                                        delete(db, id);
                                        break;
                                default: 
                                        printf("Comando nao reconhecido %c\n", op);
                                        break;
                        }
                        
                }
        }

        //sqlite3_close(db);
        //close(connfd);
        close(sockfd);

        return 0;
}       
