#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 1024

struct action {
    int type;
    int coordinates[2];
    int board[4][4];
    char buf[BUFSZ];
};

int current_state[4][4]; /* = { {-2, -2, -2, -2},
                      {-2, -2, -2, -2}, 
                      {-2, -2, -2, -2}, 
                      {-2, -2, -2, -2}, };*/

void usage(int argc, char **argv) {
    printf("usage %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int detectType();
void printBoard(struct action msg);

int main(int argc, char **argv) {
    if (argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != connect(s, addr, sizeof(storage))) {
        logexit("connect");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    
    printf("connected to %s\n", addrstr);

    struct action msg;
    while (1) {
        //bzero(&msg, sizeof(msg));
        bzero(&msg.type, sizeof(msg.type));
        bzero(&msg.coordinates, sizeof(msg.coordinates));
        printf("> ");
        //fgets(msg.buf, BUFSZ - 1, stdin);
        //scanf("%s", msg.buf);
//====================================================================//
        msg.type = detectType();
        if (msg.type == 1 || msg.type == 2 || msg.type == 3) {
            scanf("%i,%i", &msg.coordinates[0], &msg.coordinates[1]);
        }
        

       // if




//====================================================================//
        send(s, &msg, sizeof(msg), 0);

        //bzero(&msg, sizeof(msg));
        bzero(&msg.type, sizeof(msg.type));
        bzero(&msg.coordinates, sizeof(msg.coordinates));
        recv(s, &msg, sizeof(msg), 0);
        //memcpy(current_state, msg.board, sizeof(msg.board));
        
        switch (msg.type)
        {
            // SERVER....................state
            case 3:
                printBoard(msg);
                break;

            // SERVER....................win
            case 6:
                printf("YOU WIN!\n");
                printBoard(msg);
                /* code */
                break;
            
            // SERVER....................game_over
            case 8:
                printf("GAME OVER!\n");
                printBoard(msg);
                /* code */
                break;
        }
        
        if ((strncmp(msg.buf, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
    close(s);

    exit(EXIT_SUCCESS);
}

int detectType() {
    char buffer[BUFSZ];
    char actionTypes[9][12] = {"start", "reveal", "flag", "state",
                            "remove_flag", "reset", "win", "exit", 
                            "game_over"};
    scanf("%s", buffer);
    for (int i = 0; i < 9; i++)
    {
        if(strcmp(buffer, actionTypes[i]) == 0) {
            // the action will be the index of the loop
            return i;
        }
    }
    return -1;
}

void printBoard(struct action msg) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //printf("%3i\t\t", msg.board[i][j]);
            switch (msg.board[i][j]) {
                case -1:
                    printf("*\t\t");
                    break;
                
                case -2:
                    printf("-\t\t");
                    break;

                case -3:
                    printf(">\t\t");
                    break;
                
                default:
                    printf("%i\t\t", msg.board[i][j]);
                    break;
            }
/*             if(i < 3) {
                printf("\t\t");
            } */
        }
        printf("\n");
    }
}