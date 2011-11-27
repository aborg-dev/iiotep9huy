#include <iostream>
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>
#include <memory>
#include <algorithm>

#define DB(x) std::cerr << std::endl << #x << " : " << x << std::endl;

template <typename ElementType, typename ModType, 
          typename MergeFunc, typename ModFunc, typename ModUpdateFunc>
class IntervalTree
{

  private:

  size_t size_;
  std::vector<ElementType> tree;
  std::vector<ModType> modification;
  MergeFunc merge;
  ModFunc modify;
  ModUpdateFunc update_mod;
  ElementType element_zero_;
  ModType mod_zero_;

  public:

  IntervalTree() {}
  explicit IntervalTree(size_t size, ElementType element_zero, ModType mod_zero)
  {
    assert(size > 0);
    size_ = 1;
    element_zero_ = element_zero;
    mod_zero_ = mod_zero;
    while (size_ < size)
      size_ <<= 1;
    tree.assign(size, element_zero);
    modification.assign(size, mod_zero);
  }

  size_t size() const
  {
    return size_;
  }

  void RecalcNode(int index)
  {
    assert(index < size());
    int l = index*2, r = index*2 + 1;
    tree[index] = merge(modify(tree[l], modification[l]), modify(tree[r], modification[r]));
  }

  void UpdateRange(int root, int l, int r, int left_index, int right_index, ModType mod)
  {
    if (right_index < l || left_index > r)
      return;

    if (l == left_index && r == right_index)
      return void(modification[root] = update_mod(modification[root], mod));

    int m = (l + r) / 2;
    UpdateRange(root*2, l, m, left_index, right_index, mod);
    UpdateRange(root*2 + 1, m + 1, r, left_index, right_index, mod);
    RecalcNode(root);
  }

  ElementType GetValue(int root, int l, int r, int left_index, int right_index)
  {
    if (right_index < l || left_index > r)
      return element_zero_;

    if (l == left_index && r == right_index)
      return modify(tree[root], modification[root]);

    int m = (l + r) / 2;

    return merge(
      GetValue(root*2, l, m, left_index, right_index),
      GetValue(root*2 + 1, m + 1, r, left_index, right_index)
    );
  }

};

int main()
{
  //freopen("input.txt", "r", stdin);
  return 0;
}
