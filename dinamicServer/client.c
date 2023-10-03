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

int previous_state[4][4];

void resetPreviousState();
int detectType();
void printBoard(struct action msg);
void commandParse(struct action *msg);
void actionResultParse(struct action *msg);

void usage(int argc, char **argv) {
    printf("usage %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

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
    resetPreviousState();
    while (1) {
        bzero(&msg.type, sizeof(msg.type));
        bzero(&msg.coordinates, sizeof(msg.coordinates));
        commandParse(&msg);
        send(s, &msg, sizeof(msg), 0);

        bzero(&msg.type, sizeof(msg.type));
        bzero(&msg.coordinates, sizeof(msg.coordinates));
        recv(s, &msg, sizeof(msg), 0);
        actionResultParse(&msg);
        memcpy(previous_state, msg.board, sizeof(msg.board));       
/*         if ((strncmp(msg.buf, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        } */
    }
    close(s);
    exit(EXIT_SUCCESS);
}

void resetPreviousState(){
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
    printf("> ");
    scanf("%s", buffer);
    for (int i = 0; i < 9; i++)
    {
        if(strcmp(buffer, actionTypes[i]) == 0) {
            return i;
        }
    }
    return -1;
}       

void commandParse(struct action *msg) {

    int valid_command;

    do {
        valid_command = 1;

        // USER PROMPT...............................................................
        msg->type = detectType();
        if (msg->type == 1 || msg->type == 2 || msg->type == 3) {
            scanf("%i,%i", &msg->coordinates[0], &msg->coordinates[1]);}

        // COMMAND CHECK.............................................................
        if (msg->type == -1) {
            printf("error: command not found\n");
            valid_command = 0;
        }
        else if (msg->type == 1) {
            if (!((msg->coordinates[0] >= 0 && msg->coordinates[0] <= 3) &&
                (msg->coordinates[1] >= 0 && msg->coordinates[1] <= 3))) {
                printf("error: invalid cell\n");
                valid_command = 0;
            }
            else if (previous_state[msg->coordinates[0]][msg->coordinates[1]] >= 0) {
                printf("error: cell already revealed\n");
                valid_command = 0;
            }       
        }
        else if (msg->type == 2) {
            if (previous_state[msg->coordinates[0]][msg->coordinates[1]] == -3) {
                printf("error: cell already has a flag\n");
                valid_command = 0;
            } else if (previous_state[msg->coordinates[0]][msg->coordinates[1]] >= 0) {
                printf("error: cannot insert flag in revealed cell\n");
                valid_command = 0;
            }
        }
    } while (valid_command == 0);
    
/*     do {
        // PROMPT USER.................................................command
        do {
            msg->type = detectType();
            if(msg->type == -1) {printf("error: command not found\n");}
        } while (msg->type == -1);

        // PROMPT USER.............................................coordinates
        if (msg->type == 1 || msg->type == 2 || msg->type == 3) {
            scanf("%i,%i", &msg->coordinates[0], &msg->coordinates[1]);
            
            // CHECK FOR A REVEAL OUTSIDE OF THE RANGE.............................
            if (!((msg->coordinates[0] >= 0 && msg->coordinates[0] <= 3) &&
                (msg->coordinates[1] >= 0 && msg->coordinates[1] <= 3))) {
                printf("error: invalid cell\n");
                valid_command = 0;
            }

            // CHECK FOR A REVEAL IN CELL REVEALED.................................
            else if (msg->type == 1 &&
                previous_state[msg->coordinates[0]][msg->coordinates[1]] >= 0) {
                printf("error: cell already revealed\n");
                valid_command = 0;
            }

            // CHECK FOR A FLAG IN A FLAGGED CELL..................................
            else if (msg->type == 2) {
                if (previous_state[msg->coordinates[0]][msg->coordinates[1]] == -3) {
                    printf("error: cell already has a flag\n");
                    valid_command = 0;
                } else if (previous_state[msg->coordinates[0]][msg->coordinates[1]] >= 0) {
                    printf("error: cannot insert flag in revealed cell\n");
                    valid_command = 0;
                }
            }
        }

    } while (valid_command == 0); */
    
}

void printBoard(struct action msg) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            switch (msg.board[i][j]) {
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
                    printf("%i", msg.board[i][j]);
                    break;
            }
            if(j != 3) {
                printf("\t\t");
            }
        }
        printf("\n");
    }
}

void actionResultParse(struct action *msg) {
    switch (msg->type) {
        // SERVER....................state
        case 3:
            printBoard(*msg);
            break;

        // SERVER....................win
        case 6:
            printf("YOU WIN!\n");
            printBoard(*msg);
            break;
        
        // SERVER....................game_over
        case 8:
            printf("GAME OVER!\n");
            printBoard(*msg);
            break;
    }
/* 
    if ((strncmp(msg->buf, "exit", 4)) == 0) {
        printf("Client Exit...\n");
        break;
    } */
}

