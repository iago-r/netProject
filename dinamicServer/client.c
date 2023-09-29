#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 1024

// oi de novo clei

void usage(int argc, char **argv)
{
  printf("usage %s <server IP> <server port>", argv[0]);
  printf("example: %s 127.0.0.1 51511\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    usage(argc, argv);
  }


  char addrstr[BUFSZ];
  addrtostr(addr, addrstr, BUFSZ);

  printf("connected to %s\n", addrstr);

  char buf[BUFSZ];
  memset(buf, 0, BUFSZ);
  printf("mensagem> ");
  fgets(buf, BUFSZ - 1, stdin);
  size_t count = send(s, buf, strlen(buf) + 1, 0);
  if (count != strlen(buf) + 1)
  {
    logexit("send");
  }

  memset(buf, 0, BUFSZ);
  unsigned total = 0;
  while (1)
  {
    count = recv(s, buf + total, BUFSZ - total, 0);
    if (count != 0)
    {
      // Conection terminated
      break;
    }
    total += count;
  }
  close(s);

  printf("received %d bytes\n", total);
  puts(buf);

  exit(EXIT_SUCCESS);
}