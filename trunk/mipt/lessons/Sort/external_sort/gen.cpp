#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
  int n = 0, pnt = 0;
  while (pnt < strlen(argv[1])) {
    n *= 10;
    n += argv[1][pnt++] - '0';
  }
  #define print(x) std::cout.write((char*)(&(x)), sizeof(x))
  print(n);
  srand(time(NULL));
  for(int i=0; i<n; i++) {
    int x = rand();
    print(x);
  }
}
