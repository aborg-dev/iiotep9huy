/*
 * =====================================================================================
 *
 *       Filename:  string.c
 *
 *    Description:  String library implementation
 *
 *        Version:  1.0
 *        Created:  05/06/12 18:41:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <stdarg.h>

#include "string.h"

const size_t READ_BLOCK_SIZE = 100;

void init_string(string* str)
{
  str->content = NULL;
  str->length = 0;
  str->capacity = 0;
}

void free_string(string* str)
{
  if (str->content != NULL)
  {
    free(str->content);
    str->content = NULL;
    str->length = 0;
    str->capacity = 0;
  }
}

void adjust_string(string* str, size_t new_length)
{
  if (str->capacity < new_length)
  {
    str->content = (char*) realloc(str->content, new_length * sizeof(char) + 1);
    str->capacity = new_length;
  }
  str->length = new_length;
  str->content[new_length - 1] = '\0';
}

void resize_string(string* str, size_t new_length)
{
  str->content = (char*) realloc(str->content, new_length * sizeof(char) + 1);
  str->capacity = new_length;
  str->length = new_length;
  str->content[new_length - 1] = '\0';
}

void append_to_string(string* str, char* buf)
{
  int str_len = str->length;
  int buf_len = strlen(buf);

  adjust_string(str, buf_len + str_len);
  strcat(str->content, buf);
}

void print_to_string(string* str, const char* fmt, ...)
{
  char* tmp_str;
  int len_printed;
  va_list ap;

  va_start(ap, fmt);
  len_printed = vasprintf(&tmp_str, fmt, ap);
  va_end(ap);

  adjust_string(str, len_printed);
  strcpy(str->content, tmp_str);
  free(tmp_str);
}

void print_string(int fd, const string* str)
{
  write(fd, str->content, str->length);
}

void read_to_string(int fd, string* str)
{
  char buf[READ_BLOCK_SIZE];
  memset(buf, 0, sizeof(buf));
  int len_read = 0; 
  int new_len = 0;
  int offset = 0;

  while (1)
  {
    len_read = read(fd, &buf, READ_BLOCK_SIZE);
    if (len_read <= 0)
      break;

    new_len += len_read;

    adjust_string(str, new_len);
    strncpy(str->content + offset, buf, len_read);

    offset = new_len;
  }
}
