#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int countPipes(char* input)
{
  if (input == NULL)
    return 0;
  int pipeCount = 0;
  while (*input != '\0') { if (*input == '|')
    {
      pipeCount++;
    }
    input++;
  }
  return pipeCount;
}

void parseCommands(char* input, char*** commands, int* commandsCount)
{
  int length = strlen(input);
  int pipeCount = countPipes(input);
  *commandsCount = pipeCount + 1;

  *commands = (char**) calloc(*commandsCount, sizeof(char*));
  int currentCommand = 0;
  int commandBegin = 0, commandEnd = 0;

  for(int i = 0; i <= length; i++)
  {
    if (i == length || input[i] == '|')
    {
      commandEnd = i - 1;
      (*commands)[currentCommand] = (char*) calloc(commandEnd - commandBegin + 2, sizeof(char));
      strncpy((*commands)[currentCommand], input + commandBegin, commandEnd - commandBegin + 1);
      commandBegin = i + 1;
      currentCommand++;
    }
  }
}

void executeCommand(char* command, int input, int output, int obsoletePipe, int obsoletePipe1)
{
  pid_t childPID = fork();

  if (childPID == -1)
  {
    fprintf(stderr, "Can\'t fork\n");
    exit(EXIT_FAILURE);
  }

  if (childPID == 0)
  {
    close(obsoletePipe);
    close(obsoletePipe1);
    dup2(input, STDIN_FILENO);
    dup2(output, STDOUT_FILENO);
    execl("/bin/bash", "bash", "-c", command, NULL);
  }
}

void executeCommandLine(char** commands, int commandsCount)
{
  int fileDescriptor[2];
  int previousFileDescriptor[2];
  previousFileDescriptor[0] = STDIN_FILENO;
  previousFileDescriptor[1] = STDOUT_FILENO;

  for(int i = 0; i < commandsCount; i++)
  {
    if (pipe(fileDescriptor) == -1)
    {
      fprintf(stderr, "Can\'t open pipe\n");
      break;
    }
    if (i == commandsCount - 1)
    {
      dup2(STDOUT_FILENO, fileDescriptor[1]);
    }

    executeCommand(commands[i], previousFileDescriptor[0], fileDescriptor[1], previousFileDescriptor[1], fileDescriptor[0]);

    for(int j = 0; j < 2; j++)
    {
      if (i > 0)
      {
        close(previousFileDescriptor[j]);
      }
      previousFileDescriptor[j] = fileDescriptor[j];
    }
  }
  close(fileDescriptor[0]);
  close(fileDescriptor[1]);
  wait(NULL);
}

void processCommandLine(char* commandLine)
{
  printf("%s:\n", commandLine);
  char** commands = NULL;
  int commandsCount = 0;

  parseCommands(commandLine, &commands, &commandsCount);
  executeCommandLine(commands, commandsCount);

  for(int i = 0; i < commandsCount; i++)
  {
    free(commands[i]);
  }
  free(commands);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printf("Usage: \"command1 | command2\"\n");
    return 0;
  }
  else
  {
    for(int i = 1; i < argc; i++)
    {
      processCommandLine(argv[i]);
    }
  }
  return 0;
}

