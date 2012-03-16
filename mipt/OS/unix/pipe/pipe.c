#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void parseCommands()
{
  
}

void executeCommands()
{
}

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

  int file_descriptor[2];
  int previous_file_descriptor[2];
  previous_file_descriptor[0] = STDIN_FILENO;
  previous_file_descriptor[1] = STDOUT_FILENO;

  for(int i = 0; i < commandCount; i++)
  {
    if (pipe(file_descriptor) == -1)
    {
      fprintf(stderr, "Can\'t open pipe\n");
      break;
    }

    pid_t child_pid;
    child_pid = fork();

    if (child_pid == -1)
    {
      fprintf(stderr, "Can\'t fork\n");
      break;
    }

    if (child_pid == 0) // Child
    {
      if (i != commandCount - 1)
      {
        dup2(file_descriptor[1], STDOUT_FILENO);
      }
      if (i != 0)
      {
        dup2(previous_file_descriptor[0], STDIN_FILENO);
        close(previous_file_descriptor[1]);
      }

      close(file_descriptor[0]);
      execl("/bin/bash", "bash", "-c", command[i], NULL);
      wait(NULL);
    }
    else                // Parent
    {
      for(int j = 0; j < 2; j++)
      {
        if (i != 0)
          close(previous_file_descriptor[j]);
        previous_file_descriptor[j] = file_descriptor[j];
      }
    }
  }
  close(file_descriptor[0]);
  close(file_descriptor[1]);
  wait(NULL);
  for(int i = 0; i < commandCount; i++)
  {
    free(command[i]);
  }
  return 0;
}

