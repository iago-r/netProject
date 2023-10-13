#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>


#define BUFSZ 1024

struct action {
    int type;
    int coordinates[2];
    int board[4][4];
    char buf[BUFSZ];
};

int GAME_INITIALIZED = 0; // 0 for no and 1 for yes
int PLAYER_STATE = 0; // 0 for not losing and 1 for lose
int cellsToReveal, cell_value;

int answer[4][4];
int state[4][4];

void usage(int argc, char **argv);
int fillGameAnswersBoard(char* argv);
void parseArgs(int argc, char **argv);
void resetGameState();
int countNoBombCells();
void printBoard();

int main(int argc, char **argv) {
    if (argc < 5) {
        usage(argc, argv);
    }

    parseArgs(argc, argv);

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    int enable = 1;
    if(0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage))) {
        logexit("bind");
    }

    if (0 != listen(s, 10)) { // quantidade de conexões que podem estar pendentes para tratamento
        logexit("listen");
    }

    struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddrlen = sizeof(cstorage);

    int csock;
    struct action msg;
    
    
    while (1) {
        csock = accept(s, caddr, &caddrlen);
        if (csock == -1) {
            logexit("accept");
        }
        else {
            printf("client connected\n");
        }
        

        cellsToReveal = countNoBombCells();
        while(1) {
            bzero(&msg, sizeof(msg));
            recv(csock, &msg, sizeof(msg),0);
            /*
            if (msg.type == 1 || msg.type == 2 || msg.type == 3) {
                printf("From client: Your action is: %i\n\tAnd your coordinates are: %i,%i\n", msg.type, msg.coordinates[0], msg.coordinates[1]);
            } else {
                printf("From client: Your action is: %i\n", msg.type);
            }
            printf("There are, still, %i no bomb cells\n", cellsToReveal);
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    printf("%i\t\t", answer[i][j]);
                }
                printf("\n");
            }
            */

            switch (msg.type) {
                // CLIENT....................START
                case 0:
                    if (GAME_INITIALIZED == 0 && PLAYER_STATE == 0) {
                        GAME_INITIALIZED = 1;
                        bzero(&msg, sizeof(msg));
                        resetGameState();
                    }
                    msg.type = 3;
                    memcpy(msg.board, state, sizeof(state));
                    break;
                
                // CLIENT....................reveal [int],[int]
                case 1:
                    if (GAME_INITIALIZED == 1 && state[msg.coordinates[0]][msg.coordinates[1]] == -2) {
                        cell_value = answer[msg.coordinates[0]][msg.coordinates[1]];
                        //printf("%i", cell_value);
                        if (cell_value == -1) {
                            //bzero(&msg, sizeof(msg));
                            msg.type = 8;
                            memcpy(msg.board, answer, sizeof(answer));
                            GAME_INITIALIZED = 0;
                            PLAYER_STATE = 1;
                            resetGameState();
                        }
                        else {
                            state[msg.coordinates[0]][msg.coordinates[1]] = answer[msg.coordinates[0]][msg.coordinates[1]];
                            cellsToReveal--;
                            if (cellsToReveal == 0) {
                                msg.type = 6;
                            }
                            else {
                                msg.type = 3;
                            }
                            memcpy(msg.board, state, sizeof(answer));
                        }

                        // bzero(&msg, sizeof(msg));
                        // msg.type = 3;
                        // resetGameState();
                        // memcpy(msg.board, state, sizeof(state));
                    }
                    break;
                
                // CLIENT....................flag [int],[int]
                case 2:
                    if (GAME_INITIALIZED == 1 &&
                        state[msg.coordinates[0]][msg.coordinates[1]] == -2) { // cell not revealed or 
                            state[msg.coordinates[0]][msg.coordinates[1]] = -3;
                        }
                    msg.type = 3;
                    memcpy(msg.board, state, sizeof(state));             
                    break;
                
                // DANDO PAU! O STRCMP NÃO DETECTA A FUNCAO REMOVE_FLAG DA LISTA E RETORNA -1
                // CLIENT....................remove_flag [int],[int]
                case 4:
                    if (GAME_INITIALIZED == 1 &&
                        state[msg.coordinates[0]][msg.coordinates[1]] == -3) { // cell not revealed or 
                            state[msg.coordinates[0]][msg.coordinates[1]] = -2;
                        }
                    msg.type = 3;
                    memcpy(msg.board, state, sizeof(state));             
                    break;

                // CLIENT....................reset
                case 5:
                    printf("starting new game\n");
                    msg.type = 3;
                    resetGameState();
                    memcpy(msg.board, state, sizeof(answer));
                    GAME_INITIALIZED = 0;
                    PLAYER_STATE = 0;
                    break;
                
                // CLIENT....................exit
                case 7:
                    break;
            }
            
            
            send(csock, &msg, sizeof(msg),0);
            if (msg.type ==  7) {
                printf("client disconnected\n");
                break;
            }
        }
    }
    close(csock);
    exit(EXIT_SUCCESS);
}

void usage(int argc, char **argv) {
    printf("usage: %s <v4|v6> <server port> -i input/<filename>.txt\n", argv[0]);
    printf("example: %s v4 51511 -i input/jogo.txt\n", argv[0]);
    exit(EXIT_FAILURE);
}

int fillGameAnswersBoard(char* argv) {
    FILE* fp;
    if ((fp = fopen(argv, "r")) == NULL) {
        printf("can't open %s\n", argv);
        return -1;
    }
    else {
        for (int i = 0; i < 4; i++) {
            fscanf(fp, "%i,%i,%i,%i", &answer[i][0], &answer[i][1], &answer[i][2], &answer[i][3]);
        }
        fclose(fp);      
        return 0;  
    }
}

void parseArgs(int argc, char **argv) {
    int i;
    for (i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0) {
            break;
        }
    }
    if(i == argc) {
        exit(EXIT_FAILURE);
    }
    else {
        if(fillGameAnswersBoard(argv[i+1]) == -1){
            exit(EXIT_FAILURE);
        }
        printBoard();
    }
}

int countNoBombCells() {
    int counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (answer[i][j] >= 0) {
                counter++;
            }
        }
    }
    return counter;
}

void printBoard() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            switch (answer[i][j]) {
            case -1:
                printf("*"); // in case i nedd right alignment -> %3i
                break;

            case -2:
                printf("-");
                break;

            case -3:
                printf(">");
                break;

            default:
                printf("%i", answer[i][j]);
                break;
            }
            if (j != 3) {
                printf("\t\t");
            }
        }
        printf("\n");
    }
}

void resetGameState(){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
        state[i][j] = -2;
        }
    }
}

// SE O CLIENT QUITA O SERVER CAI, MAS SE O SERVER QUITA O CLIENT Nz