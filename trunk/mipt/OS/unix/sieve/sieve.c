/*
 * =====================================================================================
 *
 *       Filename:  sieve.c
 * *    Description:  Eratosfen sieve with threads
 *
 *        Version:  1.0
 *        Created:  04/13/12 21:09:39
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
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>

#define MAXN 1000000
#define NUM_THREADS 4

char sieved[MAXN];
char ready[MAXN];
pthread_mutex_t sieve_mutex[MAXN];
int n;

void *SieveNumbers(void *threadid)
{
  for(int i = 2; i <= n; i++)
  {
    pthread_mutex_lock (&sieve_mutex[i]);
    if (!sieved[i] && !ready[i])
    {
      ready[i] = 1;
      for(int j = i + i; j <= n && !sieved[j]; j += i)
      {
        sieved[j] = 1;
      }
    }
    pthread_mutex_unlock (&sieve_mutex[i]);
  }
  pthread_exit(NULL);
}

int main(int argc, char** argv)
{
  time_t t1 = clock();
  scanf("%d", &n);
  freopen("primes.txt", "w", stdout);
  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for(int t = 0; t < NUM_THREADS; t++)
  {
    int return_code = pthread_create(&thread[t], NULL, SieveNumbers, (void *)t);
    if (return_code)
    {
       printf("ERROR; return code from pthread_create() is %d\n", return_code);
       return EXIT_FAILURE;
    }
  }
  pthread_attr_destroy(&attr);
  for(int t = 0; t < NUM_THREADS; t++) 
  {
     void* status;
     int return_code = pthread_join(thread[t], &status);
     if (return_code) 
     {
       printf("ERROR; return code from pthread_join() is %d\n", return_code);
       return EXIT_FAILURE;
     }
  }

  int prime_count = 0;
  for(int i = 2; i <= n; i++)
  {
    if (!sieved[i])
      prime_count++;
  }
  printf("%d\n", prime_count);
  for(int i = 2; i <= n; i++)
  {
    if (!sieved[i])
    {
      printf("%d ", i);
    }
  }
  printf("\n");

  time_t t2 = clock();
  fprintf(stderr, "Execution time: %.3lf\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
  pthread_exit(NULL);
  return 0;
}
