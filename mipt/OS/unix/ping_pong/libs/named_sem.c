/*
 * =====================================================================================
 *
 *       Filename:  named_sem.c
 *
 *    Description:  Library for named semaphore implementation
 *
 *        Version:  1.0
 *        Created:  05/06/12 19:29:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#include "named_sem.h"

named_sem* create_semaphore(int oflag, mode_t mode)
{
  named_sem* sem = (named_sem*) malloc(sizeof(named_sem));
  init_semaphore(sem, oflag, mode);
  return sem;
}

void init_semaphore(named_sem* sem, int oflag, mode_t mode)
{
  sem->sem = NULL;
  init_string(&(sem->name));
  sem->oflag = oflag;
  sem->mode = mode;
}

void free_semaphore(named_sem* sem)
{
  if (sem->sem != NULL)
  {
    sem_close(sem->sem);
    sem->sem = NULL;
  }
  free_string(&(sem->name));
}

void unlink_semaphore(named_sem* sem)
{
  if (sem->sem != NULL)
  {
    sem_unlink(sem->name.content);
  }
  free_semaphore(sem);
}

int open_named_sem(named_sem* sem, int init_value)
{
  sem->sem = sem_open(sem->name.content, sem->oflag, sem->mode, init_value);
  return (sem->sem == SEM_FAILED);
}

