#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>

#define DB(x) cerr << endl << #x << " : " << x << endl;

template<typename Key>
class binomial_tree
{

  private:

  int size_;

  public:
  typedef binomial_tree<Key> tree;
  typedef tree * ptree;

  Key key;
  int rank;
  std::list<ptree> Child;

  binomial_tree() {}
  binomial_tree(Key k)
  {
    key = k;
    rank = 0;
    size_ = 1;
  }

  int size()
  {
    return size_;
  }

  static std::list<ptree> split(ptree target)
  {
    typename std::list<ptree> T = target->Child;
    delete target;
    return T;
  }

  static ptree merge(ptree A, ptree B)
  {
    if (!A || !B)
      return A ? A : B;

    assert(A->rank == B->rank);

    if (A->key > B->key)
      std::swap(A, B);

    ptree T = new tree(A->data, A->key);
    T->rank = rank + 1;
    T->Child.push_back(B);
    T->Child.splice(T->Child.begin(), A, );

    for(typename std::list<ptree>::iterator it = Child.begin(); it != Child.end(); ++it)
      T->Child.push_back(*it);

    T->size_ = A->size_ + B->size_;

    delete A;
    delete B;
    return T;
  }

};
/*
template<typename Data, typename Key>
class binomial_heap
{
  private:

  typename std::list< binomial_tree<Key> * > L;
  int size_;

  public:

  typedef binomial_tree<Key> tree;
  typedef binomial_tree<Key> * ptree;
  typedef typename std::list<ptree>::iterator plist;

  int size()
  {
    return size_;
  }

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
    Size = 0;

    ptree carry = 0;
    for(int i=0; i <= maxrank + 1; ++i)
    {
      if (carry && D1[i] && D2[i])
      {
        L.push_back(carry);
        size_ += carry->size;
        carry = 0;
      }

      if (D1[i])
        carry = D1[i]->merge(carry);
      if (D2[i])
        carry = D2[i]->merge(carry);

      if (carry && (carry->rank == i))
      {
        L.push_back(carry);
        Size += carry->Size;
        carry = 0;
      }
    }
    delete H;
  }

  void insert(Key k)
  {
    binomial_heap<Key> * H = new binomial_heap<Data, Key>;
    H->L.push_back(new tree(d, k));
    this->merge(H);
  }

  plist findMinIter()
  {
    plist T = L.end();
    for(plist it = L.begin(); it != L.end(); ++it)
      if ((T == L.end()) || ((*T)->key > (*it)->key))
        T = it;
    return T;
  }

  ptree findMin()
  {
    plist it = findMinIter();
    return *it;
  }


  void popMin()
  {
    plist pMin = findMinIter();
    binomial_heap<Data, Key> * H = new binomial_heap<Data, Key>;
    H->L = tree::split(*pMin);
    L.erase(pMin);
    this->merge(H);
  }

};*/

int main()
{
  binomial_tree<int> * Pnt;
  binomial_tree<int>::split(Pnt);
  /*freopen("input.txt", "r", stdin);
  binomial_heap<int, int> H;
  binomial_tree<int, int> * Pnt;
  int n;
  scanf("%d", &n);
  for(int i=0; i<n; i++)
  {
    int prior;
    scanf("%d", &prior);
    H.insert(0, prior);
    //DB(H.size());
  }
  for(int i=0; i<n; i++)
  {
    if (i) printf(" ");
    Pnt = H.findMin();
    printf("%d", Pnt->key);
    H.popMin();
    //DB(H.size());
  }
  printf("\n");*/
  return 0;
}
