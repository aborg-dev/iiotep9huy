/*
 * =====================================================================================
 *
 *       Filename:  parallel_entity.h
 *
 *    Description:  Header for parallel entities
 *
 *        Version:  1.0
 *        Created:  05/06/12 20:39:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef PARALLEL_ENTITY_H
#define PARALLEL_ENTITY_H 

// --------------message-----------------

struct message;

// --------------message-----------------


// --------------data_channel-----------------

struct data_channel;
struct data_channel_creation_params;
struct data_channel_opening_params;

struct data_channel* create_data_channel(
  struct data_channel_creation_params*);

struct data_channel* open_data_channel(
  struct data_channel_opening_params*);

void delete_data_channel(struct data_channel* dtch);

void send_message(const struct message* msg, struct data_channel* dtch);
void recive_message(struct message* msg, struct data_channel* dtch);

// --------------data_channel-----------------


// --------------parallel_entity-----------------

struct execution_params;
struct parallel_entity;
struct parallel_entity_creation_params;

struct parallel_entity* create_parallel_entity(
  const struct parallel_entity_creation_params*);

void delete_parallel_entity(struct parallel_entity* ent);

void start_parallel_execution(struct parallel_entity* ent, 
                              struct execution_params* params);

void wait_parallel_entity(struct parallel_entity* ent);

// --------------parallel_entity-----------------


// --------------synchronization_object-----------------

struct synchronization_object;
struct synchronization_object_creation_params;

struct synchronization_object* create_synchronization_object(
  struct synchronization_object_creation_params* params);

void delete_synchronization_object(struct synchronization_object* obj);

void enter_critical_section(struct synchronization_object* obj);
void exit_from_critical_section(struct synchronization_object* obj);

// --------------synchronization_object-----------------

#endif /* PARALLEL_ENTITY_H */
