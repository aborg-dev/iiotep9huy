#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <queue>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

const int blocksize = 100000;

char *block = new char[4];
void readInt(int &x, fstream &F) {
  (F).read(block, sizeof(int)); 
  x = (*((int*)(block)));
}

void printInt(int x, fstream &F) {
  F.write((char*)(&(x)), sizeof(x));
}

struct fblock {
  fstream f;
  size_t pos;
  size_t size;
  fblock() {}
  fblock(const char * fname, size_t _pos = 0, size_t _size = 0) {
    f.open(fname, ios::in | ios::out | ios::binary);
    size = _size;
    pos = _pos;
  }
  int get() {
    int ret;
    if (size == pos) return -1;
    pos++;
    readInt(ret, f);
    return ret;
  }
};

void external_sort(string target_filename) {
  fstream tmp("tmp.txt", ios::trunc | ios::in | ios::out | ios::binary);
  fstream target(target_filename.c_str(), ios::in | ios::binary);
  int n;
  readInt(n, target);
  cerr << n << endl;
  tmp << n << endl;

  int k = (n + blocksize-1)/blocksize;

  vector<int> arr(min(blocksize, n));
  vector<fblock> fpnt(k, fblock("tmp.txt"));
  priority_queue< pair<int, int> , vector<pair<int, int> >, greater<pair<int, int> > > Q;

  for(int i=0; i < k; i++) {
    for(int j=0; j < blocksize && i*blocksize + j < n; j++) {
      readInt(arr[j], target);
    }
    sort(arr.begin(), arr.end());

    fpnt[i].f.seekg(tmp.tellp());
    fpnt[i].size = min(blocksize, n-i*blocksize);
    fpnt[i].pos = 0;

    for(int j=0; j < blocksize && i*blocksize + j < n; j++) {
      cerr << arr[j] << endl;
      printInt(arr[j], tmp);
    }
  }
  for(int i=0; i < k; i++) {
    Q.push(make_pair(fpnt[i].get(), i));
  }
  cerr << endl;
  cerr << fpnt[0].get() << endl;
  cerr << fpnt[0].get() << endl;
  cerr << fpnt[0].get() << endl;
  cerr << fpnt[0].get() << endl;
  return;
  target.close();
  target.open(target_filename.c_str(), ios::out | ios::trunc | ios::binary);
  printInt(n, target);
  while (!Q.empty()) {
    pair<int,int> cur = Q.top(); Q.pop();
    int next = fpnt[cur.second].get();
    printInt(cur.first, target);
    cerr << cur.first << endl;

    if (next != -1) {
      Q.push(make_pair(next, cur.second));
    }
  }
  unlink("tmp.txt");
}

int main() {
  string target_filename = "input.txt";
  double t1 = clock();
  external_sort(target_filename);
  double t2 = clock();
  printf("%.lf\n", (t2-t1)/1000.0);
  return 0;
}

