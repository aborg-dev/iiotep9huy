#include <list>


template<typename Data, typename Key>
class binomial_tree
{

  public:
  typedef binomial_tree<Data, Key> tree;
  typedef tree * ptree;

  private:
  Key key;
  Data data;
  int rank;
  std::list<ptree> Child;

  binomial_tree() {}
  binomial_tree(Data d, Key k)
  {
    data = d;
    key = k;
    rank = 0;
  }

  public:

  std::list<ptree> split()
  {
    typename std::list<ptree> T = this->Child;
    delete this;
    return T;
  }

  ptree merge(ptree B)
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

};

template<typename Data, typename Key>
class binomial_heap
{
  typename std::list< binomial_tree<Data, Key> * > L;
};

int main() { return 0; }
