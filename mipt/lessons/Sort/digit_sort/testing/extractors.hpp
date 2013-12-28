#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

class int_extractor {
  public:
  const static size_t block_count = 4;
  const static size_t block_size = 8;
  size_t getkey(int X, int block_num) {
    return (X >> (block_size*block_num)) & ((1ll<<block_size) - 1);
  }
};

int gen_int() {
  return rand()&((1ll<<32)-1);
}

class long_long_extractor {
  public:
  const static size_t block_count = 4;
  const static size_t block_size = 16;
  size_t getkey(long long X, int block_num) {
    return (X >> (block_size*block_num)) & ((1ll<<block_size) - 1);
  }
};

long long gen_long_long() {
  return rand();
}

class string_extractor {
  public:
  const static size_t block_count = 5;
  const static size_t block_size = 7;
  size_t getkey(string X, int block_num) {
    return X[block_count - block_num - 1];
  }
};

string gen_string() {
  string a;
  for(int i=0; i<5; i++) {
    a += (rand() & ((1<<7) - 1));
  }
  return a;
}

class pair_extractor {
  public:
  const static size_t block_count = 2;
  const static size_t block_size = 16;
  size_t getkey(pair<int, int> X, int block_num) {
    return block_num ? X.first : X.second;
  }
};

pair<int, int> gen_pair() {
  return make_pair(rand()&((1<<16)-1), rand()&((1<<16)-1));
}

