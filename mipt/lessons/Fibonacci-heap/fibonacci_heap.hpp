#include <vector>

template<typename Key, typename Compare>
class fibonacci_heap
{
  protected:

  std::vector <Key> heap;

  public:

  typedef typename std::vector<Key>::iterator node_pointer;

  void push(Key k);

  Key top();
  
  void erase(node_pointer pnode);

  void merge(fibonacci_heap H1, fibonacci_heap H2);

};
