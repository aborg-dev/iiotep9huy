/*
 * =====================================================================================
 *
 *       Filename:  processes_impl.c
 *
 *    Description:  Parallel entity implementation based on: parallel_entity = process, 
 *                                                           data_channel = socket,
 *                                                           synchronization = socket
 *
 *        Version:  1.0
 *        Created:  05/09/12 14:47:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <memory.h>
#include <signal.h>
#include <stdarg.h>

#include "parallel_entity.h"
#include "./libs/string.h"

#define REPORT(message) fprintf(stderr, "%d: %s\n", getpid(), message);
#define REPORT2(s1, s2) fprintf(stderr, "%d: %s %s\n", getpid(), s1, s2);
#define DOMAIN AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define SIN_FAMILY AF_INET
#define SIN_PORT 5555
#define SIN_ADDR INADDR_LOOPBACK

const size_t ITERATIONS = 4;

const char* SEM_NAME_PREFIX = "ping_pong";

// --------------message-----------------

struct message
{
  string greeting;
};

// --------------message-----------------


// --------------data_channel-----------------

struct data_channel
{
  int fd;
};

struct data_channel_creation_params
{
  int domain;
  int type;
  int protocol;
  struct sockaddr_in addr;
};

struct data_channel_opening_params
{
  int domain;
  int type;
  int protocol;
  struct sockaddr_in addr;
};

struct data_channel* create_data_channel(struct data_channel_creation_params*
                                         socket_params)
{
  struct data_channel* dtch = (struct data_channel*) malloc(sizeof(struct data_channel));

  int listener = socket(socket_params->domain, 
                        socket_params->type, 
                        socket_params->protocol);

  bind(listener, (struct sockaddr*) &socket_params->addr, 
                              sizeof(socket_params->addr));
  
  listen(listener, 1);
  dtch->fd = accept(listener, NULL, NULL);
  return dtch;
}

struct data_channel* open_data_channel(struct data_channel_opening_params*
                                socket_params)
{
  struct data_channel* dtch = (struct data_channel*) malloc(sizeof(struct data_channel));

  dtch->fd = socket(socket_params->domain, 
                    socket_params->type, 
                    socket_params->protocol);

  connect(dtch->fd, (struct sockaddr*) &socket_params->addr, 
                                 sizeof(socket_params->addr));

  return dtch;
}

void delete_data_channel(struct data_channel* dtch)
{
  close(dtch->fd);
}

void send_message(const struct message* msg, struct data_channel* dtch)
{
  send_string(dtch->fd, &msg->greeting);
}

void recive_message(struct message* msg, struct data_channel* dtch)
{
  recv_to_string(dtch->fd, &msg->greeting);
}

// --------------data_channel-----------------


// --------------parallel_entity-----------------

struct execution_params
{
  struct data_channel* dtch;
  int player_num;
};

struct execution_params* create_execution_params()
{
  struct execution_params* exec_params = 
    (struct execution_params*) malloc(sizeof(struct execution_params));
  return exec_params;
}

void delete_execution_params(struct execution_params* exec_params)
{
  delete_data_channel(exec_params->dtch);
}

struct parallel_entity
{
  pid_t pid;
  void* (*parallel_function)(void*);
};

struct parallel_entity* create_parallel_entity() 
{
  struct parallel_entity* ent = 
    (struct parallel_entity*) malloc(sizeof(struct parallel_entity));

  ent->pid = getpid();
  return ent;
}

void delete_parallel_entity(struct parallel_entity* ent)
{
}

void start_parallel_execution(struct parallel_entity* ent,
                              struct execution_params* params)
{
  ent->parallel_function((void*) params);
}

void wait_parallel_entity(struct parallel_entity* ent)
{
  while (kill(ent->pid, 0) != -1)
  {
    sleep(1);
  }
}

// --------------parallel_entity-----------------


void* process_function(void* params)
{
  REPORT("process execution started");
  struct execution_params* exec_params = (struct execution_params*) params; 
  int player_num = exec_params->player_num;
  struct message msg;
  init_string(&msg.greeting);

  int start_iteration = 0;
  int finish_iteration = ITERATIONS;
  if (player_num == 1)
  {
    start_iteration++;
    finish_iteration++;
    print_to_string(&msg.greeting, "%d: Hello %d", getpid(), 0);
    send_message(&msg, exec_params->dtch);
  }

  for(int iteration = start_iteration; 
          iteration < finish_iteration; ++iteration)
  {

    recive_message(&msg, exec_params->dtch);
    printf("%d recived: '%s'\n", player_num, msg.greeting.content);

    print_to_string(&msg.greeting, "%d: Hello %d", getpid(), iteration);
    send_message(&msg, exec_params->dtch);

    sleep(2);
  }

  free_string(&msg.greeting);
  REPORT("process execution finished");
  return 0;
}

struct data_channel* create_game_data_channel()
{
  struct data_channel_creation_params data_channel_params;
  data_channel_params.domain = DOMAIN;
  data_channel_params.type = SOCKET_TYPE;
  data_channel_params.protocol = 0;
  data_channel_params.addr.sin_family = SIN_FAMILY;
  data_channel_params.addr.sin_port = htons(SIN_PORT);
  data_channel_params.addr.sin_addr.s_addr = htonl(SIN_ADDR);
  return create_data_channel(&data_channel_params);
}

struct data_channel* open_game_data_channel()
{
  struct data_channel_opening_params data_channel_params;
  data_channel_params.domain = DOMAIN;
  data_channel_params.type = SOCKET_TYPE;
  data_channel_params.protocol = 0;
  data_channel_params.addr.sin_family = SIN_FAMILY;
  data_channel_params.addr.sin_port = htons(SIN_PORT);
  data_channel_params.addr.sin_addr.s_addr = htonl(SIN_ADDR);
  return open_data_channel(&data_channel_params);
}

struct data_channel* get_game_data_channel(int player_num)
{
  switch (player_num)
  {
    case 1:
      return create_game_data_channel();
    case 2:
      return open_game_data_channel();
  }
  printf("Wrong player num: %d\n", player_num);
  return NULL;
}

struct execution_params* create_game_execution_params(int player_num)
{
  struct execution_params* exec_params = create_execution_params();
  exec_params->player_num = player_num;
  exec_params->dtch = get_game_data_channel(player_num);
  return exec_params;
}

struct parallel_entity* create_game_parallel_entity(int player_num)
{
  struct parallel_entity* player = create_parallel_entity();
  player->parallel_function = process_function;
  return player;
}


int main(int argc, char** argv)
{
  int player_num = 0;
  if (argc == 2)
    player_num = argv[1][0] - '0';

  if (player_num != 1 && player_num != 2)
    return 0;

  struct parallel_entity* player 
    = create_game_parallel_entity(player_num);

  struct execution_params* exec_params 
    = create_game_execution_params(player_num);
 
  start_parallel_execution(player, exec_params);

  delete_execution_params(exec_params);
  delete_parallel_entity(player);
  return 0;
}
