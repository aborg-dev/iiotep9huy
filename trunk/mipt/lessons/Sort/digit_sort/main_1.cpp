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

template<class T>
void digit_sort(T l, T r, size_t blocksize = 16) {
  size_t size = (1 << blocksize) + 1;
  int maxnum = *max_element(l, r);
  vector<int> tmp(r - l);
  vector<int> cnt(size, 0);
  int steps = ((log(maxnum) / log(2) + 1) + blocksize - 1) / blocksize;
  int cut = (1 << blocksize) - 1;
  int shift = 0;
  for(int i=0; i<steps; i++, shift += blocksize) {
    for(T it = l; it != r; ++it) {
      cnt[((*it >> shift) & cut)+1]++;
    }
    for(int it = 1; it < size; ++it) {
      cnt[it] += cnt[it-1];
    }
    for(T it = l; it != r; ++it) {
      tmp[cnt[(*it >> shift) & cut]++] = *it;
    }
    copy(tmp.begin(), tmp.end(), l);
    cnt.assign(size, 0);
  }
}

const int maxn = 10000000;
int n = maxn;
int tmp[maxn];
int s1[maxn];
int s2[maxn];

void gen() {
  srand(time(NULL));
  for(int i=0; i<n; i++)
    tmp[i] = rand();
}
void test_stl_sort() {
  for(int i=0; i<n; i++)
    s1[i] = tmp[i];
  double t1 = clock();
  sort(s1, s1+n);
  double t2 = clock();
  printf("STL: %.lf\n", (t2 - t1) / 1000.0);
}
void test_digit_sort() {
  for(int i=0; i<n; i++)
    s2[i] = tmp[i];
  double t1 = clock();
  digit_sort(s2, s2+n);
  double t2 = clock();
  printf("Digit: %.lf\n", (t2 - t1) / 1000.0);
}
void check() {
  for(int i=0; i<n; i++) {
    if (s1[i] != s2[i]) {
      for(int ii = 0; ii<n; ii++) {
        printf("%d ", s1[ii]);
      }
      printf("\n");
      for(int ii = 0; ii<n; ii++) {
        printf("%d ", s2[ii]);
      }
    }
    assert(s1[i] == s2[i]);
  }
  printf("Match\n");
}

int main() {
  gen();
  test_stl_sort();
  test_digit_sort();
  check();
  //printf("Execution time: %.lf\n", clock() / 1000.0);
  return 0;
}
