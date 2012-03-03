/*
 * =====================================================================================
 *
 *       Filename:  wordcount.c
 *
 *    Description:  Words counting program
 *
 *        Version:  1.0
 *        Created:  03/01/2012 10:14:53 AM
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
#include <ctype.h>
#include <sys/types.h>

const char* QUOTE = "\"\'$#\0";

enum RETURN_MESSAGE
{
  SUCCESS,
  FILE_ERROR,
  QUOTE_ERROR
};

typedef struct
{
  int word_count;
  int in_quotes;
  int in_word;
  int quote_filled;
} State;

void initState(State* state)
{
  state->word_count = 0;
  state->in_quotes = 0;
  state->in_word = 0;
  state->quote_filled = 0;
}

int isQuote(char c)
{
  return (strchr(QUOTE, c) != NULL);
}

enum RETURN_MESSAGE countWordsFromFile(FILE* input, int* word_count)
{
  if (input == NULL)
    return FILE_ERROR;
  State state;
  initState(&state);
  while(!feof(input))
  {
    char c = fgetc(input);
    if (isQuote(c))
    {
      state.in_quotes ^= 1;
      state.in_word = 0;
      state.quote_filled = 0;
    }
    else
    if (isalnum(c))
    {
      if (!state.in_word && (!state.quote_filled || !state.in_quotes))
        state.word_count++;
      state.quote_filled = 1;
      state.in_word = 1;
    }
    else
    {
      state.in_word = 0;
    }
  }
  *word_count = state.word_count;
  if (state.in_quotes)
    return QUOTE_ERROR;
  return SUCCESS;
}

void printWordsCountNoErrors(FILE* file)
{
  int word_count = 0;
  enum RETURN_MESSAGE error_message = countWordsFromFile(file, &word_count);
  switch (error_message)
  {
    case QUOTE_ERROR:
      printf("Unclosed quotes!\n");
      return;
    case FILE_ERROR:
      printf("File not found!\n");
      return;
    case SUCCESS:
      printf("Word count: %d\n", word_count);
      return;
  }
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printWordsCountNoErrors(stdin);   
  }
  else
  {
    for(int i = 1; i < argc; i++)
    {
      FILE *input = fopen(argv[i], "r");
      printf("File: %s\n", argv[i]);
      printWordsCountNoErrors(input);
    }
  }
	return 0;
}
