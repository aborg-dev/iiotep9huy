
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>


template<typename Data, typename Key>
class binomial_tree
{

  public:
  typedef binomial_tree<Data, Key> tree;
  typedef tree * ptree;

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
      std::swap(*this, *B);

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
  private:

  typename std::list< binomial_tree<Data, Key> * > L;

  public:

  typedef binomial_tree<Data, Key> tree;
  typedef binomial_tree<Data, Key> * ptree;
  typedef typename std::list<ptree>::iterator plist;

  void merge(binomial_heap * H)
  {
    int maxrank = 0;

    for(plist it = L.begin(); it != L.end(); ++it)
      maxrank = std::max(maxrank, (*it)->rank);

    for(plist it = H->L.begin(); it != H->L.end(); ++it)
      maxrank = std::max(maxrank, (*it)->rank);

    std::vector<ptree> D1(maxrank + 2), D2(maxrank + 2);
    for(plist it = L.begin(); it != L.end(); ++it)
      D1[(*it)->rank] = *it;

    for(plist it = H->L.begin(); it != H->L.end(); ++it)
      D2[(*it)->rank] = *it;

    L.clear();

    ptree carry = 0;
    for(int i=0; i <= maxrank + 1; ++i)
    {
      if (carry && D1[i] && D2[i])
      {
        L.push_back(carry);
        carry = 0;
      }

      if (D1[i])
        carry = D1[i]->merge(carry);
      if (D2[i])
        carry = D2[i]->merge(carry);

      if (carry && (carry->rank == i))
      {
        L.push_back(carry);
        carry = 0;
      }
    }
    delete H;
  }

  void insert(Data d, Key k)
  {
    binomial_heap<Data, Key> * H = new binomial_heap<Data, Key>;
    H->L.push_back(new tree(d, k));
    this->merge(H);
  }

  ptree findMin()
  {
    ptree T;
    for(plist it = L.begin(); it != L.end(); ++it)
      if (!T || (T->key > (*it)->key))
        T = *it;
    return T;
  }

  void popMin()
  {
    ptree pMin = findMin();
    binomial_heap<Data, Key> * H = new binomial_heap<Data, Key>;
    H->L = pMin->split();
    L->erase(pMin);
    this->merge(H);
  }

};

int main()
{
  binomial_heap<int, int> H;
  //H.insert(1, 3);
  //H.insert(5, 2);
  H.insert(9, 8);
  //H.insert(2, 2);
  H.insert(11, 7);
  binomial_tree<int, int> * Pnt;
  Pnt = H.findMin();
  printf("%d %d\n", Pnt->key, Pnt->data);
  return 0;
}
