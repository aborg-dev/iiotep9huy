#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_DEPTH = 10

int max_depth = 100;

int validFileName(char* name)
{
	if (name == NULL)
		return 0;
	if (name[0] == '.')
		return 0;
	return 1;
}

void printFileStats(char* file_path, char* file_name, int detailed)
{
	if (detailed)
	{
		struct stat* file_stat = (struct stat*) malloc(sizeof(struct stat));
		if (stat(file_name, file_stat) == 0)
		{
			printf("%d %s\n", file_stat->st_ctime, file_name);
			free(file_stat);
		}
		else
			printf("problem %s\n", file_path);
	}
	printf("%s\n", file_name);
}

void ls(char* absolute_path, int depth, int detailed)
{
	if (depth > max_depth)
		return;
	DIR* dir_file = opendir(absolute_path);
	if (dir_file == NULL)
	{
		//printf("Error: Path does not exits.\n");
		return;
	}
	printf("%s:\n", absolute_path);
	struct dirent* dir = readdir(dir_file);
	char* file_path;
	file_path = (char*) malloc(strlen(absolute_path) + 1 + 255);
	file_path[0] = '\0';
	while(dir)
	{
		if (validFileName(dir->d_name))
		{
			//if (strlen(file_path) < strlen(absolute_path) + strlen(dir->d_name))
				//file_path = (char*) realloc(file_path, strlen(absolute_path) + strlen(dir->d_name) + 1);
			file_path[0] = '\0';
			strcat(file_path, absolute_path);
			if (file_path[strlen(absolute_path) - 1] != '/')
				strcat(file_path, "/");
			strcat(file_path, dir->d_name);
			printf("%s\n", file_path);
			ls(file_path, depth + 1, detailed);
			printFileStats(file_path, dir->d_name, detailed);
		}
		dir = readdir(dir_file);
	}
	free(file_path);
	free(dir);
}

int main(int argc, char** argv)
{
	for(int i = 1; i < argc; i++)
		ls(argv[i], 0, 1);
	return 0;
}
