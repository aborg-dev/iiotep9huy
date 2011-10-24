#include "fibonacci_heap.hpp"

CLASS_T
void CLASS_C::push(Key k)
{
  heap.push_back(node(k));
  node_pointer pnode = heap.end();
  this->merge(pnode);
}

CLASS_T
void CLASS_C::merge(node_pointer pnode)
{
  pnode->r = min_H -> r;
  min_H->r->l = pnode;
  min_H->r = pnode;
}

CLASS_T
Key CLASS_C::top()
{
  return min_H.key;
}

