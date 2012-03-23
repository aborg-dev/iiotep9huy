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

int main(int argc, char** argv)
{
  int file_descriptor = mkfifo("/tmp/ping_pong_fifo", 777);
  sem_t* semaphore_descriptor = sem_open("/tmp/ping_pong_semaphore", O_CREAT, 777, 1);
  if (file_descriptor == 0)
  {
    fprintf(stderr, "file_descriptor created\n");
    sleep(5);
    close(file_descriptor);
  }
  else
  {
    if (errno == EEXIST)
    {
      fprintf(stderr, "file is already created\n");
      sleep(5);
    }
    else
    {
      fprintf(stderr, "failed to create file\n");
    }
  }
  unlink("/tmp/ping_pong_fifo");
  fprintf(stderr, "process %d is finished\n", getpid());
  return 0;
}
