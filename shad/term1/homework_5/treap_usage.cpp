#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "Treap.hpp"

int main()
{
  Treap<int, int> treap;
  for (int i = 0; i < 50; ++i)
  {
    treap.insert(std::make_pair(rand() % (i + 10), i));
  }
  treap.printTree();

  return 0;
}
