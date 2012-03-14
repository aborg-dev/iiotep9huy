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
	char* command[2];
	int pipe_position = -1;

	int length = strlen(argv[1]);
	for(int j = 0; j < length; j++)
	{
		if (argv[1][j] == '|')
		{
			pipe_position = j;
			break;
		}
	}
	if (pipe_position == -1)
	{
		fprintf(stderr, "Pipe not found\n");
		exit(EXIT_FAILURE);
	}
	
	command[0] = (char*) calloc(pipe_position, sizeof(char));
	command[1] = (char*) calloc(length - pipe_position, sizeof(char));
	strncat(command[0], argv[1], pipe_position);
	strncat(command[1], argv[1] + pipe_position + 1, length - pipe_position);

	/*
	printf("%s\n", command[0]);
	printf("%s\n", command[1]);
	*/

	int file_descriptor[2];

	if (pipe(file_descriptor) == -1)
	{
		fprintf(stderr, "Can\'t open pipe\n");
		exit(EXIT_FAILURE);
	}

	pid_t child_pid;
	child_pid = fork();

	if (child_pid == -1)
	{
		fprintf(stderr, "Can\'t fork\n");
		exit(EXIT_FAILURE);
	}

	if (child_pid == 0) // Child
	{
		//printf("child executing\n");
		close(STDIN_FILENO);
		dup2(file_descriptor[0], STDIN_FILENO);
		execl("/bin/bash", "bash", "-c", command[1], NULL);
	}
	else 								// Parent
	{
		//printf("parent executing\n");
		close(STDOUT_FILENO);
		dup2(file_descriptor[1], STDOUT_FILENO);
		execl("/bin/bash", "bash", "-c", command[0], NULL);
	}
}
