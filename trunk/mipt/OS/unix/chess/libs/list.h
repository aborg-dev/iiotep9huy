/*
 * =====================================================================================
 *
 *       Filename:  list.h
 *
 *    Description:  List library definition
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
#ifndef LIST_H
#define LIST_H 

struct list_node
{
  void* data;
  struct list_node* prev_node;
  struct list_node* next_node;
};

typedef struct list_node list_node;

// begin is barrier element
typedef struct 
{
  list_node* begin;
  list_node* end;
  size_t size;
} list;

void list_init(list* lst);
void list_free(list* lst);

list_node* list_begin(list* lst);
list_node* list_end(list* lst);
list_node* list_before_begin(list* lst);

void list_push_back(list* lst, void* element);
void list_erase(list* lst, list_node* node);

#endif /* LIST_H */
