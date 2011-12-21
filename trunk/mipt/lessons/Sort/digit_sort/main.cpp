#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <memory.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <cassert>

using namespace std;

const int len = 1;
const int maxn = 1000000;

struct int_struct {
  int a;
  bool operator() (int_struct x, int_struct y) {
    return x.a < y.a;
  }
};

class int_extractor {
  public:
  const static size_t block_count = 2;
  const static size_t block_size = 16;
  size_t getkey(int_struct X, int block_num) {
    int tmp = X.a;
    for(int i=0; i<block_num; i++) tmp >>= block_size;
    return tmp & ((1ll<<block_size) - 1);
  }
};

struct long_long_struct {
  long long a;
  bool operator() (long_long_struct x, long_long_struct y) {
    return x.a < y.a;
  }
};

class long_long_extractor {
  public:
  const static size_t block_count = 4;
  const static size_t block_size = 16;
  size_t getkey(long_long_struct X, int block_num) {
    long long tmp = X.a;
    for(int i=0; i<block_num; i++) tmp >>= block_size;
    return tmp & ((1ll<<block_size) - 1);
  }
};

struct string_struct {
  string a;
  bool operator() (string_struct x, string_struct y) {
    return x.a < y.a;
  }
};

class string_extractor {
  public:
  const static size_t block_count = 5;
  const static size_t block_size = 8;
  size_t getkey(string_struct X, int block_num) {
    return X.a[block_count - block_num - 1];
  }
};

struct pair_struct {
  pair<int, int> a;
  bool operator() (pair_struct x, pair_struct y) {
    return x.a < y.a;
  }
};

class pair_extractor {
  public:
  const static size_t block_count = 2;
  const static size_t block_size = 16;
  size_t getkey(pair_struct X, int block_num) {
    if (block_num == 0) return X.a.second;
    if (block_num == 1) return X.a.first;
  }
};

template<class T, class BlockExtractor>
void digit_sort(T l, T r, BlockExtractor BE) {
  size_t size = (1 << BE.block_size) + 1;
  vector<typeof(*l)> tmp(r - l);
  vector<int> cnt(size, 0);
  for(int i=0; i<BE.block_count; i++) {
    for(T it = l; it != r; ++it) {
      cnt[BE.getkey(*it, i) + 1]++;
    }
    for(int it = 1; it < size; ++it) {
      cnt[it] += cnt[it-1];
    }
    for(T it = l; it != r; ++it) {
      tmp[cnt[BE.getkey(*it, i)]++] = *it;
    }
    copy(tmp.begin(), tmp.end(), l);
    cnt.assign(size, 0);
  }
}

int n = maxn;
int tmp[maxn*len];
int_struct S1[maxn];
int_struct S2[maxn];
int_extractor BE;

// Generating input
void gen() {
  srand(time(NULL));
  for(int i=0; i<n*len; i++) {
    tmp[i] = rand() & ((1ll<<32)-1);
  }
}

// Tests STL_sort
void test_stl_sort() {
  for(int k = 0; k < n; k++) {
    S1[k].a = tmp[k];
  }
  double t1 = clock();
  stable_sort(S1, S1 + n, int_struct());
  double t2 = clock();
  printf("STL: %.lf\n", (t2 - t1) / 1000.0);
}

// Tests digit_sort
void test_digit_sort() {
  for(int k = 0; k < n; k++) {
    S2[k].a = tmp[k];
  }
  double t1 = clock();
  digit_sort(S2, S2 + n, BE);
  double t2 = clock();
  printf("Digit: %.lf\n", (t2 - t1) / 1000.0);
}

// Checks STL_sort and digit_sort output
void check() {
  for(int i=0; i<n; i++) {
    for(int k=0; k < BE.block_count; k++) {
      if (S1[i].a != S2[i].a) {
        for (int t = 0; t < maxn; t++) {
          printf("%d ", tmp[t]);
        }
        printf("\n");
        for (int t = 0; t < maxn; t++) {
          printf("%d ", S1[t].a);
        }
        printf("\n");
        for (int t = 0; t < maxn; t++) {
          printf("%d ", S2[t].a);
        }
        printf("\n");
      }
      assert(S1[i].a == S2[i].a);
    }
  }
  printf("Match\n");
}

int main() {

  gen_int();
  int_test_stl_sort();
  int_test_digit_sort();
  int_check();

  gen_ll();
  ll_test_stl_sort();
  ll_test_digit_sort();
  ll_check();

  gen_string();
  string_test_stl_sort();
  string_test_digit_sort();
  string_check();

  return 0;
}
