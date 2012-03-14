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

#define MAX_DEPTH = 10

const char DIR_SEPARATOR = '/';
const char* DIR_SEPARATOR_STR = "/";

int max_depth = 100;

bool isValidFileName(char* name)
{
  if (name == NULL || name[0] == '.')
    return false;
  return true;
}

void printFileMode(mode_t file_mode)
{
  printf("%c", S_ISDIR(file_mode) ? 'd' : '-');

  printf("%c", S_IRUSR & file_mode ? 'r' : '-');
  printf("%c", S_IWUSR & file_mode ? 'w' : '-');
  printf("%c", S_IXUSR & file_mode ? 'x' : '-');

  printf("%c", S_IRGRP & file_mode ? 'r' : '-');
  printf("%c", S_IWGRP & file_mode ? 'w' : '-');
  printf("%c", S_IXGRP & file_mode ? 'x' : '-');

  printf("%c", S_IROTH & file_mode ? 'r' : '-');
  printf("%c", S_IWOTH & file_mode ? 'w' : '-');
  printf("%c", S_IXOTH & file_mode ? 'x' : '-');
}

void printFileStats(char* file_path, char* file_name, int detailed)
{
  if (detailed)
  {
    struct stat file_stat;
    if (stat(file_path, &file_stat) == 0)
    {
      printFileMode(file_stat.st_mode);
      struct passwd* pwd = getpwuid(file_stat.st_uid);
      struct group* grp = getgrgid(file_stat.st_gid);
      char* time = ctime(&file_stat.st_atime);
      time[strlen(time) - 1] = '\0';
      printf(" %1d %6s %5s %5d %3s %s\n", file_stat.st_nlink,
                                          pwd->pw_name,
                                          grp->gr_name, 
                                          file_stat.st_size, 
                                          time,
                                          file_name);
    }
    else
      printf("problem %s\n", file_path); // this should never happen
  }
  else
  {
    printf("%s ", file_name);
  }
}

bool isExistingPath(char* absolute_path)
{
  DIR* dir_file = opendir(absolute_path);
  if (dir_file == NULL)
  {
    //printf("Error: Path does not exits.\n");
    return false;
  }
  closedir(dir_file);
  return true;
}

void ls(char* absolute_path, int depth, int detailed)
{
  if (depth > max_depth)
    return;
  if (!isExistingPath(absolute_path))
    return;

  printf("\n%s:\n", absolute_path);
  size_t path_length = strlen(absolute_path);
  char*  file_path   = (char*) malloc(path_length + 1);

  strcpy(file_path, absolute_path);
  if (file_path[path_length - 1] != DIR_SEPARATOR)
  {
    strcat(file_path, DIR_SEPARATOR_STR);
    path_length++;
  }
  
  for(int process_mode = 0; process_mode <= 1; process_mode++) // 0 - print, 1 - discover.
  {
    DIR*   dir_file    = opendir(absolute_path);
    struct dirent* dir = readdir(dir_file);
    while(dir)
    {
      if (isValidFileName(dir->d_name))
      {
        if (strlen(file_path) < path_length + strlen(dir->d_name))
          file_path = (char*) realloc(file_path, path_length + strlen(dir->d_name) + 1);
        file_path[path_length] = '\0';
        strcat(file_path, dir->d_name);

        if (process_mode == 0)
        {
          printFileStats(file_path, dir->d_name, detailed);
        }
        else
        {
          ls(file_path, depth + 1, detailed);
        }
      }
      dir = readdir(dir_file);
    }
    closedir(dir_file);
    free(dir);
  }
  free(file_path);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    ls(".", 0, 0);
    printf("\n");
  }
  else
  {
    for(int i = 1; i < argc; i++)
    {
      ls(argv[i], 0, 1);
    }
  }
  return 0;
}
