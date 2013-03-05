#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <functional>

using std::vector;

template
<typename ElementType, typename Comparator = std::less<ElementType> >
class PriorityQueue
{
public:

  PriorityQueue()
  {
  }

  void push(ElementType element)
  {
    tree.push_back(element);
    liftUp(tree.size() - 1);
  }

  void pop()
  {
    if (tree.empty())
    {
      throw std::logic_error("pop: PriorityQueue is empty");
    }

    std::swap(tree[0], tree.back());
    tree.pop_back();
    liftDown(0);
  }

  ElementType top() const
  {
    if (tree.empty())
    {
      throw std::logic_error("top: PriorityQueue is empty");
    }

    return tree[0];
  }

private:

  inline int getParentIndex(int nodeIndex) const
  {
    return (nodeIndex - 1) >> 1;
  }

  inline int getLeftChildIndex(int nodeIndex) const
  {
    return nodeIndex * 2 + 1;
  }

  inline int getRightChildIndex(int nodeIndex) const
  {
    return nodeIndex * 2 + 2;
  }

  void liftUp(int nodeIndex)
  {
    int parentIndex = getParentIndex(nodeIndex);
    while (nodeIndex != 0 && comparator(tree[nodeIndex], tree[parentIndex]))
    {
      std::swap(tree[nodeIndex], tree[parentIndex]);
      nodeIndex = parentIndex;
      parentIndex = getParentIndex(nodeIndex);
    }
  }

  void liftDown(int nodeIndex)
  {
    int minChildIndex = nodeIndex;
    int leftChildIndex = getLeftChildIndex(nodeIndex);
    int rightChildIndex = getRightChildIndex(nodeIndex);
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
  }
}

bool checkIfSequencesAreSorted(const vector< vector<int> >& sequences)
{
  for (const auto& sequence : sequences)
  {
    if (!std::is_sorted(sequence.begin(), sequence.end()))
    {
      return false;
    }
  }
  return true;
}

struct SequenceElementInfo
{
  SequenceElementInfo(int value, int sequenceIndex, int position): 
    value(value),
    sequenceIndex(sequenceIndex),
    position(position)
  {
  }

  int value, sequenceIndex, position;
};

struct SequenceElementInfoComparator
{
  bool operator() (const SequenceElementInfo& eFirst, const SequenceElementInfo& eSecond)
  {
    return eFirst.value < eSecond.value;
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

  vector<int> sortedSequence(totalLength);
  for (int i = 0; i < totalLength; ++i)
  {
    SequenceElementInfo topElement = priorityQueue.top();
    priorityQueue.pop();
    sortedSequence[i] = topElement.value;
    if (topElement.position + 1 < sequences[topElement.sequenceIndex].size())
    {
      int newElementValue = sequences[topElement.sequenceIndex][topElement.position + 1];
      int newElementSequenceIndex = topElement.sequenceIndex;
      int newElementPosition = topElement.position + 1;

      priorityQueue.push(
        SequenceElementInfo(newElementValue, 
                            newElementSequenceIndex, 
                            newElementPosition)
      );
    }
  }
  return sortedSequence;
}

void printOutput(const vector<int>& sequence)
{
  for (int i = 0; i < sequence.size(); ++i)
  {
    std::cout << sequence[i] << " ";
  }
  std::cout << std::endl;
}

int main()
{
  vector< vector<int> > sequences;
  readInput(&sequences);

  if (!checkIfSequencesAreSorted(sequences))
  {
    throw std::logic_error("Wrong input data, sequences are not sorted");
  }

  vector<int> sortedSequence = mergeSortedSequences(sequences);

  printOutput(sortedSequence);
  return 0;
}

