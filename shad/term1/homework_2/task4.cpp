#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <functional>

using std::vector;

template
<typename ElementType, typename Comparator = std::less<ElementType>>
class PriorityQueue
{
public:

  PriorityQueue()
  {
  }

  void push(ElementType key)
  {
    tree.push_back(key);
    liftUp(tree.size() - 1);
  }

  void pop()
  {
    if (tree.empty())
      throw std::logic_error("pop: PriorityQueue is empty");

    std::swap(tree[0], tree.back());
    tree.pop_back();
    liftDown(0);
  }

  ElementType top() const
  {
    if (tree.empty())
      throw std::logic_error("top: PriorityQueue is empty");

    return tree[0];
  }

private:

  void liftUp(int nodeIndex)
  {
    int parentIndex = (nodeIndex - 1) >> 1;
    while (nodeIndex != 0 && comparator(tree[nodeIndex], tree[parentIndex]))
    {
      std::swap(tree[nodeIndex], tree[parentIndex]);
      nodeIndex = parentIndex;
      parentIndex = (nodeIndex - 1) >> 1;
    }
  }

  void liftDown(int nodeIndex)
  {
    int minChildIndex = nodeIndex;
    int leftChildIndex = nodeIndex * 2 + 1;
    int rightChildIndex = nodeIndex * 2 + 2;
    if (leftChildIndex < tree.size() && comparator(tree[leftChildIndex], tree[minChildIndex]))
    {
      minChildIndex = leftChildIndex;
    }
    if (rightChildIndex < tree.size() && comparator(tree[rightChildIndex], tree[minChildIndex]))
    {
      minChildIndex = rightChildIndex;
    }
    if (minChildIndex != nodeIndex)
    {
      std::swap(tree[nodeIndex], tree[minChildIndex]);
      liftDown(minChildIndex);
    }
  }

  vector<ElementType> tree;
  Comparator comparator;
  
};

void readInput(vector< vector<int> >* sequences)
{
  int sequencesNumber, sequenceLength;
  std::cin >> sequencesNumber >> sequenceLength;
  sequences->resize(sequencesNumber);

  for (int i = 0; i < sequencesNumber; ++i)
  {
    (*sequences)[i].resize(sequenceLength);
    for (int j = 0; j < sequenceLength; ++j)
    {
      std::cin >> (*sequences)[i][j];
    }
    if (!std::is_sorted((*sequences)[i].begin(), (*sequences)[i].end()))
    {
      std::cerr << "fail" << std::endl;
      exit(-1);
    }
  }
}

struct SequenceElementInfo
{
  SequenceElementInfo(int value, int elementSequence, int elementPosition): 
    value(value),
    elementSequence(elementSequence),
    elementPosition(elementPosition)
  {
  }

  int value, elementSequence, elementPosition;
};

struct SequenceElementInfoComparator
{
  bool operator() (const SequenceElementInfo& e_first, const SequenceElementInfo& e_second)
  {
    return e_first.value < e_second.value;
  }
};

vector<int> mergeSortedSequences(const vector< vector<int> >& sequences)
{
  PriorityQueue<SequenceElementInfo, SequenceElementInfoComparator> priorityQueue;
  size_t totalLength = 0;

  for (int i = 0; i < sequences.size(); ++i)
  {
    totalLength += sequences[i].size();
    priorityQueue.push(SequenceElementInfo(sequences[i][0], i, 0));
  }

  vector<int> result(totalLength);
  for (int i = 0; i < totalLength; ++i)
  {
    SequenceElementInfo top = priorityQueue.top();
    priorityQueue.pop();
    result[i] = top.value;
    if (top.elementPosition + 1 < sequences[top.elementSequence].size())
    {
      priorityQueue.push(SequenceElementInfo(
        sequences[top.elementSequence][top.elementPosition + 1], 
        top.elementSequence, top.elementPosition + 1));
    }
  }
  return result;
}

void printOutput(const vector<int>& sequence)
{
  for (int i = 0; i < sequence.size(); ++i)
  {
    std::cout << sequence[i] << " ";
  }
  std::cout << std::endl;
}

/*
void gen(int n, int m)
{
  freopen("input.txt", "w", stdout);
  printf("%d %d\n", n, m);
  for (int i = 0; i < n; ++i)
  {
    vector<int> v(m);
    for (int j = 0; j < m; ++j)
      v[j] = rand() % 100;
    std::sort(v.begin(), v.end());
    for (int j = 0; j < m; ++j)
      std::cout << v[j] << " ";
    std::cout << std::endl;
  }
  exit(0);
}
*/

int main()
{
  // gen(100, 100);
  // freopen("input.txt", "r", stdin); // REMOVE
  vector< vector<int> > sequences;
  readInput(&sequences);

  vector<int> sortedSequence = mergeSortedSequences(sequences);

  printOutput(sortedSequence);
  return 0;
}
