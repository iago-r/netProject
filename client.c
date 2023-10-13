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

int GAME_INITIALIZED = 0;
int GAME_STATE = 0; // EXIT == -1; STARTED == 1; NOT STARTED == 0;

int previous_state[4][4];

void usage(int argc, char **argv);
void startGameBoard();
int detectType();
void printBoard(struct action msg);
int commandParse(struct action *msg);
void actionResultParse(struct action *msg);

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

    struct action msg;
    startGameBoard();
    while (1) {
        //SENDING PACKAGE...............................................
        bzero(&msg.type, sizeof(msg.type));
        bzero(&msg.coordinates, sizeof(msg.coordinates));
        GAME_STATE = commandParse(&msg);
        send(s, &msg, sizeof(msg), 0);

        //RECEIVING PACKAGE.............................................
        bzero(&msg.type, sizeof(msg.type));
        bzero(&msg.coordinates, sizeof(msg.coordinates));
        recv(s, &msg, sizeof(msg), 0);
        actionResultParse(&msg);
        memcpy(previous_state, msg.board, sizeof(msg.board));

        if (GAME_STATE == -1) {
            break;
        }
    }
    close(s);
    exit(EXIT_SUCCESS);
}

void usage(int argc, char **argv) {
    printf("usage %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

void startGameBoard() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            previous_state[i][j] = -2;
        }
    }
}

int detectType() {
    char buffer[BUFSZ];
    char actionTypes[9][15] = {"start", "reveal", "flag", "state",
                               "remove_flag", "reset", "win", "exit",
                               "game_over"};
    //printf("> ");
    scanf("%s", buffer);
    for (int i = 0; i < 9; i++) {
        if (strcmp(buffer, actionTypes[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int commandParse(struct action *msg) {

    int valid_command;

    do {
        valid_command = 1;

        // USER PROMPT...............................................................
        msg->type = detectType();
        if (msg->type == 1 || msg->type == 2 || msg->type == 4) {
            scanf("%i,%i", &msg->coordinates[0], &msg->coordinates[1]);
        }

        // COMMAND CHECK.............................................................
        if (msg->type == -1 || msg->type == 3 || msg->type == 6 ||msg->type == 8) {
            printf("error: command not found\n");
            valid_command = 0;
        }
        else if (GAME_INITIALIZED == 1) {
            if ((msg->type == 1 || msg->type == 2 || msg->type == 4) && 
                    (!((msg->coordinates[0] >= 0 && msg->coordinates[0] <= 3) &&
                    (msg->coordinates[1] >= 0 && msg->coordinates[1] <= 3)))){
                if (msg->type == 1) {
                    printf("error: invalid cell\n");
                }
                valid_command = 0;
            }
            else if (msg->type == 1) {
                if (previous_state[msg->coordinates[0]][msg->coordinates[1]] >= 0) {
                    printf("error: cell already revealed\n");
                    valid_command = 0;
                }
            }
            else if (msg->type == 2) {
                if (previous_state[msg->coordinates[0]][msg->coordinates[1]] == -3) {
                    printf("error: cell already has a flag\n");
                    valid_command = 0;
                }
                else if (previous_state[msg->coordinates[0]][msg->coordinates[1]] >= 0) {
                    printf("error: cannot insert flag in revealed cell\n");
                    valid_command = 0;
                }
            }
        }
        else if (msg->type == 7) {
            return -1;
        }
    } while (valid_command == 0);
    if (msg->type == 0 || msg->type == 5) {
        GAME_INITIALIZED = 1;
    }
    return 0;
}

void printBoard(struct action msg) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            switch (msg.board[i][j]) {
            case -1:
                printf("*");
                break;

            case -2:
                printf("-");
                break;

            case -3:
                printf(">");
                break;

            default:
                printf("%i", msg.board[i][j]);
                break;
            }
            if (j != 3) {
                printf("\t\t");
            }
        }
        printf("\n");
    }
}

void actionResultParse(struct action *msg) {
    if (GAME_INITIALIZED == 1) {
        switch (msg->type) {
            // SERVER.........................state
            case 3:
                printBoard(*msg);
                break;

            // SERVER..........................win
            case 6:
                printf("YOU WIN!\n");
                printBoard(*msg);
                GAME_INITIALIZED = 0;
                break;

            // SERVER....................game_over
            case 8:
                printf("GAME OVER!\n");
                printBoard(*msg);
                GAME_INITIALIZED = 0;
                break;
        }
    }
}
