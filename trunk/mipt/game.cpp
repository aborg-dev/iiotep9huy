#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <memory.h>
#include <map>
#include <cassert>
#include <queue>

#define DB(x) cerr << #x << ": " << x << endl;
#define x first
#define y second

using namespace std;

const int MAXN = 51;

int field[MAXN][MAXN];
int n, m;

struct state
{
  int pointer_x, pointer_y, type;
  void change(int delta)
  {
    if (type == 0)
    {
      pointer_y += delta;
      pointer_y += m;
      pointer_y %= m;
    }
    if (type == 1)
    {
      pointer_x += delta;
      pointer_x += n;
      pointer_x %= n;
    }   
  }
  int& pointer()
  {
    if (type == 0)
      return pointer_y;
    else
      return pointer_x;
  }
};

state current;

void ClearScreen()
{
  cout << string(33, '\n');
}

void PrintField(state current)
{
  if (current.type == 0)
  {
    for(int i = 0; i < (current.pointer_y + 1)*3; i++)
      printf(" ");
    printf("+");
  }
  printf("\n");
  for(int i = 0; i < n; i++)
  {
    if (current.type == 1)
    {
      if (current.pointer_x == i)
        printf("+");
      else
        printf(" ");
    }
    else
      printf(" ");
    for(int j = 0; j < m; j++)
    {
      printf("%3d", field[i][j]);
    }
    printf("\n");
  }
}

void column_rotate(int row_num, int delta)
{
  if (delta == 1)
  {
    int tmp = field[0][row_num];
    for(int i = 0; i < n - 1; i++)
      field[i][row_num] = field[i + 1][row_num];
    field[n - 1][row_num] = tmp;
  }
  else
  {
    int tmp = field[n - 1][row_num];
    for(int i = n - 1; i > 0; i--)
      field[i][row_num] = field[i - 1][row_num];
    field[0][row_num] = tmp;
  }
}

void row_rotate(int column_num, int delta)
{
  if (delta == 1)
  {
    int tmp = field[column_num][0];
    for(int i = 0; i < m - 1; i++)
      field[column_num][i] = field[column_num][i + 1];
    field[column_num][n - 1] = tmp;
  }
  else
  {
    int tmp = field[column_num][m - 1];
    for(int i =  m - 1; i > 0; i--)
      field[column_num][i] = field[column_num][i - 1];
    field[column_num][0] = tmp;
  }
}

void d() { column_rotate(current.pointer_y, 1); }
void u() { column_rotate(current.pointer_y, -1); }
void r() { row_rotate(current.pointer_x, 1); }
void l() { row_rotate(current.pointer_x, -1); }

void cool_rotate()
{
  d(); l(); u(); r(); d(); l(); u(); l(); d(); r(); u(); l();
}

void field_rotate(state current, int delta)
{
  if (current.type == 0)
    column_rotate(current.pointer_y, delta);
  else
    row_rotate(current.pointer_x, delta);
}

int main()
{
  FILE *input = fopen("input.txt", "r");
  fscanf(input, "%d%d", &n, &m);
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      fscanf(input, "%d", &field[i][j]);
    }
  }
  fclose(input);
  PrintField(current);
  while (1)
  {
    ClearScreen();
    PrintField(current);
    char keyboard_input;
    system("stty raw");
    keyboard_input = getchar();
    system("stty cooked");
    switch (keyboard_input)
    {
      case 65:
      case 66:
        if (current.type == 0)
          field_rotate(current, keyboard_input == 66 ? -1 : 1);
        else
          current.change(keyboard_input == 66 ? 1 : -1);
        break;
      case 67:
      case 68:
        if (current.type == 1)
          field_rotate(current, keyboard_input == 67 ? -1 : 1);
        else
          current.change(keyboard_input == 67 ? 1 : -1);
        break;
      case 32:
        current.type ^= 1;
        break;
      case 'x':
        cool_rotate();
        break;
      case 'q':
        return 0;
    }
  }
  return 0;
}
