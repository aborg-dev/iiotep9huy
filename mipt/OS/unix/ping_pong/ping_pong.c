/*
 * =====================================================================================
 *
 *       Filename:  ping_pong.c
 *
 *    Description:  Ping-pong game between processes.
 *
 *        Version:  1.0
 *        Created:  03/23/2012 09:19:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

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

#define REPORT(message) fprintf(stderr, "%d: %s\n", getpid(), message);

const size_t MESSAGE_MAXLEN = 20;
const size_t SEMAPHORE_NAME_LEN = 15;

const char* FIFO_NAME       = "/tmp/ping_pong_fifo";
const char* DELETE_SEM_NAME = "delete_semaphore";
const char* FIFO_SEM_NAME   = "fifo_semaphore";

typedef struct
{
  sem_t* sem;
  char* name;
} named_semaphore;

void introduction(named_semaphore* my_sem,
                  named_semaphore* partner_sem,
                  sem_t* file_sem, 
                  int* fd,
                  char** buf)
{
  REPORT("I'm waiting for file");

  sem_wait(file_sem);
  *fd = mkfifo(FIFO_NAME, 0777);
  sem_post(file_sem);

  if (*fd == 0)
  {
    *fd = open(FIFO_NAME, O_RDWR);

    sem_wait(file_sem);
    REPORT("I created file");
    write(*fd, my_sem->name, SEMAPHORE_NAME_LEN); 
    sem_post(file_sem);

    REPORT("I'm waiting for my_sem");
    sem_wait(my_sem->sem);

    read(*fd, partner_sem->name, SEMAPHORE_NAME_LEN);
    partner_sem->sem = sem_open(partner_sem->name, 0);
    memset(*buf, 0, sizeof(*buf));
    sprintf(*buf, "Hi, My name is %d", getpid());
    write(*fd, *buf, MESSAGE_MAXLEN);
    REPORT("Message was written");

    sem_post(partner_sem->sem);
  }
  else
  {
    *fd = open(FIFO_NAME, O_RDWR);
    //fprintf(stderr, "I am here2 %d\n", *fd);
    sem_wait(file_sem);
    REPORT("I've opened file!");
    read(*fd, partner_sem->name, SEMAPHORE_NAME_LEN);
    REPORT(partner_sem->name)
    partner_sem->sem = sem_open(partner_sem->name, 0);
    write(*fd, my_sem->name, SEMAPHORE_NAME_LEN);
    sem_post(file_sem);
    sem_post(partner_sem->sem);
  }
}

void converse(sem_t* my_sem, 
              sem_t* partner_sem,
              int fd,
              char** buf)
{
    fprintf(stderr, "\n");
    sem_wait(my_sem);
    memset(*buf, 0, sizeof(*buf));
    read(fd, *buf, MESSAGE_MAXLEN);
    REPORT("I've got message: ");
    REPORT(*buf);
    memset(*buf, 0, sizeof(*buf));
    sprintf(*buf, "Hi, my name is %d", getpid());
    write(fd, *buf, MESSAGE_MAXLEN);
    sem_post(partner_sem);
    sleep(5);
}

int main(int argc, char** argv)
{
  named_semaphore my_sem;
  named_semaphore partner_sem;
  my_sem.name = (char*) malloc(SEMAPHORE_NAME_LEN);
  partner_sem.name = (char*) malloc(SEMAPHORE_NAME_LEN);
  char* buf = (char*) malloc(MESSAGE_MAXLEN);
  sprintf(my_sem.name, "ping_pong_%d", getpid());

  my_sem.sem      = sem_open(my_sem.name, O_CREAT, 0777, 0);
  partner_sem.sem = NULL;
  sem_t* file_sem = sem_open(FIFO_SEM_NAME, O_CREAT, 0777, 1);
  int fd = 0;

  introduction(&my_sem, &partner_sem, file_sem, &fd, &buf);

  int count = 3;
  REPORT("Partner semaphore");
  REPORT(partner_sem.name);
  REPORT("I'm near inf while");
  while (count--)
  {
    converse(my_sem.sem, 
             partner_sem.sem, 
             fd, &buf);
  }
  //int sem_value = 0;
  //sem_getvalue(file_sem, &sem_value);
  //fprintf(stderr, "Sem value: %d\n", sem_value);
  REPORT("Deleting stuff");
  //free(buf);
  free(my_sem.name);
  free(partner_sem.name);
  sem_unlink(my_sem.name);
  sem_unlink(FIFO_SEM_NAME);
  unlink(FIFO_NAME);
  REPORT("process is finished");
  return 0;
}
