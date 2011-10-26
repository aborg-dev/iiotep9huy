#include "binomial_heap.hpp"
#include <cassert>

#define tree \
        binomial_tree<Data, Key>

#define ptree \
        tree *

template<typename Data, typename Key>
ptree tree::merge(ptree B)
{
  if (!B)
    return this;

  assert(rank == B->rank);

  if (key < B->key)
    swap(this, B);

  ptree T = new tree(B->data, B->key);
  T->rank = rank + 1;
  T->Child.push_back(B);

  for(typename std::list<ptree>::iterator it = Child.begin(); it != Child.end(); ++it)
    T->Child.push_back(*it);

  delete this;
  delete B;
  return T;
}

template<typename Data, typename Key>
std::list<ptree> tree::split()
{
  typename std::list<ptree> T = this->Child;
  delete this;
  return T;
}

int main()
{
  return 0;
}
