

template<class node, class CmpFn>
class fibonacci_heap
{
  protected:

  vector<node> heap;

  public:

  typedef vector<node>::iterator node_pointer;
  
  public:
  
  void push(node nd);

  void erase(node_pointer p_nd);

  void pop();

  node_pointer top();

  private:

  node_pointer p_max;

}
