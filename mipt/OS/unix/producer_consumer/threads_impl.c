/*
 * =====================================================================================
 *
 *       Filename:  threads_impl.c
 *
 *    Description:  Parallel entity implementation based on: parallel_entity = thread, 
 *                                                           data_channel = shared memory 
 *                                                           synchronization = mutex
 *
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
#include <fcntl.h>
#include <memory.h>
#include <signal.h>
#include <stdarg.h>
#include <pthread.h>

#include "parallel_entity.h"
#include "./libs/string.h"

#define REPORT(msg) fprintf(stderr, "%zd: %s\n", pthread_self(), msg);

#define STOCK_SIZE 50
#define ITERATIONS 3
#define PRODUCERS_NUMBER 5
#define CONSUMERS_NUMBER 5

// --------------product-----------------

struct product
{
  size_t id;
  string product_name;
};

struct product_info
{
  string product_name;
};

struct product* create_product(int id, char* product_name)
{
  struct product* item = (struct product*) malloc(sizeof(struct product));
  init_string(&item->product_name);
  item->id = id;
  print_to_string(&item->product_name, product_name);
  return item;
}

void delete_product(struct product* item)
{
  free_string(&item->product_name);
  free(item);
}

// --------------product-----------------


// --------------storage-----------------

struct storage
{
  struct product** products;
  struct synchronization_object* storage_is_used; 
  size_t storage_size;
  size_t storage_available;
};

struct storage_creation_params
{
  size_t storage_size;
};

struct storage* create_storage(struct storage_creation_params*
                                         storage_params)
{
  struct storage* stock = (struct storage*) malloc(sizeof(struct storage));
  stock->storage_size = stock->storage_available = storage_params->storage_size;
  stock->products = (struct product**) malloc(sizeof(struct product*) * stock->storage_size);
  stock->storage_is_used = create_synchronization_object(NULL);
  return stock;
}

void delete_storage(struct storage* stock)
{
  for(int i = 0; i < stock->storage_size; ++i)
    delete_product(stock->products[i]);
  free(stock->products);
  delete_synchronization_object(stock->storage_is_used);
  free(stock);
}

void add_item(struct product* item, struct storage* stock)
{
  printf("Adding item %zd %s\n", item->id, item->product_name.content);
  while (1)
  {
    enter_critical_section(stock->storage_is_used);
    
    if (stock->storage_available > 0)
    {
      size_t put_position = stock->storage_size - stock->storage_available;
      stock->products[put_position] = item;
      stock->storage_available--;
      exit_from_critical_section(stock->storage_is_used);
      return;
    }
    
    exit_from_critical_section(stock->storage_is_used);
    sleep(3);
  }
}

int find_item_index(struct product_info* item_info, struct storage* stock)
{
  for(int i = 0; i < (int)(stock->storage_size) - (int)(stock->storage_available); ++i)
  {
    if (equal_string(&stock->products[i]->product_name, &item_info->product_name))
      return i;
  }
  return -1;
}

struct product* get_item_by_index(size_t index, struct storage* stock)
{
  return stock->products[index];
}

struct product* give_item_by_index(size_t index, struct storage* stock)
{
  struct product* item = stock->products[index];
  stock->products[index] = NULL;
  for(int i = index; i < (int)(stock->storage_size) - (int)(stock->storage_available); ++i)
  {
    stock->products[i] = stock->products[i + 1];
  }
  stock->storage_available++;
  return item;
}

struct product* get_item(struct product_info* item_info, struct storage* stock)
{
  printf("Getting item %s\n", item_info->product_name.content);
  while (1)
  {
    enter_critical_section(stock->storage_is_used);
    int item_index = find_item_index(item_info, stock);
    if (item_index != -1)
    {
      struct product* item = give_item_by_index(item_index, stock);
      exit_from_critical_section(stock->storage_is_used);
      return item;
    }
    exit_from_critical_section(stock->storage_is_used);
    sleep(3);
  }
}

// --------------storage-----------------


// --------------parallel_entity-----------------

struct execution_params
{
  struct storage* stock;
};

struct parallel_entity
{
  pthread_t thread;
  void* (*parallel_function)(void*);
};

struct parallel_entity_creation_params
{
  int player_type;
};

void* producer_function(void* params);
void* consumer_function(void* params);

struct parallel_entity* create_parallel_entity(struct parallel_entity_creation_params* params) 
{
  struct parallel_entity* ent = 
    (struct parallel_entity*) malloc(sizeof(struct parallel_entity));
  if (params->player_type == 1)
  {
    ent->parallel_function = producer_function;
  }
  else
  if (params->player_type == 2)
  {
    ent->parallel_function = consumer_function;
  }
  else
    fprintf(stderr, "Wrong entity type\n");
  return ent;
}

void delete_parallel_entity(struct parallel_entity* ent)
{
  free(ent);
}

void start_parallel_execution(struct parallel_entity* ent,
                              struct execution_params* params)
{
  pthread_create(&(ent->thread), NULL, ent->parallel_function, (void*) params);
}

void wait_parallel_entity(struct parallel_entity* ent)
{
  pthread_join(ent->thread, NULL);
}

// --------------parallel_entity-----------------


// --------------synchronization_object-----------------

struct synchronization_object
{
  pthread_mutex_t mutex;
};

struct synchronization_object* create_synchronization_object(
  struct synchronization_object_creation_params* params)
{
  struct synchronization_object* obj = 
    (struct synchronization_object*) malloc(sizeof(struct synchronization_object));

  pthread_mutex_init(&(obj->mutex), NULL);
  return obj;
}

void delete_synchronization_object(struct synchronization_object* obj)
{
  pthread_mutex_destroy(&(obj->mutex));
  free(obj);
}

void enter_critical_section(struct synchronization_object* obj)
{
  pthread_mutex_lock(&(obj->mutex));
}

void exit_from_critical_section(struct synchronization_object* obj)
{
  pthread_mutex_unlock(&(obj->mutex));
}

// --------------synchronization_object-----------------

void* producer_function(void* params)
{
  REPORT("producer execution started");
  struct execution_params* exec_params = (struct execution_params*) params; 
  struct storage* stock = exec_params->stock;
  for(int i = 0; i < ITERATIONS; ++i)
  {
    struct product* item = create_product(i, "Banana");
    add_item(item, stock);
    printf("I've added %s to stock!\n", item->product_name.content);
    sleep(2);
  }
  REPORT("producer execution finised");
  return 0;
}

void* consumer_function(void* params)
{
  REPORT("consumer execution started");
  struct execution_params* exec_params = (struct execution_params*) params; 
  struct storage* stock = exec_params->stock;
  struct product_info item_info;
  init_string(&item_info.product_name);
  print_to_string(&item_info.product_name, "Banana");
  for(int i = 0; i < ITERATIONS; ++i)
  {
    struct product* item = get_item(&item_info, stock);
    printf("I've got %s from stock!\n", item->product_name.content);
    delete_product(item);
    sleep(3);
  }
  REPORT("consumer execution finished");
  return 0;
}

struct parallel_entity* create_game_parallel_entity(int player_type)
{
  struct parallel_entity_creation_params params;
  params.player_type = player_type;
  struct parallel_entity* player = create_parallel_entity(&params);
  return player;
}

struct storage* create_game_storage()
{
  struct storage_creation_params storage_params;
  storage_params.storage_size = STOCK_SIZE;
  struct storage* stock = create_storage(&storage_params);
  return stock;
}

struct parallel_entity** create_multiple_parallel_entities(int player_type, int number)
{
  struct parallel_entity** entities = (struct parallel_entity**) malloc(sizeof(struct parallel_entity*) * number);
  for(int i = 0; i < number; ++i)
    entities[i] = create_game_parallel_entity(player_type);
  return entities;
}

void start_multiple_parallel_execution(struct parallel_entity** entities, int number, struct execution_params* params)
{
  for(int i = 0; i < number; i++)
      start_parallel_execution(entities[i], params);
}

void wait_multiple_parallel_entities(struct parallel_entity** entities, int number)
{
  for(int i = 0; i < number; ++i)
    wait_parallel_entity(entities[i]);
}

void delete_multiple_parallel_entities(struct parallel_entity** entities, int number)
{
  for(int i = 0; i < number; ++i)
    delete_parallel_entity(entities[i]);
}

int main(int argc, char** argv)
{
  
  struct storage* stock = create_game_storage();
  struct execution_params exec_params;
  exec_params.stock = stock;
 
  struct parallel_entity** producers = create_multiple_parallel_entities(1, PRODUCERS_NUMBER);
  struct parallel_entity** consumers = create_multiple_parallel_entities(2, CONSUMERS_NUMBER);

  start_multiple_parallel_execution(producers, PRODUCERS_NUMBER, &exec_params);
  start_multiple_parallel_execution(consumers, CONSUMERS_NUMBER, &exec_params);

  wait_multiple_parallel_entities(producers, PRODUCERS_NUMBER);
  wait_multiple_parallel_entities(consumers, CONSUMERS_NUMBER);

  delete_multiple_parallel_entities(producers, PRODUCERS_NUMBER);
  delete_multiple_parallel_entities(consumers, CONSUMERS_NUMBER);

  delete_storage(stock);
  return 0;
}
