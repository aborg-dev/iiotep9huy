#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <cassert>

#include "struct+extractor.cpp"
#include "digit_sort.cpp"

using namespace std;

template <class T>
void test_stl_sort(T l, T r) {
  double t1 = clock();
  sort(l, r);
  double t2 = clock();
  printf("STL: %.lf\n", (t2 - t1) / 1000.0);
}

template <class T, class BlockExtractor>
void test_digit_sort(T l, T r, BlockExtractor BE) {
  double t1 = clock();
  digit_sort(l, r, BE);
  double t2 = clock();
  printf("Digit: %.lf\n", (t2 - t1) / 1000.0);
}

template <class T>
void check(T l1, T r1, T l2, T r2) {
  while (l1 != r1) {
    assert(*l1 == *l2);
    l1++;
    l2++;
  }
  printf("Match!\n");
}

template <class Type, class Generator, class BlockExtractor>
void test(int n, Generator G, BlockExtractor BE) {
  vector<Type> T(n), S1(n), S2(n);
  generate(T.begin(), T.end(), G);
  copy(T.begin(), T.end(), S1.begin());
  copy(T.begin(), T.end(), S2.begin());

  //test_stl_sort(S1.begin(), S1.end());
  test_stl_sort(S1.begin(), S1.end());
  test_digit_sort(S2.begin(), S2.end(), BE);
  check(S1.begin(), S1.end(), S2.begin(), S2.end());

  T.clear(); S1.clear(); S2.clear();
}

const int int_N = 100000000;
const int longlong_N = 100000000;
const int string_N = 1000000;
const int pair_N = 100000000;

void test_all() {
  printf("n = %d, int:\n", int_N);
  test <int>(int_N, gen_int, int_extractor());
  printf("\n");

  printf("n = %d, long long:\n", longlong_N);
  test <long long>(longlong_N, gen_long_long, long_long_extractor());
  printf("\n");

  printf("n = %d, string:\n", string_N);
  test <string>(string_N, gen_string, string_extractor());
  printf("\n");

  printf("n = %d, pair:\n", pair_N);
  test < pair<int,int> >(pair_N, gen_pair, pair_extractor());
  printf("\n");
}

int main() {
  srand(time(NULL));
  test_all();
  return 0;
}
