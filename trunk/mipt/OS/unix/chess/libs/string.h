/*
 * =====================================================================================
 *
 *       Filename:  string.h
 *
 *    Description:  String library definition
 *
 *        Version:  1.0
 *        Created:  05/06/12 18:40:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef STRING_H
#define STRING_H 

typedef struct 
{
  char* content;
  size_t length;
  size_t capacity;
} string;

void init_string(string* str);
void free_string(string* str);

void adjust_string(string* str, size_t new_length);
void resize_string(string* str, size_t new_length);

void append_to_string(string* str, char* buf);
void print_to_string(string* str, const char* fmt, ...);
void read_to_string(int fd, string* str);

void send_string(int socket, const string* str);
void recv_to_string(int socket, string* str);

void print_string(int fd, const string* str);

int string_to_int(const string* str);

#endif /* STRING_H */
