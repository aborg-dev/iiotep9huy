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
const char* FIFO_SEM_NAME   = "fifo_semaphore";
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

  init_string(&(dtch->name));
  print_to_string(&(dtch->name), fifo_params->name.content);

  mkfifo(dtch->name.content, fifo_params->mode); 
  dtch->fd = open(dtch->name.content, fifo_params->oflag);
  return dtch;
}

struct data_channel* open_data_channel(struct data_channel_opening_params*
                                fifo_params)
{
  struct data_channel* dtch = (struct data_channel*) malloc(sizeof(struct data_channel));

  init_string(&dtch->name);
  dtch->fd = -1;
  print_to_string(&dtch->name, fifo_params->name.content);

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
  print_string(dtch->fd, &(msg->greeting));
}

void recive_message(struct message* msg, struct data_channel* dtch)
{
  read_to_string(dtch->fd, &(msg->greeting));
}

// --------------data_channel-----------------


// --------------parallel_entity-----------------

struct execution_params
{
  struct data_channel* dtch;
  struct synchronization_object* my_turn_sync;
  struct synchronization_object* partner_turn_sync;
};

struct parallel_entity
{
  string name;
  pid_t pid;
  void* (*parallel_function)(void*);
};

struct parallel_entity_creation_params
{
  string name;
};

struct parallel_entity* create_parallel_entity(
  const struct parallel_entity_creation_params* creation_params) 
{
  struct parallel_entity* ent = 
    (struct parallel_entity*) malloc(sizeof(struct parallel_entity));

  init_string(&(ent->name));
  ent->name = creation_params->name;
  ent->pid = getpid();
  return ent;
}

void delete_parallel_entity(struct parallel_entity* ent)
{
  free_string(&(ent->name));
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

void* first_process_function(void* params)
{
  REPORT("First process execution started");
  struct execution_params* exec_params = (struct execution_params*) params; 
  struct message msg;
  init_string(&msg.greeting);
  print_to_string(&msg.greeting, "%d: Hello %d", getpid(), 0);
  send_message(&msg, exec_params->dtch);
  exit_from_critical_section(exec_params->partner_turn_sync);

  for(int iteration = 1; iteration < ITERATIONS + 1; ++iteration)
  {
    enter_critical_section(exec_params->my_turn_sync);

    recive_message(&msg, exec_params->dtch);
    printf("first  recived: '%s'\n", msg.greeting.content);

    print_to_string(&msg.greeting, "%d: Hello %d", getpid(), iteration);
    send_message(&msg, exec_params->dtch);

    exit_from_critical_section(exec_params->partner_turn_sync);
    sleep(2);
  }

  free_string(&msg.greeting);
  return 0;
}

void* second_process_function(void* params)
{
  REPORT("Second process execution started");
  struct execution_params* exec_params = (struct execution_params*) params; 
  struct message msg;
  init_string(&msg.greeting);

  for(int iteration = 0; iteration < ITERATIONS; ++iteration)
  {
    enter_critical_section(exec_params->my_turn_sync);

    recive_message(&msg, exec_params->dtch);
    printf("second recived: '%s'\n", msg.greeting.content);

    print_to_string(&msg.greeting, "%d: Hello %d", getpid(), iteration);

    send_message(&msg, exec_params->dtch);

    exit_from_critical_section(exec_params->partner_turn_sync);
    sleep(2);
  }

  free_string(&msg.greeting);
  return 0;
}

struct execution_params* create_execution_params(int playerNum)
{
  struct execution_params* exec_params = 
    (struct execution_params*) malloc(sizeof(struct execution_params));

  struct synchronization_object_creation_params my_sync_params;
  init_string(&my_sync_params.name);
  print_to_string(&my_sync_params.name, 
                  "%s_%d", SEM_NAME_PREFIX, playerNum);

  struct synchronization_object_creation_params partner_sync_params;
  init_string(&partner_sync_params.name);
  print_to_string(&partner_sync_params.name, 
                  "%s_%d", SEM_NAME_PREFIX, (playerNum == 1) ? 2 : 1);

  my_sync_params.init_value = 0;
  partner_sync_params.init_value = 0;

  exec_params->my_turn_sync = 
    create_synchronization_object(&my_sync_params);

  exec_params->partner_turn_sync = 
    create_synchronization_object(&partner_sync_params);
  
  return exec_params;
}

void delete_execution_params(struct execution_params* exec_params)
{
  delete_data_channel(exec_params->dtch);
  delete_synchronization_object(exec_params->partner_turn_sync);
  delete_synchronization_object(exec_params->my_turn_sync);
}

int main(int argc, char** argv)
{
  int playerNum = 0;
  if (argc == 2)
  {
    if (argv[1][0] == '1')
      playerNum = 1;
    else
    if (argv[1][0] == '2')
      playerNum = 2;
  }
  if (playerNum == 0)
    return 0;

  struct parallel_entity_creation_params creation_params;
  init_string(&creation_params.name);


  struct parallel_entity* player;
  struct execution_params* exec_params = 
    create_execution_params(playerNum);

  switch (playerNum)
  {
    case 1:
    {
      struct data_channel_creation_params data_channel_params;
      init_string(&data_channel_params.name);
      print_to_string(&data_channel_params.name, FIFO_NAME);
      data_channel_params.mode = 0777;
      data_channel_params.oflag = O_RDWR | O_NONBLOCK;
      exec_params->dtch = create_data_channel(&data_channel_params);

      print_to_string(&creation_params.name, "First process"); 
      player = create_parallel_entity(&creation_params);
      player->parallel_function = first_process_function;
      break;
    }
    case 2:
    {
      struct data_channel_opening_params data_channel_params;
      init_string(&data_channel_params.name);
      print_to_string(&data_channel_params.name, FIFO_NAME);
      data_channel_params.oflag = O_RDWR | O_NONBLOCK;
      exec_params->dtch = open_data_channel(&data_channel_params);

      print_to_string(&creation_params.name, "Second process"); 
      player = create_parallel_entity(&creation_params);
      player->parallel_function = second_process_function;
      break;
    }
  }
 
  start_parallel_execution(player, exec_params);

  delete_execution_params(exec_params);
  delete_parallel_entity(player);
  REPORT("Process is finished");
  return 0;
}
