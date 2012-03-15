#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Usage: \"command1 | command2\"\n");
    return 0;
  }
  int pipeCount = 0;
  int commandCount = 0;
  int length = strlen(argv[1]);

  for(int i = 0; i < length; i++)
  {
    if (argv[1][i] == '|')
      pipeCount++;
  }
  commandCount = pipeCount + 1;

  char* command[commandCount];
  int currentCommand = 0;
  int commandBegin = 0, commandEnd = 0;

  for(int i = 0; i <= length; i++)
  {
    if (i == length || argv[1][i] == '|')
    {
      commandEnd = i - 1;
      command[currentCommand] = (char*) calloc(commandEnd - commandBegin + 2, sizeof(char*));
      strncpy(command[currentCommand], argv[1] + commandBegin, commandEnd - commandBegin + 1);
      commandBegin = i + 1;
      currentCommand++;
    }
  }

  for(int i = 0; i < commandCount; i++)
  {
    printf("%s\n", command[i]);
  }

  int file_descriptor[pipeCount][2];

  for(int i = 0; i < pipeCount; i++)
  {
    if (pipe(file_descriptor[i]) == -1)
    {
      fprintf(stderr, "Can\'t open pipe\n");
      break;
    }
  }

  for(int i = 0; i < commandCount; i++)
  {
    printf("Command number: %d\n", i);
    /*if (i < pipeCount)
    {
      if (pipe(file_descriptor[i]) == -1)
      {
        fprintf(stderr, "Can\'t open pipe\n");
        break;
      }
    }*/

    pid_t child_pid;
    child_pid = fork();

    if (child_pid == -1)
    {
      fprintf(stderr, "Can\'t fork\n");
      break;
    }

    if (child_pid == 0) // Child
    {
      printf("Child executing:\n");
      if (i != commandCount - 1)
      {
        close(STDOUT_FILENO);
        dup2(file_descriptor[i][1], STDOUT_FILENO);
      }
      if (i != 0)
      {
        close(STDIN_FILENO);
        dup2(file_descriptor[i - 1][0], STDIN_FILENO);
      }
      printf("AAA %d\n", i);
      execl("/bin/bash", "bash", "-c", command[i], NULL);
    }
    else                // Parent
    {
      printf("Parent executing:\n");
    }
  }
  printf("Now we are waiting\n");
  wait(NULL);
  for(int i = 0; i < commandCount; i++)
  {
    free(command[i]);
  }
  printf("Memory is free\n");
  return 0;
}

