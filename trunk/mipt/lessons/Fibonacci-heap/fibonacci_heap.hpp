#include <vector>

#define CLASS_T \
        template<typename Key, class Compare>

#define CLASS_C \
        fibonacci_heap<Key, Compare>

template<typedef Data, typedef Key>
class node
{
  Key key;
  typedef typename std::vector<node>::iterator node_pointer;
  node_pointer l, r, p, ch;
  int degree;
  bool mark;

  node();
  node(Key k)
  {
    l = r = p = ch = 0;
    key = k;
  }
};

template<typename Key, class Compare>
class fibonacci_heap
{


  public:

  typedef typename std::vector<node>::iterator node_pointer;

  void push(Key k);

  void merge(node_pointer pnode);

  Key top();


  protected:
  std::vector<node> heap;
  node min_H;

};


