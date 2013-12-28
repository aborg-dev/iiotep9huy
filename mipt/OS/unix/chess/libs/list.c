/*
 * =====================================================================================
 *
 *       Filename:  list.c
 *
 *    Description:  List library implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <stdbool.h>

#include "list.h"

void list_node_init(list_node* node)
{
  node->data = NULL;
  node->prev_node = NULL;
  node->next_node = NULL;
}

void list_node_free(list_node* node)
{
  free(node->data);
  free(node);
}

void list_init(list* lst)
{
  lst->begin = (list_node*) malloc(sizeof(list_node));
  list_node_init(lst->begin);
  lst->end = (list_node*) malloc(sizeof(list_node));
  list_node_init(lst->end);
  lst->begin->next_node = lst->end;
  lst->end->prev_node = lst->begin;
  lst->size = 0;
}

void list_free(list* lst)
{
  list_node* cur = lst->begin;
  while (cur != lst->end)
  {
    list_node* next = cur->next_node;
    list_node_free(cur);
    cur = next;
  }
  list_node_free(lst->end);
  free(lst);
}

void list_push_back(list* lst, void* data)
{
  list_node* new_node = (list_node*) malloc(sizeof(list_node));
  list_node_init(new_node);
  new_node->data = data;
  new_node->prev_node = lst->end->prev_node;
  new_node->next_node = lst->end;
  lst->end->prev_node->next_node = new_node;
  lst->end->prev_node = new_node;
  lst->size++;
}

void list_erase(list* lst, list_node* node)
{
  if (node == lst->begin || node == lst->end)
    return;
  node->prev_node->next_node = node->next_node;
  node->next_node->prev_node = node->prev_node;
  list_node_free(node);
  lst->size--;
}

list_node* list_before_begin(list* lst)
{
  return lst->begin;
}

list_node* list_begin(list* lst)
{
  return lst->begin->next_node;
}

list_node* list_end(list* lst)
{
  return lst->end;
}

#include "list.h"