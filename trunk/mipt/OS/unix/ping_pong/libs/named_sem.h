/*
 * =====================================================================================
 *
 *       Filename:  named_sem.h
 *
 *    Description:  Library for work with named semaphores
 *
 *        Version:  1.0
 *        Created:  05/06/12 19:27:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef NAMED_SEM_H
#define NAMED_SEM_H 

#include <sys/types.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>

#include "string.h"

typedef struct
{
  sem_t* sem;
  string name;
  int oflag;
  mode_t mode;
} named_sem;

named_sem* create_semaphore(int oflag, mode_t mode);
void init_semaphore(named_sem* sem, int oflag, mode_t mode);
void free_semaphore(named_sem* sem);
void unlink_semaphore(named_sem* sem);
bool open_named_sem(named_sem* sem, int init_value);

#endif /* NAMED_SEM_H */
