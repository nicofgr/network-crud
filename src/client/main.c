#include <stdio.h>
#include "../shared/network.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>



int main(){

        int connfd;

        connfd = create_client();
        connect_to_server(connfd, PORT);

        int quit = FALSE;
        while(quit == FALSE){
                puts(" ╔═══════════════════╗\n"
                     " ║ Escolha uma opção ║\n"
                     " ╠═══════════════════╣\n"
                     " ║ C - Criar         ║\n"
                     " ║ R - leR           ║\n"
                     " ║ U - atUalizar     ║\n"
                     " ║ D - Deletar       ║\n"
                     " ║ q - sair          ║\n" 
                     " ╚═══════════════════╝");

                // TODO Arrumar a leitura pra ignorar strings
                u8 op;
                do{
                        op = getchar();
                }while(op == '\n');
                int temp;
                while((temp = getchar()) != '\n' && temp != EOF);

                star_data data;
                int message_size;
                char* message;

                u8 id; // TODO expand (only 256)
                switch(op){
                        case 'C':
                        case 'c':
                                puts("CRIAR");
                                puts("Digite o nome da estrela:");
                                scanf("%s", data.nome);
                                puts("Digite a massa da estrela:");
                                scanf("%f", &data.massa);
                                puts("Digite a temperatura da estrela:");
                                scanf("%f", &data.temperatura);
                                puts("Digite a luminosidade da estrela:");
                                scanf("%f", &data.luminosidade);
                                puts("Digite o raio da estrela:");
                                scanf("%f", &data.raio);

                                message_size = sizeof(op) + sizeof(star_data);
                                message = (char*)malloc(message_size);
                                memcpy(message, &op, sizeof(op));
                                memcpy(message+sizeof(op), &data, message_size);
                                printf("%s\n", message);

                                //send(connfd, &op, sizeof(op), 0);
                                send(connfd, message, message_size, 0);
                                free(message);
                                break;
                        case 'R':
                        case 'r':
                                puts("LER");
                                puts("Digite o ID desejado:");
                                scanf("%hhu", &id);
                                message_size = sizeof(op) + sizeof(id);
                                message = (char*)malloc(message_size);
                                memcpy(message, &op, sizeof(op));
                                memcpy(message+sizeof(op), &id, sizeof(id));
                                send(connfd, message, message_size, 0);
                                free(message);
                                break;
                        case 'U':
                        case 'u':
                                puts("ATUALIZAR");

                                puts("Digite o ID desejado:");
                                scanf("%hhu", &id);

                                puts("Digite o nome da estrela:");
                                scanf("%s", data.nome);
                                puts("Digite a massa da estrela:");
                                scanf("%f", &data.massa);
                                puts("Digite a temperatura da estrela:");
                                scanf("%f", &data.temperatura);
                                puts("Digite a luminosidade da estrela:");
                                scanf("%f", &data.luminosidade);
                                puts("Digite o raio da estrela:");
                                scanf("%f", &data.raio);


                                message_size = sizeof(op) + sizeof(id) + sizeof(data);

                                message = (char*)malloc(message_size);

                                memcpy(message, &op, sizeof(op));
                                memcpy(message+sizeof(op), &id, sizeof(id));
                                memcpy(message+sizeof(op)+sizeof(id), &data, sizeof(data));

                                send(connfd, message, message_size, 0);
                                free(message);
                                break;
                        case 'D':
                        case 'd':
                                puts("DELETAR");
                                puts("Digite o ID desejado:");
                                scanf("%hhu", &id);
                                message_size = sizeof(op) + sizeof(id);
                                message = (char*)malloc(message_size);
                                memcpy(message, &op, sizeof(op));
                                memcpy(message+sizeof(op), &id, sizeof(id));
                                send(connfd, message, message_size, 0);
                                free(message);
                                break;
                        case 'Q':
                        case 'q':
                                quit = TRUE;
                                break;
                        default: 
                                printf("Comando nao reconhecido %c\n", op);
                                break;
                }
        }

        close(connfd);
        
        return 0;
}
