/*
 * =====================================================================================
 *
 *       Filename:  backup.c
 *
 *    Description:  Backups user files.
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
#include <fcntl.h>

const size_t MAX_PATH_LEN = 255;
const size_t BLOCK_SIZE = 255;
const char DIR_SEPARATOR = '/';
const char* DIR_SEPARATOR_STR = "/";

bool isValidFileName(char* name)
{
  return (name != NULL && name[0] != '.');
}

bool isExistingDir(char* absolute_path)
{
  DIR* dir_file = opendir(absolute_path);
  if (dir_file == NULL)
  {
    return false;
  }
  closedir(dir_file);
  return true;
}

bool isExistingPath(char* absolute_path)
{
  struct stat file_stat;
  if (stat(absolute_path, &file_stat) == 0)
  {
    return true;
  }
  return false;
}

void copy_file(char* source, char* destination)
{
  if (isExistingDir(source))
  {
    struct stat source_stat;
    stat(source, &source_stat);
    if (isExistingPath(destination))
      unlink(destination);
    mkdir(destination, source_stat.st_mode);
    return;
  }
  FILE* src = fopen(source, "r");
  FILE* dest = fopen(destination, "w");
  char* buf = (char*) malloc(BLOCK_SIZE);
  size_t bytes_read = -1;
  while ((bytes_read = fread(buf, BLOCK_SIZE, 1, src)) > 0)
  {
    fwrite(buf, bytes_read, 1, dest);
  }
  fclose(src);
  fclose(dest);
}

size_t get_file_hash(char* file_name)
{
  size_t hash = 0;
  FILE* file = fopen(file_name, "r");
  int fchar = 0;
  while ((fchar = fgetc(file)) > 0)
  {
    hash ^= fchar;
  }
  return hash;
}

bool need_backup(char* source, char* destination)
{
  struct stat source_stat;
  struct stat destination_stat;
  if (stat(source, &source_stat) == 0)
  {
    struct passwd* source_pwd = getpwuid(source_stat.st_uid);
    if (source_pwd->pw_uid != getuid())
    {
      return false;
    }
  }
  else
    fprintf(stderr, "Trying to backup bad file.\n");

  if (!isExistingPath(destination))
  {
    return true;
  }
  else
  {
    if (stat(destination, &destination_stat) == 0)
    {
      if (source_stat.st_size != destination_stat.st_size ||
          get_file_hash(source) != get_file_hash(destination))
      {
        return true;
      }
    }
    else
      fprintf(stderr, "Trying to backup to bad file.\n");
  }
  return false;
}

void backup_file(char* source, char* destination)
{
  fprintf(stdout, "Copy %s to %s\n", source, destination);
  copy_file(source, destination);
}

void recursive_backup(char* absolute_path, char* source, char* destination)
{
  char* source_dir_path = (char*) malloc(MAX_PATH_LEN);
  strcpy(source_dir_path, source);
  strcat(source_dir_path, absolute_path);

  char* destination_dir_path = (char*) malloc(MAX_PATH_LEN);
  strcpy(destination_dir_path, destination);
  strcat(destination_dir_path, absolute_path);

  if (!isExistingDir(source_dir_path))
    return;

  //fprintf(stderr, "\n%s: %d\n", absolute_path, strlen(absolute_path));
  size_t path_length = strlen(absolute_path);
  char*  file_path   = (char*) malloc(MAX_PATH_LEN);
  char* source_file_path = (char*) malloc(MAX_PATH_LEN);
  char* destination_file_path = (char*) malloc(MAX_PATH_LEN);

  strcpy(file_path, absolute_path);
  if (file_path[path_length - 1] != DIR_SEPARATOR)
  {
    strcat(file_path, DIR_SEPARATOR_STR);
    path_length++;
  }

  // Traversing source directory content
  DIR*   dir_file    = opendir(source_dir_path);
  struct dirent* dir = readdir(dir_file);
  while(dir)
  {
    if (isValidFileName(dir->d_name))
    {
      file_path[path_length] = '\0';
      strcat(file_path, dir->d_name);
      
      strcpy(source_file_path, source);
      strcat(source_file_path, file_path);

      strcpy(destination_file_path, destination);
      strcat(destination_file_path, file_path);

      if (isExistingPath(source_file_path))
      {
        if (need_backup(source_file_path, destination_file_path))
        {
          backup_file(source_file_path, destination_file_path);
        }
      }

      recursive_backup(file_path, source, destination);
    }
    dir = readdir(dir_file);
  }
  closedir(dir_file);
  free(dir);
  // --------------------------
 
  // Traversing destination directory content
  dir_file    = opendir(destination_dir_path);
  dir = readdir(dir_file);
  while(dir)
  {
    if (isValidFileName(dir->d_name))
    {
      file_path[path_length] = '\0';
      strcat(file_path, dir->d_name);

      strcpy(source_file_path, source);
      strcat(source_file_path, file_path);

      strcpy(destination_file_path, destination);
      strcat(destination_file_path, file_path);

      if (isExistingPath(destination_file_path) &&
          !isExistingPath(source_file_path))
      {
        fprintf(stdout, "Removing old file %s\n", destination_file_path);
        unlink(destination_file_path);
      }

    }
    dir = readdir(dir_file);
  }
  closedir(dir_file);
  free(dir);
  // --------------------------

  free(file_path);
}

void backup(char* source, char* destination)
{
  printf("Backup %s to %s\n", source, destination);
  if (!isExistingDir(source))
  {
    fprintf(stderr, "backup: cannot find: %s No such file or directory\n", source);
    return;
  }
  if (!isExistingDir(destination))
  {
    fprintf(stderr, "backup: cannot find: %s No such file or directory\n", destination);
    return;
  }
  char* dummy_path = (char*) malloc(1);
  dummy_path[0] = '\0';

  if (source[strlen(source) - 1] == DIR_SEPARATOR)
  {
    source[strlen(source) - 1] = '\0';
  }

  if (destination[strlen(destination) - 1] == DIR_SEPARATOR)
  {
    destination[strlen(destination) - 1] = '\0';
  }

  recursive_backup(dummy_path, source, destination);
}

int main(int argc, char** argv)
{
  if (argc < 2 || argc > 3)
  {
    fprintf(stderr, "Usage: backup [SOURCE] DEST\n");
    return 0;
  }
  if (argc == 2)
  {
    backup(".", argv[1]);
  }
  else
  {
    backup(argv[1], argv[2]);
  }
  return 0;
}
