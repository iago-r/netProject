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

int GAME_INITIALIZED = 0;
int NO_BOMB_CELLS = 0;
int CELLS_TO_REVEAL;

int answer[4][4];
int state[4][4];

void usage(int argc, char **argv);
int fillGameAnswersBoard(char* argv);
void parseArgs(int argc, char **argv);
void resetGameState();
int countNoBombCells();
void printBoard();
void commandParse(struct action *msg);

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
        
        while(1) {
            //RECEIVING PACKAGE.............................................
            bzero(&msg.type, sizeof(msg.type));
            bzero(&msg.coordinates, sizeof(msg.coordinates));
            recv(csock, &msg, sizeof(msg),0);
            commandParse(&msg);

            //SENDING PACKAGE...............................................
            send(csock, &msg, sizeof(msg), 0);
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
        NO_BOMB_CELLS = countNoBombCells();
        printBoard();
    }
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













void commandParse(struct action *msg) {

    int *answer_cell, *revealing_cell;
    if (msg->type == 1) {
        answer_cell = &answer[msg->coordinates[0]][msg->coordinates[1]];
    }
    if (msg->type == 1 || msg->type == 2 || msg->type == 4) {
        revealing_cell = &state[msg->coordinates[0]][msg->coordinates[1]];
    }

    switch (msg->type) {
        // CLIENT....................reset
        case 5:
            printf("starting new game\n");
            GAME_INITIALIZED = 0;

        // CLIENT....................start
        case 0:
            if (GAME_INITIALIZED == 0) {
                GAME_INITIALIZED = 1;
                CELLS_TO_REVEAL = NO_BOMB_CELLS;
                resetGameState();
            }
            msg->type = 3;
            memcpy(msg->board, state, sizeof(state));
            break;

        // CLIENT....................reveal [int],[int]
        case 1:
            if (GAME_INITIALIZED == 1 && *revealing_cell == -2) {
                if (*answer_cell == -1) {
                    msg->type = 8;
                    memcpy(msg->board, answer, sizeof(answer));
                    resetGameState();
                }
                else {
                    *revealing_cell = *answer_cell;
                    CELLS_TO_REVEAL--;
                    if (CELLS_TO_REVEAL == 0) {
                        msg->type = 6;
                        memcpy(msg->board, answer, sizeof(answer));
                        resetGameState();
                    }
                    else {
                        msg->type = 3;
                        memcpy(msg->board, state, sizeof(state));
                    }
                }
            }
            break;
        
        // CLIENT....................flag [int],[int]
        case 2:
            if (GAME_INITIALIZED == 1 &&
                *revealing_cell == -2) { // cell not revealed or 
                    *revealing_cell = -3;
                }
            msg->type = 3;
            memcpy(msg->board, state, sizeof(state));             
            break;
        
        // CLIENT....................remove_flag [int],[int]
        case 4:
            if (GAME_INITIALIZED == 1 &&
                *revealing_cell == -3) { // cell not revealed or 
                    *revealing_cell = -2;
                }
            msg->type = 3;
            memcpy(msg->board, state, sizeof(state));             
            break;

        // CLIENT....................exit
        case 7:
            GAME_INITIALIZED = 0;
            CELLS_TO_REVEAL = NO_BOMB_CELLS;
            resetGameState();
            break;
    }
}