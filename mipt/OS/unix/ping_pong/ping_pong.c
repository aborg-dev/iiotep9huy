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
#define REPORT2(s1, s2) fprintf(stderr, "%d: %s %s\n", getpid(), s1, s2);

const size_t MESSAGE_MAXLEN     = 20;
const size_t SEMAPHORE_NAME_LEN = 15;
const size_t ITERATIONS         = 3;

const char* FIFO_NAME       = "/tmp/ping_pong_fifo";
const char* DELETE_SEM_NAME = "delete_semaphore";
const char* FIFO_SEM_NAME   = "fifo_semaphore";

typedef struct
{
  sem_t* sem;
  char* name;
} named_sem;

void introduction(named_sem* my_sem,
                  named_sem* partner_sem,
                  sem_t* file_sem, 
                  int* fd,
                  char** buf)
{
  sem_wait(file_sem);
  *fd = mkfifo(FIFO_NAME, 0777);
  sem_post(file_sem);

  if (*fd == 0)
  {
    REPORT("I've created file");
    *fd = open(FIFO_NAME, O_RDWR);

    sem_wait(file_sem);
    write(*fd, my_sem->name, SEMAPHORE_NAME_LEN); 
    sem_post(file_sem);

    sem_wait(my_sem->sem);
    {
      read(*fd, partner_sem->name, SEMAPHORE_NAME_LEN);
      partner_sem->sem = sem_open(partner_sem->name, 0);
      sprintf(*buf, "Hello from %d", getpid());
      write(*fd, *buf, MESSAGE_MAXLEN);
    }
    sem_post(partner_sem->sem);
  }
  else
  {
    *fd = open(FIFO_NAME, O_RDWR);
    REPORT("I've opened file!");

    sem_wait(file_sem);
    {
      read(*fd, partner_sem->name, SEMAPHORE_NAME_LEN);
      partner_sem->sem = sem_open(partner_sem->name, 0);
      write(*fd, my_sem->name, SEMAPHORE_NAME_LEN);
    }
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
    {
      read(fd, *buf, MESSAGE_MAXLEN);

      REPORT2("I've got message: ", *buf);
      sprintf(*buf, "Hello from %d", getpid());

      write(fd, *buf, MESSAGE_MAXLEN);
    }
    sem_post(partner_sem);
    sleep(2);
}

void begin_conversation(int iterations, sem_t** my_sem, sem_t** partner_sem, 
                        int fd, char** buf)
{
  while (iterations--)
  {
    converse(*my_sem, 
             *partner_sem, 
             fd, buf);
  }
}


int main(int argc, char** argv)
{
  named_sem my_sem;
  named_sem partner_sem;
  my_sem.name = (char*) malloc(SEMAPHORE_NAME_LEN);
  partner_sem.name = (char*) malloc(SEMAPHORE_NAME_LEN);
  char* buf = (char*) malloc(MESSAGE_MAXLEN);
  sprintf(my_sem.name, "ping_pong_%d", getpid());

  my_sem.sem      = sem_open(my_sem.name, O_CREAT, 0777, 0);
  partner_sem.sem = NULL;
  sem_t* file_sem = sem_open(FIFO_SEM_NAME, O_CREAT, 0777, 1);
  int fd = 0;

  introduction(&my_sem, &partner_sem, file_sem, &fd, &buf);

  REPORT2("Partner semaphore: ", partner_sem.name);
  begin_conversation(ITERATIONS, &my_sem.sem, &partner_sem.sem, fd, &buf);
  
  free(buf);
  free(my_sem.name);
  free(partner_sem.name);
  sem_unlink(my_sem.name);
  sem_unlink(FIFO_SEM_NAME);
  unlink(FIFO_NAME);
  REPORT("process is finished");
  return 0;
}
