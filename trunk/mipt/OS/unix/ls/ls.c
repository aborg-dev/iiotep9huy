#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define MAX_DEPTH 10
#define MAX_PATH_LENGTH (MAX_DEPTH * (1 << 8))

//const int MAX_DEPTH = 10;
//const int MAX_PATH_LENGTH = 255 * MAX_DEPTH;

char path[MAX_PATH_LENGTH];
int max_depth = 0;

int ValidFile(char* name)
{
	if (name == NULL)
		return 0;
	if (name[0] == '.')
		return 0;
	return 1;
}

void ls(char* absolute_path, int depth)
{
  if (depth > max_depth)
    return;
	DIR* dir_file = opendir(absolute_path);
	if (dir_file == NULL)
		return;

	printf("%s:\n", absolute_path);
	struct dirent* dir = readdir(dir_file);
  char* new_path = (char*) malloc( sizeof(absolute_path) );
	while(dir)
	{
    if (sizeof(new_path) < sizeof(absolute_path) + sizeof(dir->d_name))
      new_path = (char*) realloc( new_path, sizeof(absolute_path) + sizeof(dir->d_name) );
    new_path[0] = 0;
		if (ValidFile(dir->d_name))
		{
			strcat(new_path, absolute_path);
			strcat(new_path, dir->d_name);
			printf("%s\n", dir->d_name);
			ls(new_path, depth + 1);
		}
		dir = readdir(dir_file);
	}
  free(new_path);
  free(dir);
}

int main(int argc, char** argv)
{
  // -R all
  // -maxdepth
	if (argc < 2)
	{
		printf("Error: Need more arguments.\n");
		return 1;
	}
  strcat(path, argv[1]);
	ls(path, 0);
	return 0;
}
