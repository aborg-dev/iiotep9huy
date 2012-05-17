/*
 * =====================================================================================
 *
 *       Filename:  processes_impl.c
 *
 *    Description:  Parallel entity implementation based on: parallel_entity = process, 
 *                                                           data_channel = named pipe
 *                                                           synchronization = named semaphore
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
#include "./libs/named_sem.h"

#define REPORT(message) fprintf(stderr, "%d: %s\n", getpid(), message);
#define REPORT2(s1, s2) fprintf(stderr, "%d: %s %s\n", getpid(), s1, s2);

const size_t ITERATIONS = 4;

const char* FIFO_NAME       = "/tmp/ping_pong_fifo";
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
  string name;
};

struct data_channel_creation_params
{
  string name;
  mode_t mode;
  int oflag;
};

struct data_channel_opening_params
{
  string name;
  int oflag;
};

struct data_channel* create_data_channel(struct data_channel_creation_params*
                                         fifo_params)
{
  struct data_channel* dtch = (struct data_channel*) malloc(sizeof(struct data_channel));

  init_string(&dtch->name);
  print_to_string(&dtch->name, fifo_params->name.content);

  mkfifo(dtch->name.content, fifo_params->mode); 
  dtch->fd = open(dtch->name.content, fifo_params->oflag);
  return dtch;
}

struct data_channel* open_data_channel(struct data_channel_opening_params*
                                fifo_params)
{
  struct data_channel* dtch = (struct data_channel*) malloc(sizeof(struct data_channel));

  init_string(&dtch->name);
  print_to_string(&dtch->name, fifo_params->name.content);

  dtch->fd = -1;
  while (dtch->fd < 0)
  {
    dtch->fd = open(dtch->name.content, fifo_params->oflag); 
    sleep(1);
  }
  return dtch;
}

void delete_data_channel(struct data_channel* dtch)
{
  close(dtch->fd);
  unlink(dtch->name.content);
}

void send_message(const struct message* msg, struct data_channel* dtch)
{
  print_string(dtch->fd, &msg->greeting);
}

void recive_message(struct message* msg, struct data_channel* dtch)
{
  read_to_string(dtch->fd, &msg->greeting);
}

// --------------data_channel-----------------


// --------------parallel_entity-----------------

struct execution_params
{
  struct data_channel* dtch;
  struct synchronization_object* my_turn_sync;
  struct synchronization_object* partner_turn_sync;
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
  delete_synchronization_object(exec_params->partner_turn_sync);
  delete_synchronization_object(exec_params->my_turn_sync);
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


// --------------synchronization_object-----------------

struct synchronization_object
{
  named_sem* sem;
};

struct synchronization_object_creation_params
{
  string name;
  int init_value;
};

struct synchronization_object* create_synchronization_object(
  struct synchronization_object_creation_params* params)
{
  struct synchronization_object* ent = 
    (struct synchronization_object*) malloc(sizeof(struct synchronization_object));

  
  ent->sem = create_semaphore(O_CREAT, 0777);
  print_to_string(&(ent->sem->name), params->name.content);
  open_named_sem(ent->sem, params->init_value);
  return ent;
}

void delete_synchronization_object(struct synchronization_object* obj)
{
  unlink_semaphore(obj->sem);
}

void enter_critical_section(struct synchronization_object* obj)
{
  sem_wait(obj->sem->sem);
}

void exit_from_critical_section(struct synchronization_object* obj)
{
  sem_post(obj->sem->sem);
}

// --------------synchronization_object-----------------

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
    exit_from_critical_section(exec_params->partner_turn_sync);
  }

  for(int iteration = start_iteration; 
          iteration < finish_iteration; ++iteration)
  {
    enter_critical_section(exec_params->my_turn_sync);

    recive_message(&msg, exec_params->dtch);
    printf("%d recived: '%s'\n", player_num, msg.greeting.content);

    print_to_string(&msg.greeting, "%d: Hello %d", getpid(), iteration);
    send_message(&msg, exec_params->dtch);

    exit_from_critical_section(exec_params->partner_turn_sync);
    sleep(2);
  }

  free_string(&msg.greeting);
  REPORT("process execution finished");
  return 0;
}

struct data_channel* create_game_data_channel()
{
  struct data_channel_creation_params data_channel_params;
  init_string(&data_channel_params.name);
  print_to_string(&data_channel_params.name, FIFO_NAME);
  data_channel_params.mode = 0777;
  data_channel_params.oflag = O_RDWR | O_NONBLOCK;
  return create_data_channel(&data_channel_params);
}

struct data_channel* open_game_data_channel()
{
  struct data_channel_opening_params data_channel_params;
  init_string(&data_channel_params.name);
  print_to_string(&data_channel_params.name, FIFO_NAME);
  data_channel_params.oflag = O_RDWR | O_NONBLOCK;
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

struct synchronization_object* 
  create_game_synchronization_object(int player_num)
{
  struct synchronization_object_creation_params sync_params;
  init_string(&sync_params.name);
  print_to_string(&sync_params.name, "%s_%d", SEM_NAME_PREFIX, player_num);
  sync_params.init_value = 0;
  return create_synchronization_object(&sync_params);
}

struct execution_params* create_game_execution_params(int player_num)
{
  struct execution_params* exec_params = create_execution_params();
  exec_params->player_num = player_num;
  exec_params->dtch = get_game_data_channel(player_num);

  exec_params->my_turn_sync = 
    create_game_synchronization_object(player_num);

  int partner_num = (player_num == 1) ? 2 : 1;
  exec_params->partner_turn_sync = 
    create_game_synchronization_object(partner_num);
  
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
