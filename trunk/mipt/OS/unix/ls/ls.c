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

#define PATH_MAX 4096

const char DIR_SEPARATOR = '/';
const char* DIR_SEPARATOR_STR = "/";

bool isValidFileName(char* name)
{
  return (name != NULL && name[0] != '.');
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

void printFileStats(char* file_path, char* file_name, bool detailed)
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

void ls(char* absolute_path, bool detailed);

void printDirectoryContent(char* absolute_path, int path_length, bool detailed)
{
  DIR*   dir_file    = opendir(absolute_path);
  struct dirent* dir = readdir(dir_file);
  while(dir)
  {
    if (isValidFileName(dir->d_name))
    {
      absolute_path[path_length] = '\0';
      strcat(absolute_path, dir->d_name);

      printFileStats(absolute_path, dir->d_name, detailed);
    }
    dir = readdir(dir_file);
  }
  absolute_path[path_length] = '\0';
  closedir(dir_file);
}

void exploreChildDirectories(char* absolute_path, int path_length, bool detailed)
{
  DIR*   dir_file    = opendir(absolute_path);
  struct dirent* dir = readdir(dir_file);
  while(dir)
  {
    if (isValidFileName(dir->d_name))
    {
      absolute_path[path_length] = '\0';
      strcat(absolute_path, dir->d_name);

      ls(absolute_path, detailed);
    }
    dir = readdir(dir_file);
  }
  absolute_path[path_length] = '\0';
  closedir(dir_file);
}

void ls(char* absolute_path, bool detailed)
{
  if (!isExistingPath(absolute_path))
    return;

  printf("\n%s:\n", absolute_path);
  size_t path_length = strlen(absolute_path);

  if (absolute_path[path_length - 1] != DIR_SEPARATOR)
  {
    strcat(absolute_path, DIR_SEPARATOR_STR);
    path_length++;
  }
  
  printDirectoryContent(absolute_path, path_length, detailed);
  exploreChildDirectories(absolute_path, path_length, detailed);
}

int main(int argc, char** argv)
{
  char path[PATH_MAX];
  memset(path, 0, sizeof(path));
  if (argc < 2)
  {
    strcpy(path, ".");
    ls(path, false);
    printf("\n");
  }
  else
  {
    for(int i = 1; i < argc; i++)
    {
      strcpy(path, argv[i]);
      ls(path, true);
    }
  }
  return 0;
}
