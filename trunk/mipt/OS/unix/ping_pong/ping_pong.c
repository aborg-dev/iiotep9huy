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

const size_t MESSAGE_MAXLEN = 20;
const size_t SEMAPHORE_NAME_LEN = 15;


int main(int argc, char** argv)
{
  char *semaphore_name = (char*) malloc(SEMAPHORE_NAME_LEN);
  char *partner_semaphore_name = (char*) malloc(SEMAPHORE_NAME_LEN);
  char* buf = (char*) malloc(MESSAGE_MAXLEN);
  sprintf(semaphore_name, "ping_pong_%d", getpid());
  //fprintf(stderr, "%s\n", semaphore_name);

  sem_t* my_semaphore = sem_open(semaphore_name, O_CREAT, 0777, 0);
  sem_t* partner_semaphore = NULL;
  sem_t* file_semaphore = sem_open("fifo_semaphore", O_CREAT, 0777, 1);

  sem_post(file_semaphore);
  fprintf(stderr, "I'm waiting for file\n");

  sem_wait(file_semaphore);
  int file_descriptor = mkfifo("/tmp/ping_pong_fifo", 0777);
  sem_post(file_semaphore);

  if (file_descriptor == 0)
  {
    file_descriptor = open("/tmp/ping_pong_fifo", O_RDWR);
    fprintf(stderr, "I am here1 %d\n", file_descriptor);

    sem_wait(file_semaphore);
    fprintf(stderr, "I created file! %d\n", getpid());
    write(file_descriptor, semaphore_name, SEMAPHORE_NAME_LEN); 
    sem_post(file_semaphore);

    fprintf(stderr, "I'm waiting for my_semaphore\n");
    sem_wait(my_semaphore);

    read(file_descriptor, partner_semaphore_name, SEMAPHORE_NAME_LEN);
    partner_semaphore = sem_open(partner_semaphore_name, 0);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "Hi! My name is %d\n", getpid());
    write(file_descriptor, buf, MESSAGE_MAXLEN);
    fprintf(stderr, "Woohoo!");

    sem_post(partner_semaphore);

  }
  else
  {
    file_descriptor = open("/tmp/ping_pong_fifo", O_RDWR);
    fprintf(stderr, "I am here2 %d\n", file_descriptor);
    sem_wait(file_semaphore);
    fprintf(stderr, "I've opened file! %d\n", getpid());
    read(file_descriptor, partner_semaphore_name, SEMAPHORE_NAME_LEN);
    fprintf(stderr, "%s\n", partner_semaphore_name);
    partner_semaphore = sem_open(partner_semaphore_name, 0);
    write(file_descriptor, semaphore_name, SEMAPHORE_NAME_LEN);
    sem_post(file_semaphore);
    sem_post(partner_semaphore);
  }

  int count = 3;
  fprintf(stderr, "Partner semaphore = %s\n", partner_semaphore_name);
  fprintf(stderr, "I'm near infinite while %d\n", getpid());
  while (count--)
  {
    sem_wait(my_semaphore);
    read(file_descriptor, buf, MESSAGE_MAXLEN);
    fprintf(stderr, "I am %d. I've got message: \" %s \"\n", getpid(), buf);
    sprintf(buf, "Hi, my name is %d", getpid());
    write(file_descriptor, buf, MESSAGE_MAXLEN);
    sem_post(partner_semaphore);
    sleep(5);
  }
  free(buf);
  free(partner_semaphore_name);
  sem_close(my_semaphore);
  sem_close(file_semaphore);
  unlink("/tmp/ping_pong_fifo");
  fprintf(stderr, "process %d is finished\n", getpid());
  return 0;
}
