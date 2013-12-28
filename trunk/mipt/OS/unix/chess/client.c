/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  Chess client implementation 
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#define _POSIX_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <stdarg.h> 
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#include "parallel_entity.h"
#include "./libs/string.h"
#include "./libs/list.h"

#define REPORT(message) fprintf(stderr, "%d: %s\n", getpid(), message);
#define REPORT2(s1, s2) fprintf(stderr, "%d: %s %s\n", getpid(), s1, s2);
#define DOMAIN AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define SIN_FAMILY AF_INET
#define SIN_PORT 5555
#define SIN_ADDR INADDR_ANY

#define MAX_GAMELEVEL 11

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "Usage: client [client_name] [client_game_level]");
    return 0;
  }
  int sock;
  struct sockaddr_in addr;

  sock = socket(DOMAIN, SOCKET_TYPE, 0);
  if(sock < 0)
  {
    perror("socket");
    exit(1);
  }

  addr.sin_family = SIN_FAMILY;
  addr.sin_port = htons(SIN_PORT);
  addr.sin_addr.s_addr = htonl(SIN_ADDR);

  if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("connect");
    exit(2);
  }

  string name;
  init_string(&name);
  fprintf(stderr, "My name is %s\n", argv[1]);
  print_to_string(&name, "%s", argv[1]);

  string game_level;
  init_string(&game_level);
  print_to_string(&game_level, "%s", argv[2]);

  fprintf(stderr, "Writing name and game_level\n");
  send_string(sock, &name);
  fprintf(stderr, "Name was sent\n");
  send_string(sock, &game_level);
  fprintf(stderr, "Game level was sent");
  close(sock);
  return 0;
}