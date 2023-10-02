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

    //char buf[BUFSZ];
    struct action msg;
    while (1) {
        //bzero(buf, sizeof(buf));
        bzero(&msg, sizeof(msg));
        printf("Enter the string : ");
        //fgets(buf, BUFSZ - 1, stdin);
        fgets(msg.buf, BUFSZ - 1, stdin);
        //send(s, buf, sizeof(buf), 0);
        send(s, &msg, sizeof(msg), 0);

        //bzero(buf, sizeof(buf));
        bzero(&msg, sizeof(msg));
        //recv(s, buf, sizeof(buf), 0);
        recv(s, &msg, sizeof(msg), 0);
        //printf("From Server : %s", buf);
        printf("From Server: %s", msg.buf);
        //if ((strncmp(buf, "exit", 4)) == 0) {
        if ((strncmp(msg.buf, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
    close(s);

    exit(EXIT_SUCCESS);
}