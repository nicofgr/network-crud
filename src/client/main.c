#include <stdio.h>
#include "../shared/network.h"
#include <string.h>
#include <stdint.h>

#define TRUE  1
#define FALSE 0
#define u8 uint8_t

int main(){

        int connfd;

        connfd = create_client();
        connect_to_server(connfd, 8080);

        char* message = "Standard message.";
        char buffer[1024] = {0};

        int quit = FALSE;
        while(quit == FALSE){
                puts(" ╔═══════════════════╗\n"
                     " ║ Escolha uma opção ║\n"
                     " ╠═══════════════════╣\n"
                     " ║ C - Criar         ║\n"
                     " ║ R - Ler           ║\n"
                     " ║ U - Atualizar     ║\n"
                     " ║ D - Deletar       ║\n"
                     " ║ q - Sair          ║\n" 
                     " ╚═══════════════════╝");

                u8 op;
                do{
                        op = getchar();
                }while(op == '\n');

                switch(op){
                        case 'C':
                        case 'c':
                                send(connfd, &op, sizeof(op), 0);
                                break;
                        case 'R':
                        case 'r':
                                send(connfd, &op, sizeof(op), 0);
                                break;
                        case 'U':
                        case 'u':
                                send(connfd, &op, sizeof(op), 0);
                                break;
                        case 'D':
                        case 'd':
                                send(connfd, &op, sizeof(op), 0);
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
