#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <vector>

using namespace std;

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
